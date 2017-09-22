#define D3D9_RASTER_OFFSET

cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 ScreenTexelUV;
};

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 color;
};

Texture2D    textureMap		: register(t5);
SamplerState textureSampler	: register(s5);

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowGeomInput
{
	float3 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct ShadowGeomOutput
{
	float4 Position    : SV_POSITION;
	float2 TextureUV   : TEXCOORD0;
	float Norm_Depth   : TEXCOORD1;
};

ShadowGeomOutput VS(ShadowGeomInput In)
{
	ShadowGeomOutput Out;
	
	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float4 eyeSpacePos = mul(float4(In.Position, 1), WorldViewTM);
	
	Out.Position = mul(eyeSpacePos, ProjTM);
#ifdef D3D9_RASTER_OFFSET
	Out.Position.x -= ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += ScreenTexelUV.y * Out.Position.w;
#endif
	Out.TextureUV = In.TextureUV;
	
	Out.Norm_Depth     = eyeSpacePos.z;
	return Out;
}

void PS(ShadowGeomOutput In, out float4 outColor : SV_TARGET0)
{

	//outColor = float4(1.0f, 0, 0, 0);
	outColor = float4(In.Norm_Depth, 0, 0, 1);

	float2 uv = In.TextureUV;
	float4 color = textureMap.Sample(textureSampler, uv);
	clip( color.a <  0.9f ? -1:1 );

}