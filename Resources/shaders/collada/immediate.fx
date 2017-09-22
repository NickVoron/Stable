float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

texture baseColor1;
texture baseColor2;
texture normal1;
texture normal2;
texture specular;
texture emission;

sampler base1Sam = sampler_state {Texture = <baseColor1>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler base2Sam = sampler_state {Texture = <baseColor2>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler norm1Sam = sampler_state {Texture = <normal1>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler norm2Sam = sampler_state {Texture = <normal2>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler specularSam = sampler_state {Texture = <specular>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler emissionSam = sampler_state {Texture = <emission>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};


struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV1 : TEXCOORD0;
	float2 UV2: TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Norm  : TEXCOORD0;
	float2 UV1 : TEXCOORD1;
	float2 UV2 : TEXCOORD2;
};


VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;
	float4x4 WorldView = mul(World, View);
    float4 P = mul(float4(In.Pos, 1), WorldView);
	float3 N = mul(In.Normal, (float3x3)World);
    Out.Pos  = mul(P, Projection);
	Out.UV1  = float2(In.UV1.x, -In.UV1.y);
	Out.UV2  = float2(In.UV2.x, -In.UV2.y);
	Out.Norm = N;
    return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	//return float4(0.3, 1, 0.3, 1);
	float light = saturate( dot(normalize(In.Norm), normalize(float3(1, 1, 1)) ) ) + float4(0.3f, 0.3f, 0.3f, 1.0f);
	//return light * tex2D(base1Sam, In.UV1);// * tex2D(base2Sam, In.UV2);
	//return light;
	//return float4(In.UV1, 0, 1);
	return float4(In.Norm, 1);
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
