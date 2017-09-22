cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 


Texture2D    textureMap		: register( t0 );
SamplerState textureSampler	: register( s0 );


struct FONT_VS_INPUT
{
    float2 pos		: POSITION;
	float4 rect		: TEXCOORD0;
	float4 texRect	: TEXCOORD1;
	float4 color	: COLOR0;
};

struct FONT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 tex	: TEXCOORD0;
	float4 color: TEXCOORD1;
};

FONT_VS_OUTPUT VS(FONT_VS_INPUT input)
{
    FONT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1.0f);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.tex = input.pos.xy * input.texRect.zw;
	output.tex += input.texRect.xy;// -float2(0.5f, 0.5f) / 256;;
	output.color = input.color;
    return output;
}

float4 PS(FONT_VS_OUTPUT input) : SV_TARGET0
{
	float4 color = textureMap.SampleLevel(textureSampler, input.tex, 0);
	color.a = color.r;
	return color.r * input.color;

	/*
	float3 color = float3(0.0, 0.0, 0.0);
	float dist = textureMap.Sample(textureSampler, input.tex).r;
	float width = fwidth(dist);
	float alpha =1.0- smoothstep(0.5 - width, 0.5 + width, dist);
	return float4(color, alpha);	   */
}