#include "../material/common.fx"

texture lightMapTex0;
texture lightMapTex1;
texture lightMapTex2;
texture lightMapTex3;
texture lightMapTex4;
texture lightMapTex5;
texture lightMapTex6;
texture lightMapTex7;
texture lightMapTex8;
sampler lightMap0 = sampler_state {Texture = <lightMapTex0>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap1 = sampler_state {Texture = <lightMapTex1>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap2 = sampler_state {Texture = <lightMapTex2>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap3 = sampler_state {Texture = <lightMapTex3>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap4 = sampler_state {Texture = <lightMapTex4>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap5 = sampler_state {Texture = <lightMapTex5>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap6 = sampler_state {Texture = <lightMapTex6>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap7 = sampler_state {Texture = <lightMapTex7>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler lightMap8 = sampler_state {Texture = <lightMapTex8>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

float4 lightSH0;
float4 lightSH1;
float4 lightSH2;
float4 lightSH3;
float4 lightSH4;
float4 lightSH5;
float4 lightSH6;
float4 lightSH7;
float4 lightSH8;

float3 lightV3;

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal  : NORMAL;
	float4 UV1 : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float2 UV3 : TEXCOORD2;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float4 UV1 : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float2 UV3 : TEXCOORD2;
	float Light : TEXCOORD3;
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
	Out.UV3 = In.UV3;
	Out.Light = saturate( dot(N, normalize(-lightV3)) );
	//Out.Light = N;
	
    return Out;
}

float SH4(float2 uv)
{
	return dot(tex2D(lightMap0, uv), lightSH0);
}

float SH9(float2 uv)
{
	float4 sh0 = tex2D(lightMap0, uv);
	float4 sh1 = tex2D(lightMap1, uv);
	float4 sh2 = tex2D(lightMap2, uv);
	return	dot(sh0, lightSH0) + dot(sh1, lightSH1) + sh2.x * lightSH2.x;
}

float SH16(float2 uv)
{
	float4 sh0 = tex2D(lightMap0, uv);
	float4 sh1 = tex2D(lightMap1, uv);
	float4 sh2 = tex2D(lightMap2, uv);
	float4 sh3 = tex2D(lightMap3, uv);
	return	dot(sh0, lightSH0) + dot(sh1, lightSH1) + dot(sh2, lightSH2) + dot(sh3, lightSH3); 
}

float SH25(float2 uv)
{
	float4 sh0 = tex2D(lightMap0, uv);
	float4 sh1 = tex2D(lightMap1, uv);
	float4 sh2 = tex2D(lightMap2, uv);
	float4 sh3 = tex2D(lightMap3, uv);
	float4 sh4 = tex2D(lightMap4, uv);
	float4 sh5 = tex2D(lightMap5, uv);
	float4 sh6 = tex2D(lightMap6, uv);
	return dot(sh0, lightSH0) + dot(sh1, lightSH1) + dot(sh2, lightSH2) + dot(sh3, lightSH3) + dot(sh4, lightSH4) + dot(sh5, lightSH5) + sh6.x * lightSH6.x; 
}

float SH36(float2 uv)
{
	float4 sh0 = tex2D(lightMap0, uv);
	float4 sh1 = tex2D(lightMap1, uv);
	float4 sh2 = tex2D(lightMap2, uv);
	float4 sh3 = tex2D(lightMap3, uv);
	float4 sh4 = tex2D(lightMap4, uv);
	float4 sh5 = tex2D(lightMap5, uv);
	float4 sh6 = tex2D(lightMap6, uv);
	float4 sh7 = tex2D(lightMap7, uv);
	float4 sh8 = tex2D(lightMap8, uv);
	return dot(sh0, lightSH0) + dot(sh1, lightSH1) + dot(sh2, lightSH2) + dot(sh3, lightSH3) +	dot(sh4, lightSH4) + dot(sh5, lightSH5) + dot(sh6, lightSH6) + dot(sh7, lightSH7) +	dot(sh8, lightSH8); 
}

float4 RGBA32F(float2 uv)
{
	return tex2D(lightMap0, uv);
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	return RGBA32F(In.UV2);// * diffColor(In.UV1);// + emissionColor(In.UV1.xy);
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
