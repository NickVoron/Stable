cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 					  

Texture2D    textureMap		: register( t0 );
SamplerState textureSampler	: register( s0 );

struct TEX_RECT_VS_INPUT
{
    float2 pos		: POSITION;
	float4 rect		: TEXCOORD0;
	float4 texRect	: TEXCOORD1;
	float4 color	: COLOR0;			     
};

struct TEX_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 tex	: TEXCOORD0;
	float4 color: COLOR0;
};

TEX_RECT_VS_OUTPUT VS(TEX_RECT_VS_INPUT input)
{
    TEX_RECT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1.0f);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.tex = input.pos.xy * (input.texRect.zw);
	output.tex += input.texRect.xy;

	output.color = input.color;
    return output;
}

float4 PS(TEX_RECT_VS_OUTPUT input) : SV_TARGET0
{
	return textureMap.SampleLevel(textureSampler, input.tex, 0) * input.color;
}
