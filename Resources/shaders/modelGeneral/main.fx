cbuffer cbPerCamera : register( b0 )
{
	float4x4 View       : VIEW;
	float4x4 Projection : PROJECTION;
	float4 direction;
	float4 light;
}

cbuffer cbPerObject : register( b1 )
{
	float4x4 World;
	float4 currentColor;
	int4 colorFromVertex;
}

Texture2D    textureMap		: register(t0);
SamplerState textureSampler	: register(s0);


struct VS_INPUT
{
    float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD0;
	float4 Color	: COLOR;
};

struct VS_OUTPUT
{
    float4 Pos		: SV_POSITION;
	float4 Color	: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
	float2 UV		: TEXCOORD3;
	float4 Flags	: TEXCOORD4;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float3 P = mul(float4(In.Pos, 1), (float4x3)mul(World, View));
	Out.Pos  = mul(float4(P, 1), Projection);
	Out.Color = colorFromVertex[0] ? In.Color : currentColor;
	Out.Normal = In.Normal;
	Out.Flags.x = colorFromVertex[1] ? 1.0f : 0.0f;
	//Out.UV = float2(fmod(In.UV.x, 1.0f), fmod(In.UV.y, 1.0f));
	Out.UV = In.UV;
	Out.Color = float4(Out.Normal, 1);
	
    return Out;
}

float4 PS(VS_OUTPUT In ) : SV_TARGET0
{
	float2 uv = In.UV;// float2(fmod(In.UV.x, 1.0f), fmod(In.UV.x, 1.0f));
	float3 color = lerp(float3(1, 1, 1), textureMap.Sample(textureSampler, uv).xyz, In.Flags.x);
	float3 n = normalize(In.Normal);

	float3 res = clamp(dot(n, light), 0.0f, 1.0f) + 0.3;
	//res = n;
	//return float4(color, 1);
	return float4(res*color, 1);
	return float4(uv, 0, 1);
}
