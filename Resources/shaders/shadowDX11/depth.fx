cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 cameraPos;
};

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 color;
};


Texture2D    textureMap		: register(t5);
SamplerState textureSampler	: register(s5);


struct DepthGeomInput
{
	float3 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct DepthGeomOutput
{
	float4 Position         : SV_POSITION;
	float2 TextureUV        : TEXCOORD0;
	float4 Norm_Depth       : TEXCOORD1;
};

DepthGeomOutput VS(DepthGeomInput In)
{
	DepthGeomOutput Out;
	
	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float4 posEyeSpace = mul(float4(In.Position, 1), WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)WorldViewTM));
	
	Out.Position = mul(posEyeSpace, ProjTM);
	Out.TextureUV        = In.TextureUV;
	Out.Norm_Depth.xyz   = normEyeSpace;
	Out.Norm_Depth.w	 = posEyeSpace.z;
	
	return Out;
}

void PS(DepthGeomOutput In, out float4 outDepth : SV_TARGET0)
{
	float2 uv = In.TextureUV;
	float4 color = textureMap.Sample(textureSampler, uv);
	
	clip( color.a < 0.9f ? -1:1 );


	outDepth = float4(In.Norm_Depth.w, 0, 0, 1);

	//outDepth = float4(1, 0, 0, 1);
	//outDepth = 1;
}