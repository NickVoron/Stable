#include "../material/common.fx"

float3 lightV3;

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal  : NORMAL;
	float4 UV1 : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float4 UV1 : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float Light : TEXCOORD2;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);
    float3 P = mul(float4(In.Pos, 1), (float4x3)WorldView);
	float3 N = mul(float4(In.Normal, 1), (float4x3)World);
    Out.Pos  = mul(float4(P, 1), Proj);
	Out.UV1 = float4(In.UV1.x, -In.UV1.y, In.UV1.z, -In.UV1.w);
	Out.UV2 = In.UV2;
	Out.Light = saturate( dot(N, normalize(-lightV3)) );
	//Out.Light = N;
	
    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	return In.Light;
}

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		//FillMode = Wireframe;
		ZEnable = true;
    }
}
