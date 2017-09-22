
cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 dirLightDirectionEyeSpace;
	float4 ScreenTexelUV;
	float4 cameraPos;
}


cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
};




//
float4 sunLightColor()
{
	return float4(0.7f, 0.7f, 0.7f, 1.0f);
}
float4 lightAmbient()
{
	return float4(0.3f, 0.3f, 0.3f, 1.0f);
}

//textures
Texture2D    sailDiffTex	: register(t3);
SamplerState diffSampler	: register(s3);

Texture2D    sailNormTex	: register(t4);
SamplerState normSampler	: register(s4);


//structs
struct VS_INPUT
{
	float3 vPos		: POSITION;
	float2 vTex		: TEXCOORD0;
	float3 vNorm	: NORMAL;
	//float3 vBiNorm	: BINORMAL;
	//float3 vTan		: TANGENT;
};

struct VS_SAILS_OUTPUT
{
	float4 Position			: SV_POSITION;
	float2 Tex				: TEXCOORD0;
	float3 Norm				: TEXCOORD1;
	//float3 BiNorm			: TEXCOORD2;
	//float3 Tan				: TEXCOORD3;
	float Depth				: TEXCOORD4;
	float4 ScreenProjCoords : TEXCOORD5;
};

//for verlet sail
VS_SAILS_OUTPUT VS(VS_INPUT input)
{
	VS_SAILS_OUTPUT Output;
	float4x4 worldView = mul(WorldTM, ViewTM);
	float4 pos = mul(float4(input.vPos,1), worldView);

	Output.Position = mul(pos, ProjTM);
	Output.Tex = input.vTex.xy;
	Output.Norm = input.vNorm;
	//Output.BiNorm = vBiNorm;
	//Output.Tan = vTan; 
	Output.Depth = pos.z;
	Output.ScreenProjCoords.xy = Output.Position.xy * float2(0.5f, -0.5f) + Output.Position.ww * 0.5f;
	Output.ScreenProjCoords.zw = Output.Position.zw;
	Output.ScreenProjCoords.xy += Output.ScreenProjCoords.ww * 0.5f * ScreenTexelUV.xy;
	return Output;    
}


void PS(VS_SAILS_OUTPUT In, out float4 outColor : SV_TARGET0)
{ 
	float shadow = 1;//tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float nDotL = abs(dot( normalize(In.Norm), dirLightDirectionEyeSpace.xyz));
	float4 l = saturate( shadow * nDotL * sunLightColor() + lightAmbient());
	
	outColor = l *  sailDiffTex.Sample(diffSampler, In.Tex);
	//Output.color.xyz = abs(In.BiNorm);

}









////////////////
//cbuffer PerCamera : register(b0)
//{
//	float4x4 ProjTM;
//	float4x4 ViewTM;
//	float4 DirLightDirectionEyeSpace;
//	float4 ScreenTexelUV;
//	float4 cameraPos;
//}
//
//cbuffer PerObject : register(b1)
//{
//	float4x4 WorldTM;
//};
//
//Texture2D    ScreenShadowMap : register(t0);
//Texture2D    depthMap			: register(t2);
//
//Texture2D    textureMap			: register(t5);
//SamplerState textureSampler		: register(s5);
//
//
//
////--------------------------------------------------------------------------------------
//
//struct MainGeomInput
//{
//	float3 Position    : POSITION;
//};
//
//struct MainGeomOutput
//{
//	float4 Position         : SV_POSITION;
//};
//
//MainGeomOutput VS(MainGeomInput In)
//{
//	MainGeomOutput Out;
//
//	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
//	float4 posEyeSpace = mul(float4(In.Position, 1), WorldViewTM);
//
//
//	Out.Position = mul(posEyeSpace, ProjTM);
//	return Out;
//}
//
//void PS(MainGeomOutput In, out float4 outColor : SV_TARGET0)
//{
//	outColor = 1;
//}
//
