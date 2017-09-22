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

cbuffer FrameParams: register(b2)
{
	float2 minCorner;
	float2 size;
	float3 addColor;
};


Texture2D    tex2D			: register(t3);
SamplerState textureSampler	: register(s3);

struct SOLID_RECT_VS_INPUT
{
    float3 pos  : POSITION;
	float2 uv	: TEXCOORD0;
};

struct SOLID_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 uv: TEXCOORD0;
};

SOLID_RECT_VS_OUTPUT VS(SOLID_RECT_VS_INPUT input)
{
	float4x4 ModelView = mul(WorldTM, ViewTM);
	float4x4 ModelViewProj = mul(ModelView, ProjTM);

    SOLID_RECT_VS_OUTPUT output;	 
	output.pos = mul(float4(input.pos, 1), ModelViewProj);

	output.uv = input.uv * size;
	output.uv += minCorner;

    return output;
}

float4 PS(SOLID_RECT_VS_OUTPUT input) : SV_TARGET0
{	
	float4 c0 = tex2D.Sample(textureSampler, input.uv);
	c0.xyz *= colorOBJ.x;
	return c0;
}