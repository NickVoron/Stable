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
	float3 Norm : TEXCOORD1;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);
	float4 P = mul(float4(In.Pos, 1), WorldView);
	float3 N = mul(In.Normal, (float3x3)World);
	Out.Pos  = mul(P, Projection);
	Out.Light = saturate( dot(N, normalize(lightDir)) );
	Out.Norm = N;

    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3 res = color.xyz;
	res *= In.Light;
    return float4(abs(In.Norm), 1);
}

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		FillMode = wireframe;
		ZEnable = true;
    }
}