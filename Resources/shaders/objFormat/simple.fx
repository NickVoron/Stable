float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

texture tex0;
sampler tex = sampler_state { Texture = <tex0>; MipFilter = LINEAR; MinFilter = LINEAR; MagFilter = LINEAR; };

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Norm  : TEXCOORD0;
	float2 UV : TEXCOORD1;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;
	float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(In.Pos, 1), (float4x3)WorldView);
	float3 N = mul(In.Normal, (float3x3)World);
    Out.Pos  = mul(float4(P, 1), Projection);
	Out.UV  = In.UV;
	Out.Norm = In.Normal;
    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	//return float4(In.UV, 0, 1);
	return saturate( dot(normalize(In.Norm), normalize(float3(1, 1, 1)) ) ) + float4(0.3f, 0.3f, 0.3f, 1.0f);
   // return tex2D(tex, In.UV);
}

//#define fm wireframe
#define fm solid

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 PS();
		CullMode = none;
		FillMode = fm;
		ZEnable = true;
    }
}
