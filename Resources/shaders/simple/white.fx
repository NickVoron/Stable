cbuffer cbPerCamera : register( b0 )
{
	float4x4 viewProjection;
}

cbuffer cbPerObject : register( b1 )
{
	float4x4 world;
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

	float4x4 mtx = mul(world, viewProjection);
	Out.Pos = mul(float4(In.Pos, 1), (float4x4)mtx);

    return Out;
}

float4 PS(VS_OUTPUT In ) : SV_TARGET0
{
	return 1;
}
