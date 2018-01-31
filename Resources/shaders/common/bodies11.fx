cbuffer cbPerCamera : register( b0 )
{
	float4x4 View       : VIEW;
	float4x4 Projection : PROJECTION;
	float4 direction;
	float4 lightDirection;
}

cbuffer cbPerObject : register( b1 )
{
	float4x4 World      : WORLD;
	float4 currentColor;
	int4 colorFromVertex;
}


struct VS_INPUT
{
    float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float4 Color	: COLOR;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
	float4 Color	: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;
	float4x4 mtx = colorFromVertex[1] == 0 ? mul(World, View) : World;
	float3 P = mul(float4(In.Pos, 1), (float4x3)mtx);

	Out.Pos  = mul(float4(P, 1), Projection);
	Out.Color = colorFromVertex[0] ? In.Color : currentColor;
	Out.Normal = mul( In.Normal, (float3x3)World);
	
    return Out;
}

float4 PS(VS_OUTPUT In ) : SV_TARGET0
{
	if (colorFromVertex[2] == 0)
	{
		return float4(In.Color.xyz, 1);
	}
	else if(colorFromVertex[2] == 0x0010)
	{
		float light = saturate(dot(In.Normal, lightDirection)) + 0.3f;
		return float4(In.Color.xyz * light, 1);
	}
	
	return float4(1, 1, 0, 1);
}
