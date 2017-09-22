cbuffer PerFrameParameters : register(b0)
{
	float4x4 ViewTM;
	float4x4 ProjTM;
	float4x4 WorldTM;
	float4 CameraPos;
	float4 params;
};

//Atmospheric scattering variables from O'Neil's implementation
cbuffer PerFrameParameters : register(b1)
{
	float4 SS;		
	float4 IWL;
	float4 radius;
	float4 K;
	float4 S;
	float4 GG;
	float4 lightDir;
}

cbuffer SystemParams : register(b2)
{
	int4 nSamp;
	float4 fSamp;
}

cbuffer PlanetParams : register(b3)
{
	float4 PlanetPosition;
	float4 PlanetRadiuses;
}

cbuffer SunParams : register(b4)
{
	float4 SunPosition;
}

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float ScaleDepth = S.w;
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
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 PositionPS : SV_POSITION;
	float4 C0 : COLOR0;
	float4 C1 : COLOR1;
	float3 T0 : TEXCOORD0;
	float3 Normal : TEXCOORD2;
};


// SkyFromSpace Technique
VS_OUTPUT VS(VS_INPUT input)
{
	float AttenuateFudgeGround = 0.15f;
	int nSamples = nSamp.x;
	float fSamples = nSamp.x;
	float OuterRadius = radius.x;
	float OuterRadiusSquared = OuterRadius*OuterRadius;
	float InnerRadius = radius.z;
	float InnerRadiusSquared = InnerRadius*InnerRadius;
	float KrESun = K.x;
	float KmESun = K.y;
	float Kr4PI = K.z;
	float Km4PI = K.w;
	float Scale = S.x;
	float ScaleOverScaleDepth = S.y;
	float InvScaleDepth = S.z;
	float ScaleDepth = S.w;
	float3 InvWaveLength = IWL.xyz;				
	float SkyScale = SS.x;
	float3 CamPos = CameraPos.xyz;
	float CamHeight = length(CamPos);
	float CamHeightSquared = CamHeight*CamHeight;
	float3 light = lightDir.xyz;

	float3 pos = input.Position + (input.Normal * SkyScale);
	float3 posWS = mul(float4(pos, 1.0f), WorldTM).xyz;

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
	float3 samplePoint = rayStart + (sampleRay * 0.5f);

	float3 frontColor = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < nSamples; i++) 
	{
		float height = length(samplePoint);
		float depth = exp(ScaleOverScaleDepth * (InnerRadius - height));
		float lightAngle = dot(-light, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = startOffset + (depth * (scale(lightAngle) - scale(cameraAngle)));
		float3 attenuate = exp(-scatter * ((InvWaveLength * Kr4PI) + Km4PI));

		frontColor += attenuate * (depth * scaledLength);

		samplePoint += sampleRay;
	}

	VS_OUTPUT output;
	output.PositionPS = mul(float4(pos, 1.0f), mul(ViewTM, ProjTM));
	output.C0 = float4(frontColor * (InvWaveLength * KrESun), 1.0f);
	output.C1 = float4(frontColor * KmESun, 1.0f);
	output.T0 = CamPos - posWS;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
  float3 light = lightDir.xyz;

  float fCos = dot(-light, input.T0) / length(input.T0);
  float fCos2 = fCos * fCos;
  float4 color = (getRayleighPhase(fCos2) * input.C0) + (getMiePhase(fCos, fCos2, GG.x, GG.y) * input.C1);
  color.a = color.b;
 // color = input.C0;
  color.a = 1;
  return color;
}


					
/*
struct VS_INPUT
{
	float3 pos	: POSITION;
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
};


VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT output;

	float3 P = mul(float4(input.pos, 1), (float4x3)ViewTM);
	output.pos = mul(float4(P, 1), ProjTM);
	
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	return 1.81;
}


					*/