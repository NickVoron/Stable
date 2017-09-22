cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 cameraPos;
};

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 colorOBJ;
};


struct VS_INPUT
{
	float3 pos		: POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

struct VS_OUTPUT
{
	float4 pos			: SV_POSITION;
	float4 Norm_Depth	: TEXCOORD1;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float3 P = mul(float4(input.pos, 1), (float4x3)WorldViewTM);
	output.pos = mul(float4(P, 1), ProjTM);

	//
	float4 posEyeSpace = mul(float4(input.pos, 1), ViewTM);
	output.Norm_Depth.w = posEyeSpace.z;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	//return 1;
	return float4(input.Norm_Depth.w, 0, 0, 1);

}
