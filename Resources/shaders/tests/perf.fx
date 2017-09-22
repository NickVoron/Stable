float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

float3 color = 1;
float3 lightDir	= 1;

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Light : TEXCOORD0;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(In.Pos, 1), (float4x3)WorldView);
	float3 N = mul(float4(In.Normal, 1), (float4x3)World);
    Out.Pos  = mul(float4(P, 1), Projection);
	Out.Light = saturate( dot(N, normalize(lightDir)) );

    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3 res = color.xyz;
	res *= In.Light;
    return float4(res, 1);
}

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		FillMode = Wireframe;
		ZEnable = true;
    }
}