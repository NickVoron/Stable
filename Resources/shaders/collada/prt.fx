float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

float3 sunDir;
float4 getSunColor()	{	return float4(1.0f, 0.8f, 0.7f, 1.0f);	}
//float4 getSunColor()	{	return float4(1.0f, 1.0f, 1.0f, 1.0f);	}
float4 getAmbientColor(){	return float4(0.6f, 0.6f, 1.0f, 1.0f);	}

float4 SHDir1, SHDir2, SHAmb1, SHAmb2;
float2 SHDirAmb;

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
int baseBlendMode = 2;

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



// struct VS_INPUT
// {
//     float3 Pos		: POSITION;
// 	float3 Norm		: NORMAL;
// 	float4 SHDir1	: TEXCOORD0;
// 	float4 SHDir2	: TEXCOORD1;
// 	float4 SHAmb1	: TEXCOORD2;
// 	float4 SHAmb2	: TEXCOORD3;
// 	float2 SHDirAmb : TEXCOORD4;
// };

struct VS_INPUT
{
	float3 Pos		: POSITION;
	float3 Norm		: NORMAL;
	float3 Tangent	: TANGENT;
	float2 UV1		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
	float4 SHDir1	: TEXCOORD2;
	float4 SHDir2	: TEXCOORD3;
	float4 SHAmb1	: TEXCOORD4;
	float4 SHAmb2	: TEXCOORD5;
	float2 SHDirAmb : TEXCOORD6;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
	float3 Norm		: TEXCOORD0;
	float2 UV1		: TEXCOORD1;
	float2 UV2		: TEXCOORD2;
	float3 Color	: TEXCOORD3;
};


// VS_OUTPUT VS( VS_INPUT In )
// {
//     VS_OUTPUT Out;
// 	float4x4 WorldView = mul(World, View);
//     float4 P = mul(float4(In.Pos, 1), WorldView);
//     Out.Pos  = mul(P, Projection);
// 	float3 N = mul(In.Norm, (float3x3)World);
// 
// 	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;
// 	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
// 	float amb1 = dot(In.SHDir1, SHAmb1) + dot(In.SHDir1, SHAmb2) + In.SHDirAmb.x*SHDirAmb.y;
// 	Out.Color.r = dir;
// 	Out.Color.g = amb1;
// 	Out.Color.b = amb;
// 	Out.Norm = N;
// 
//     return Out;
// }

float4 diffColor(float2 uv1, float2 uv2)
{
	float4 c1 = useBase1Tex ? tex2D(base1Sam, uv1) : constBaseColor1;
	float4 c2 = useBase2Tex ? tex2D(base2Sam, uv2) : constBaseColor2;

	if(baseBlendMode == 1) return c1 * c2;
	if(baseBlendMode == 2) return c1 + c2;
	return 0;
}

float4 specularColor(float2 uv)	{ return useSpecular ? tex2D(specularSam, uv) : 0;}
float4 emissionColor(float2 uv) { return useEmission ? tex2D(emissionSam, uv) : 0;}

VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;
	float4x4 WorldView = mul(World, View);
	float4 P = mul(float4(In.Pos, 1), WorldView);
	Out.Pos  = mul(P, Projection);
	float3 N = mul(In.Norm, (float3x3)World);
	Out.UV1  = float2(In.UV1.x, -In.UV1.y);
	Out.UV2  = float2(In.UV2.x, -In.UV2.y);
	Out.Norm = N;

 	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;
 	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
 	float amb1 = dot(In.SHDir1, SHAmb1) + dot(In.SHDir1, SHAmb2) + In.SHDirAmb.x*SHDirAmb.y;
 	Out.Color.r = dir;
 	Out.Color.g = amb1;
 	Out.Color.b = amb;

	return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	//return 1;
	//return float4(In.Norm, 1);

//	float light = saturate( dot(normalize(In.Norm), normalize(sunDir) ) );// + float4(0.3f, 0.3f, 0.3f, 1.0f);
//	return light * diffColor(In.UV1, In.UV2);

	float4 dir = (In.Color.r + In.Color.b) * getSunColor();
	float4 amb = (In.Color.b + In.Color.g) * getAmbientColor();
	//float4 res = 0.7f *dir + 0.3f * amb;
	float4 res = dir + amb;
	//return dir;
	//return amb;
	//return tex2D(base1Sam, In.UV1); 
	return res;
	//float4 diffColor = 1;

	//return res * tex2D(base1Sam, In.UV1); 
	//return (0.7 * (In.Color.r + In.Color.b) * getSunColor() + 0.3f * (In.Color.g + In.Color.b) * getAmbientColor());
	//return 0.7f*(In.Color.r + In.Color.g) * getSunColor() + 0.3f * (In.Color.r + In.Color.g) * getAmbientColor();
	//return float4(In.Color, 1.0f);
	//return float4(0.3, 1, 0.3, 1);

	//return float4(In.UV1, 0, 1);
	//return float4(normalize(In.Norm), 1);
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
