cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 

struct SOLID_RECT_VS_INPUT
{
    float2 pos  : POSITION;
	float4 rect	: TEXCOORD0;
	float4 color: COLOR0;
};

struct SOLID_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float4 color: TEXCOORD0;
};

SOLID_RECT_VS_OUTPUT VS(SOLID_RECT_VS_INPUT input)
{
    SOLID_RECT_VS_OUTPUT output;	 
	float3 resPos = float3(input.pos, 1.0);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.color = input.color;
    return output;
}

float4 PS(SOLID_RECT_VS_OUTPUT input) : SV_TARGET0
{
//	return 1;
	return input.color;
}