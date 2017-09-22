#include "../common/func.fx"
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Proj		: PROJECTION;

float3 cameraPos;
float3 lightDir = float3(1,1,1);
float4 getSunColor()	{	return float4(1.0f, 0.8f, 0.7f, 1.0f);	}
float4 getAmbientColor(){	return float4(0.6f, 0.6f, 1.0f, 1.0f);	}


float4 constBaseColor1 = float4(1, 0, 0, 1);
float4 constBaseColor2 = float4(0, 1, 0, 1);
float4 constSpecular = float4(0, 0, 1, 1);
float4 constEmission = float4(0, 1, 1, 1);

bool useBase1Tex = false;
bool useBase2Tex = false;
bool useSpecular = false;
bool useEmission = false;

bool useNormal1Tex;
bool useNormal2Tex;


// 1 - mul, 2 - add signed
int baseBlendMode = 1;

texture baseColor1;
texture baseColor2;
texture normal1;
texture normal2;
texture specular;
texture emission;

sampler base1Sam	= sampler_state {Texture = <baseColor1>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler base2Sam	= sampler_state {Texture = <baseColor2>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler norm1Sam	= sampler_state {Texture = <normal1>;		MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler norm2Sam	= sampler_state {Texture = <normal2>;		MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler specularSam = sampler_state {Texture = <specular>;		MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler emissionSam = sampler_state {Texture = <emission>;		MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

struct VS_INPUT
{
	float3 Pos			: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 UV1			: TEXCOORD0;
	float2 UV2			: TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float3 Binormal	: BINORMAL;
	float2 UV1		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
	float3 PosV		: TEXCOORD2;
};

float4 diffColor(float2 uv1, float2 uv2)
{
	//return tex2D(base1Sam, uv1) * tex2D(base2Sam, uv1);
	float4 c1 = useBase1Tex ? tex2D(base1Sam, uv1) : constBaseColor1;
	float4 c2 = useBase2Tex ? tex2D(base2Sam, uv2) : constBaseColor2;

	if(baseBlendMode == 1) return c1 * c2;
	if(baseBlendMode == 2) return c1 + c2;
	return 0;
}

float3 normal(float2 uv, float3 t, float3 b, float3 n)
{
	//float3 normLocal = float3(0,1,0);
	float3 normLocal = normalize( (tex2D(norm1Sam, uv).rgb - float3(0.5f, 0.5f, 0.0f)) * float3(2,2,1) ).rbg;
	float3 norm = normalize( normLocal.x*t + normLocal.y*n + normLocal.z*b);
	return norm;
	return normalize( n );
}

float4 specularColor(float2 uv)	{ return useSpecular ? tex2D(specularSam, uv) : 0;}
float4 emissionColor(float2 uv) { return useEmission ? tex2D(emissionSam, uv) : 0;}

VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);

	float4 pos = float4(In.Pos, 1);
	float4 P = mul(pos, WorldView);
	Out.Pos  = mul(P, Proj);
	Out.UV1  = float2(In.UV1.x, -In.UV1.y);
	Out.UV2  = float2(In.UV2.x, -In.UV2.y);
	Out.Normal = mul(In.Normal, (float3x3)World);
	Out.Tangent = mul(In.Tangent,(float3x3)World);
	Out.Binormal = normalize( cross(In.Tangent, In.Normal) );
	Out.PosV = mul(pos, World).xyz - cameraPos;

	return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3x3 bnmat = float3x3(In.Tangent, In.Binormal, In.Normal);
	float3 N = normal(In.UV1, In.Tangent, In.Binormal, In.Normal);
	float3 vecI = normalize( In.PosV );
	
	//N = normalize(In.Normal);
	float light = saturate( dot(N, -lightDir) );
	//return light;
	//return 1;
	//return In.Color;
	//return float4(N, 1);
	//return specular + light;
	//return float4(normal())
	//return spec;
	//return specularColor(In.UV1);
	return (light + 0.3f)*diffColor(In.UV1, In.UV2);
}

//#define fm wireframe
#define fm solid

technique Animated
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
