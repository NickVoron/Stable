cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 DirLightDirectionEyeSpace;
	float4 ScreenTexelUV;
	float4 cameraPos;
}

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 colorOBJ;
};

struct VS_INPUT
{
	float3 Pos			: POSITION;
	float2 TileUV		: TEXCOORD0;
	float4 TileCoord	: TEXCOORD1;
	float3 TileColor	: TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 Pos				: SV_POSITION;
};


struct HS_OUTPUT
{
	float4 Pos				: SV_POSITION;
};

struct DS_OUTPUT
{
	float4 Pos				: SV_POSITION;
	float2 UV				: COLOR0;
	float4 ScreenProjCoords : COLOR1;
};




Texture2D    ScreenShadowMap	: register(t0);
Texture2D    depthMap			: register(t2);
SamplerState sampScreenShadow	: register(s0);

Texture2D    heightMap		: register( t3 );
Texture2D    normalMap		: register( t4 );
Texture2D    blendtMap		: register( t5 );
Texture2D    diffuseTex		: register(t6);

SamplerState heightSampler	: register(s3);


Texture2D    t0			: register( t7 );
Texture2D    t1			: register( t8 );
Texture2D    t2			: register( t9 );
SamplerState texSampler	: register( s4 );



VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;

	float2 addition = (In.TileCoord.xy * In.TileCoord.zw) + In.TileUV.xy * In.TileCoord.zw;
	Out.Pos = float4(float3(addition.x, 0, addition.y), 1);
	//Out.UV = 0;
	//Out.Color = In.TileColor;

	return Out;
}

struct HS_ConstantOutput
{
	// Tess factor for the FF HW block
	float fTessFactor[4]    : SV_TessFactor;
	float fInsideTessFactor[2] : SV_InsideTessFactor;
};


HS_ConstantOutput HS_Constant( InputPatch<VS_OUTPUT, 4> I )
{
	float maxf = 500.0;
	HS_ConstantOutput O;
	
	float4 camera = float4(cameraPos.x, 0, cameraPos.z, 1);

	float f0 = min( length(I[0].Pos - camera), length(I[3].Pos - camera) );
	float f1 = min( length(I[0].Pos - camera), length(I[1].Pos - camera) );
	float f2 = min( length(I[1].Pos - camera), length(I[2].Pos - camera) );
	float f3 = min( length(I[2].Pos - camera), length(I[3].Pos - camera) );
	
	O.fTessFactor[0] = max( maxf / f0, 1 );
	O.fTessFactor[1] = max( maxf / f1, 1 );
	O.fTessFactor[2] = max( maxf / f2, 1 );
	O.fTessFactor[3] = max( maxf / f3, 1 );

	//O.fTessFactor[0] = 1;
	//O.fTessFactor[1] = 1;
	//O.fTessFactor[2] = 1
	//O.fTessFactor[3] = 1;

	O.fInsideTessFactor[0] = (O.fTessFactor[0] + O.fTessFactor[1] + O.fTessFactor[2] + O.fTessFactor[3]) / 4;
	O.fInsideTessFactor[1] = O.fInsideTessFactor[0];
	return O;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[patchconstantfunc("HS_Constant")]
[outputcontrolpoints(4)]
HS_OUTPUT HS( InputPatch<VS_OUTPUT, 4> I, uint uCPID : SV_OutputControlPointID )
{
	VS_OUTPUT input = (VS_OUTPUT)I[uCPID];
	
	HS_OUTPUT output;
	output.Pos = input.Pos;

	return output;
}

[domain("quad")]
DS_OUTPUT DS( HS_ConstantOutput HSConstantData, const OutputPatch<HS_OUTPUT, 4> I, float2 uvt : SV_DomainLocation )
{
	DS_OUTPUT Out;

	float4 p = I[0].Pos + uvt.x * (I[1].Pos - I[0].Pos) + uvt.y * (I[3].Pos - I[0].Pos);

	float2 geoUV = p.xz/4096;
	p.y = (heightMap.SampleLevel(heightSampler, geoUV, 0).x  - 1.0f )*350;
	//p.y = 0;

	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float4 posEyeSpace = mul(p, WorldViewTM);
	Out.Pos = mul(posEyeSpace, ProjTM);
	Out.UV = p.xz;

	//shadow
	Out.ScreenProjCoords.xy = Out.Pos.xy * float2(0.5f, -0.5f) + Out.Pos.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Pos.zw;

	return Out;
}

float4 PS(DS_OUTPUT In ) : SV_TARGET0
{

	float4 blend = blendtMap.SampleLevel(texSampler, In.UV/ 4096, 0);
	float4 diffuse = diffuseTex.Sample(texSampler, In.UV / 4096);
	//normals
	float3 normal = normalMap.SampleLevel(texSampler, In.UV / 4096, 0);
	normal = 2 * normal - 1.0f;
	normal = normal.xzy;
	
	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float3 normEyeSpace = normalize(mul(normal, (float3x3)WorldViewTM));

	float4 t0Color = t0.Sample(texSampler, In.UV / 16);
	float4 t1Color = t1.Sample(texSampler, In.UV / 10) * blend.y;
	float4 t2Color = t2.Sample(texSampler, In.UV / 20) * blend.z;
		

	float4 texColor = (t0Color*blend.x + t1Color + t2Color)/(blend.x+ blend.y+blend.z);
	//float4 texColor = diffuse;
	
	float dotNL = saturate(dot(normalize(normEyeSpace), DirLightDirectionEyeSpace.xyz));

	float shadow = ScreenShadowMap.Sample(sampScreenShadow, In.ScreenProjCoords.xy / In.ScreenProjCoords.w).r;

	float radiance = 1.0*float3(1.0, 1.0, 1.0) * (dotNL* shadow) + float3(0.3, 0.3, 0.3);
	//float radiance = float3(1.0, 1.0, 1.0) * (shadow) + float3(0.3, 0.3, 0.3);

	
	float3 resultColor = 1.0*radiance*texColor.xyz;
	//float3 resultColor = radiance;
	
	return float4(resultColor, 1);
	//return texColor;


	//return float4(height.x, height.y, height.z, 1);
	//return float4(In.Color, 1);
	//return float4(1, 1, 0, 1);
	//return float4(In.UV, 0, 1);
	//return float4(1,1, 0, 1);
}
