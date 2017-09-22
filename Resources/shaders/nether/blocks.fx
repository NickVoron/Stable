float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

int lightEnable = 0;
int textureEnable = 1;
int highlighted = 0;
int lightFromCamera = true;
float3 currentColor	= 1;

float3 lightDir;
float3 cameraDir;
float3 lightColor;



texture MaterialTex;
sampler Mat = sampler_state
{
	texture = <MaterialTex>;

	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Light : TEXCOORD0;
	float2 UV : TEXCOORD1;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(In.Pos, 1), (float4x3)WorldView);
	float3 N = mul(In.Normal, (float3x3)World);
    Out.Pos  = mul(float4(P, 1), Projection);
	float3 ld = lightFromCamera ? cameraDir : lightDir;
	Out.Light = saturate( dot(normalize(N), normalize(ld)))+0.3;
	Out.UV = In.UV;
	
    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3 res = 0.0f;

	if(textureEnable)
	{
		res = tex2D(Mat, In.UV).xyz * currentColor;
	}
	else
	{
		res = currentColor;
	}

	res *= In.Light;

    return float4(res, 1);
}

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = CCW;
		//CullMode = none;
		//FillMode = Wireframe;
		ZEnable = true;
    }
}
