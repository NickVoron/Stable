cbuffer cbPerObject : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 cameraPos;
}


struct VS_INPUT
{
	float3 Pos		: POSITION;
};

struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
};


VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;

	float3 P = mul(In.Pos-cameraPos.xyz, (float4x3)mul(World, View));
	Out.Pos  = mul(float4(P, 1), Projection);

	return Out;
}

float4 PS(VS_OUTPUT In ) : SV_TARGET0
{
	return float4(1, 1, 0, 1);
}
