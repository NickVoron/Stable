cbuffer cbPerObject : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}


int lightEnable = 0;
float3 currentColor	= 1;

float3 lightDir	= 1;

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal : NORMAL;
	//float4 Diffuse : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	//float3 Light : TEXCOORD0;
	//float4 Diffuse : TEXCOORD1;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);
	//float4x4 WorldView = colorFromVertex[1] == 0 ? mul(World, View) : World;
    float3 P = mul(float4(In.Pos, 1), (float4x3)WorldView);
	float3 N = mul(float4(In.Normal, 1), (float4x3)World);
    Out.Pos  = mul(float4(P, 1), Projection);
	//Out.Light = saturate( dot(N, normalize(lightDir)) );
	//Out.Diffuse = In.Diffuse;
	
	//if(lightEnable == 0) Out.Light = 1;
	
    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3 res = currentColor.xyz;
	//res *= In.Light;// * In.Diffuse;
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
