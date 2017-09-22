cbuffer PerCamera : register(b0)
{
	float4x4 ViewProjTM;
	float4 radius;
}

TextureCube  textureMap : register(t0);
SamplerState textureSampler	: register(s0);

struct VS_INPUT
{							
	float3 pos	: POSITION;
	float2 tex0 : TEXCOORD0;						 
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float3 tex0 : TEXCOORD0;
};


VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos*radius.x, 1), (float4x4)ViewProjTM);
	output.tex0 = -input.pos;


	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	
	float4 color = textureMap.Sample(textureSampler, input.tex0);
	return color;

	//return float4(input.tex0.x, input.tex0.y, 0,1);
}
