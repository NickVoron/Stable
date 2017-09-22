cbuffer PerFrameParameters {
	uniform float4x4 WorldViewProj;
	uniform float4x4 World;
	uniform float3 CamPos;
	uniform float CamHeight;
	uniform float CamHeightSquared;
	uniform float Time;
};

//Cloud properties for their speed and shadow qualities
cbuffer CloudParameters {
	uniform float CloudSpeed = .001f;
	uniform float CloudHeight = .002f;
	uniform float CloudShadowIntensity = .2f;
	uniform float CloudScale = 1.0f;
	uniform float CloudRotateDirection = -1.0f;
};

//Material properties - largely copy n paste from core shaders
cbuffer PerMaterial {
	uniform float3 MatDiffuse = { 1.0f, 1.0f, 1.0f};
	uniform float3 MatAmbient = { 0.0f, 0.0f, 0.0f};
	uniform float3 MatEmissive = { 0.0f, 0.0f, 0.0f};
	uniform float3 MatSpecular = { 0.0f, 0.0f, 0.0f};
	uniform float MatShininess = 16.0f;
	uniform float Alpha = 1.0f;
	uniform float3 GlobalAmbient : GLOBALAMBIENT = { 0.f, 0.f, 0.f };
	uniform Light SunLight : LIGHTLIST;
	uniform bool UseSpecularMap = false;
	//Multiply albedo by this to dim the color intensity
	uniform float FudgeFactor = .35f;
	//Fudge factors for tweaking the output.C1 color for the dark side (essentially ambient qualities so its not pitch black)
	uniform float AttenuateFudgeGround = .15f;
	uniform float AttenuateFudgeClouds = .02f;
	//Adjust the transparency of the cloud layer
	uniform float CloudAlpha = 1.0f;
};

//Atmospheric scattering variables from O'Neil's implementation
cbuffer ScatteringParameters {
	uniform int nSamples = 2;
	uniform float fSamples = 2.0f;

	uniform float SkyScale = 2.5f;

	uniform float3 InvWaveLength = {5.602, 9.478, 19.646};
	uniform float OuterRadius = 102.5;
	uniform float OuterRadiusSquared = 10506.25;

	uniform float InnerRadius = 100.0;
	uniform float InnerRadiusSquared = 10000.0;

	uniform float KrESun = .0375;
	uniform float KmESun = .0225;
	uniform float Kr4PI = .0314;
	uniform float Km4PI = .0188;
	uniform float Scale = .2;
	uniform float ScaleOverScaleDepth = .8;
	uniform float InvScaleDepth = 4;
	uniform float ScaleDepth = .25;
	uniform float G = -.95;
	uniform float GSquared = .9025;
};

//All the textures - specular map is optional, rest are not
uniform Texture2D DiffuseMap;
uniform Texture2D LightsMap;
uniform Texture2D CloudMap;
uniform Texture2D NormalMap;
uniform Texture2D SpecularMap;

//Just need one sampler for this
uniform SamplerState MapSampler {
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//Normal map vertex input
struct VSInputNm {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	float3 Binormal : BINORMAL;
	float3 Tangent : TANGENT;
};

//Normal map vertex output
struct VSOutputNm {
	float4 PositionPS : SV_POSITION;
	float3 PositionWS : TEXCOORD0;
	float3 T0 : TEXCOORD1;
	float3 NormalWS : TEXCOORD2;
	float3 TangentWS : TEXCOORD3;
	float3 BinormalWS : TEXCOORD4;
	float4 C0 : COLOR0;
	float4 C1 : COLOR1;
};

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return ScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0f;
	return 0.75 + 0.75*fCos2;
}

// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
}

// Returns the far intersection point of a line and a sphere
float getFarIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B + sqrt(fDet));
}

float2 Rotate(float2 coord, float time, float dir) {
	coord.x += (sin(time) * dir);
	return coord;
}

//Simple vertex input/output structures

struct VSInput {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VSOutput {
  float4 PositionPS : SV_POSITION;
  float4 C0 : COLOR0;
  float4 C1 : COLOR1;
  float3 T0 : TEXCOORD;
};

struct VSOutput2 {
  float4 PositionPS : SV_POSITION;
  float3 NormalWS : NORMAL;
  float4 C0 : COLOR0;
  float4 C1 : COLOR1;
  float3 T0 : TEXCOORD;
};

//
//
// SkyFromSpace Technique
//
//

VSOutput VS_SkyFromSpace(float3 Position : POSITION, float3 Normal : NORMAL) {
  float3 pos = Position + (Normal * SkyScale);
  float3 posWS = mul(float4(pos, 1.0f), World).xyz;
  
  float3 ray = posWS - CamPos;
  float far = length(ray);
  ray /= far;
  
  float near = getNearIntersection(CamPos, ray, CamHeightSquared, OuterRadiusSquared);
  float3 rayStart = CamPos + (ray * near);
  far -= near;
  
  float startAngle = dot(ray, rayStart) / OuterRadius;
  float startDepth = exp(-InvScaleDepth);
  float startOffset = startDepth * scale(startAngle);
  
  float sampleLength = far / fSamples;
  float scaledLength = sampleLength * Scale;
  float3 sampleRay = ray * sampleLength;
  float3 samplePoint = rayStart + (sampleRay * .5f);
  
  float3 frontColor = float3(0.0f, 0.0f, 0.0f);
  
  for(int i = 0; i < nSamples; i++) {
    float height = length(samplePoint);
    float depth = exp(ScaleOverScaleDepth * (InnerRadius - height));
    float lightAngle = dot(-SunLight.Direction, samplePoint) / height;
    float cameraAngle = dot(ray, samplePoint) / height;
    float scatter = startOffset + (depth * (scale(lightAngle) - scale(cameraAngle)));
    float3 attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));
    
    frontColor += attenuate * (depth * scaledLength);
    
    samplePoint += sampleRay;
  }
  
  VSOutput output;
  output.PositionPS = mul(float4(pos, 1.0f), WorldViewProj);
  output.C0 = float4(frontColor * (InvWaveLength * KrESun), 1.0f);
  output.C1 = float4(frontColor * KmESun, 1.0f);
  output.T0 = CamPos - posWS;
  return output;
}

float4 PS_SkyFromSpace(VSOutput input) : SV_TARGET {
  float fCos = dot(-SunLight.Direction, input.T0) / length(input.T0);
  float fCos2 = fCos * fCos;
  float4 color = (getRayleighPhase(fCos2) * input.C0) + (getMiePhase(fCos, fCos2, G, GSquared) * input.C1);
  color.a = color.b;
  return color;
}

technique10 SkyFromSpace {
  pass pass0 {
    SetGeometryShader(0);
    SetVertexShader(CompileShader(vs_4_0, VS_SkyFromSpace()));
    SetPixelShader(CompileShader(ps_4_0, PS_SkyFromSpace()));
  }
}

//
//
// GroundFromSpace Technique
//
//

VSOutputNm VS_GroundFromSpace(VSInputNm input) {
	float3 posWS = mul(float4(input.Position, 1.0f), World).xyz;
	float3 ray = posWS - CamPos;
	posWS = normalize(posWS);
	float far = length(ray);
	ray /= far;

	float near = getNearIntersection(CamPos, ray, CamHeightSquared, OuterRadiusSquared);
	
	float3 rayStart = CamPos + (ray * near);
	far -= near;

	float depth = exp((InnerRadius - OuterRadius) * InvScaleDepth);
	float camAngle = dot(-ray, posWS);
	float camScale = scale(camAngle);
	float lightAngle = dot(-SunLight.Direction, posWS);
	float lightScale = scale(lightAngle);
	float camOffset = depth * camScale;
	float temp = (lightScale + camScale);

	float sampleLength = far / fSamples;
	float scaledLength = sampleLength * Scale;
	float3 sampleRay = ray * sampleLength;
	float3 samplePoint = rayStart + (sampleRay * .5f);
	
	float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	float3 attenuate;

	for(int i = 0; i < nSamples; i++) {
		float height = length(samplePoint);
		float depth2 = exp(ScaleOverScaleDepth * (InnerRadius - height));
		float scatter = (depth2 * temp) - camOffset;
		attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));
		frontColor += attenuate * (depth2 * scaledLength);
		samplePoint += sampleRay;
	}

	VSOutputNm output;
	output.PositionPS = mul(float4(input.Position, 1.0f), WorldViewProj);
	output.PositionWS = posWS;
	//Transform BNT to world space
	output.NormalWS = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
	output.TangentWS = normalize(mul(float4(input.Tangent, 0.0f), World).xyz);
	output.BinormalWS = normalize(mul(float4(input.Binormal, 0.0f), World).xyz);
	output.C0 = float4(frontColor * ((InvWaveLength * KrESun) + KmESun), 1.0f);
	output.C1 = float4(attenuate + AttenuateFudgeGround, 1.0f); //Fudge the attenuation
	output.T0 = float3(input.TexCoord, 0.0f);

	return output;
}

float4 PS_GroundFromSpace(VSOutputNm input) : SV_TARGET {

	//float4 color2 = input.C0 + (.25f * input.C1);
	//return color2;
	/*
	float4 albedo = DiffuseMap.Sample(MapSampler, input.T0.xy);
	albedo *= .15f;
	float shadowHeight = CloudHeight * dot(input.NormalWS, --SunLight.Direction);
	float2 shadowAngle = input.T0.xy - float2(shadowHeight, shadowHeight);
	float2 cloudCoords = Rotate(shadowAngle, Time * CloudSpeed, CloudRotateDirection);
	float cloudShadow = (-(CloudMap.Sample(MapSampler, cloudCoords).r)) * CloudShadowIntensity;

	return input.C0 + ((albedo + cloudShadow) * input.C1);*/

	//Get the normal
	float4 texNormal = Expand(NormalMap.Sample(MapSampler, input.T0.xy));

	//Transform it to world space
	texNormal = float4((input.NormalWS * texNormal.z) + (texNormal.x * input.TangentWS + texNormal.y * -input.BinormalWS), 0.0f);

	float3 normalWS = normalize(texNormal.xyz);

	//Get the diffuse map color
	float4 albedo = albedo = DiffuseMap.Sample(MapSampler, input.T0.xy);
	albedo *= FudgeFactor;

	//Get the specular map color
	float4 spec = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(UseSpecularMap) {
		spec = SpecularMap.Sample(MapSampler, input.T0.xy);
	}

	//Calc the view V vector for specular highlights
	float3 V = normalize(CamPos - input.PositionWS);
	float3 lightContribs = float3(0.0f, 0.0f, 0.0f);
	
	Material mat;
	mat.Ambient = MatAmbient;
	mat.Diffuse = MatDiffuse;
	mat.Specular = MatSpecular;
	mat.Shininess = MatShininess;

	//Only ever one light - the sun

	LightResult result = ComputeSingleLight(SunLight, input.PositionWS, normalWS, V, mat);
	float3 ambient = result.Ambient;
	float3 diffuse = result.Diffuse;
	float3 specular = result.Specular;

	diffuse = (diffuse + ambient) * albedo.rgb;

	specular *= spec.xyz;

	//Don't need spot effect
	lightContribs += ComputeAttenuation(input.PositionWS, SunLight) * (diffuse + specular);

	float4 color = float4(lightContribs, Alpha * albedo.a);//float4((MatAmbient * GlobalAmbient) + MatEmissive + lightContribs, Alpha * albedo.a);
	//return color;

	//For clouds on the darkside
	float ndotl = max(dot(normalWS, -SunLight.Direction), 0);

	//Get the city lights on the dark side, should not be visible on the lighted side
	float4 lights = max(dot(normalWS, SunLight.Direction), 0) * LightsMap.Sample(MapSampler, input.T0.xy);

	float shadowHeight = CloudHeight * dot(input.NormalWS, SunLight.Direction);
	float2 shadowAngle = input.T0.xy - float2(shadowHeight, shadowHeight);
	float2 cloudCoords = Rotate(shadowAngle, Time * CloudSpeed, CloudRotateDirection);
	float cloudShadow = (-(CloudMap.Sample(MapSampler, cloudCoords).r)) * (CloudShadowIntensity * ndotl);

	return input.C0 + ((color + lights + cloudShadow) * input.C1);
}	

technique10 GroundFromSpace {
	pass pass0 {
	    SetGeometryShader(0);
		SetVertexShader(CompileShader(vs_4_0, VS_GroundFromSpace()));
		SetPixelShader(CompileShader(ps_4_0, PS_GroundFromSpace()));
	}
}

//
//
// CloudsFromSpace Technique
//
//


VSOutput2 VS_CloudsFromSpace(VSInput input) {
	float3 pos = input.Position + (input.Normal * CloudScale);
	float3 posWS = mul(float4(pos, 1.0f), World).xyz;

	float3 ray = posWS - CamPos;
	posWS = normalize(posWS);
	float far = length(ray);
	ray /= far;

	float near = getNearIntersection(CamPos, ray, CamHeightSquared, OuterRadiusSquared);
	
	float3 rayStart = CamPos + (ray * near);
	far -= near;

	float depth = exp((InnerRadius - OuterRadius) * InvScaleDepth);
	float camAngle = dot(-ray, posWS);
	float camScale = scale(camAngle);
	float lightAngle = dot(-SunLight.Direction, posWS);
	float lightScale = scale(lightAngle);
	float camOffset = depth * camScale;
	float temp = (lightScale + camScale);

	float sampleLength = far / fSamples;
	float scaledLength = sampleLength * Scale;
	float3 sampleRay = ray * sampleLength;
	float3 samplePoint = rayStart + (sampleRay * .5f);
	
	float3 frontColor = float3(0.0f, 0.0f, 0.0f);
	float3 attenuate;

	for(int i = 0; i < nSamples; i++) {
		float height = length(samplePoint);
		float depth2 = exp(ScaleOverScaleDepth * (InnerRadius - height));
		float scatter = (depth2 * temp) - camOffset;
		attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));
		frontColor += attenuate * (depth2 * scaledLength);
		samplePoint += sampleRay;
	}

	VSOutput2 output;
	output.PositionPS = mul(float4(pos, 1.0f), WorldViewProj);
	output.NormalWS = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
	output.C0 = float4(frontColor * ((InvWaveLength * KrESun) + KmESun), 1.0f);
	output.C1 = float4(attenuate + AttenuateFudgeClouds, 1.0f); //Fudge the attenuation
	output.T0 = float3(input.TexCoord, 0.0f);
	return output;
}

float4 PS_CloudsFromSpace(VSOutput2 input) : SV_TARGET {
	float2 cloudCoord = Rotate(input.T0.xy, Time * CloudSpeed, CloudRotateDirection);
	float4 cloudColor = CloudMap.Sample(MapSampler, cloudCoord);
	cloudColor.a = CloudAlpha;
	return cloudColor * input.C1;
}

technique10 CloudsFromSpace {
	pass pass0 {
		SetGeometryShader(0);
		SetVertexShader(CompileShader(vs_4_0, VS_CloudsFromSpace()));
		SetPixelShader(CompileShader(ps_4_0, PS_CloudsFromSpace()));
	}
}

technique10 Planet {

	pass GroundFromSpace {
		SetGeometryShader(0);
		SetVertexShader(CompileShader(vs_4_0, VS_GroundFromSpace()));
		SetPixelShader(CompileShader(ps_4_0, PS_GroundFromSpace()));
	}
	
	pass SkyFromSpace {
	    SetGeometryShader(0);
		SetVertexShader(CompileShader(vs_4_0, VS_SkyFromSpace()));
		SetPixelShader(CompileShader(ps_4_0, PS_SkyFromSpace()));
	}
	
	
	pass CloudsFromSpace {
		SetGeometryShader(0);
		SetVertexShader(CompileShader(vs_4_0, VS_CloudsFromSpace()));
		SetPixelShader(CompileShader(ps_4_0, PS_CloudsFromSpace()));
	}
}

