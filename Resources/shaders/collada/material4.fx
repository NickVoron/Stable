#include "../common/func.fx"
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

float3 cameraPos;
float3 lightDir;
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

int indexBase;
int2 matTexSize;
float2 invMatTexSize;// = float2(1.0f / 256.0f, 1 / 4.0f);
texture matricesTexture;
sampler matricesSam = sampler_state {Texture = <matricesTexture>; AddressU = WRAP; AddressV = WRAP;};

int2 getIndex(int index, int baseIndex)
{
	int2 idx;
	idx.x = (index + baseIndex) * 4;
	idx.y = idx.x / matTexSize.x;
	idx.x -= idx.y * matTexSize.x;
	return idx;
}

float4x4 getMatrix(int index, int baseIndex)
{
	int2 idx = getIndex(index, baseIndex); 
	
	float y = idx.y + 0.5f;

	float4 uv01 = float4(idx.x + 0 + 0.5f, y, idx.x + 1 + 0.5f, y) * invMatTexSize.xyxy;
	float4 uv23 = float4(idx.x + 2 + 0.5f, y, idx.x + 3 + 0.5f, y) * invMatTexSize.xyxy;
	
	float4 row0 = tex2Dlod(matricesSam, float4( uv01.xy, 0, 0 ));
	float4 row1 = tex2Dlod(matricesSam, float4( uv01.zw, 0, 0 ));
	float4 row2 = tex2Dlod(matricesSam, float4( uv23.xy, 0, 0 ));
	float4 row3 = tex2Dlod(matricesSam, float4( uv23.zw, 0, 0 ));
	
	return float4x4(row0, row1, row2, row3);
}

struct ANIMATED_VS_INPUT
{
	float3 Pos			: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 UV1			: TEXCOORD0;
	float2 UV2			: TEXCOORD1;
	float4 blendWeight	: BLENDWEIGHT;
	int4 blendIndices	: BLENDINDICES;
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

bool animated;
VS_OUTPUT AnimatedVS( ANIMATED_VS_INPUT In )
{
	VS_OUTPUT Out;

	float4x4 WorldView = mul(World, View);

	float4 pos = float4(In.Pos, 1);


	if(animated)
	{
		int4 indices = In.blendIndices;
		float4 weights = In.blendWeight;

		float4x4 m0, m1, m2, m3;
		m0 = getMatrix(indices[0], indexBase);
		m1 = getMatrix(indices[1], indexBase);
		m2 = getMatrix(indices[2], indexBase);
		m3 = getMatrix(indices[3], indexBase);

		float4 resPos = 0;
		resPos += mul(pos, m0 ) * weights[0];
		resPos += mul(pos, m1 ) * weights[1];
		resPos += mul(pos, m2 ) * weights[2];
		resPos += mul(pos, m3 ) * weights[3];

		float3 resNorm = 0;
		resNorm += mul(In.Normal, (float3x3)m0 ) * weights[0];
		resNorm += mul(In.Normal, (float3x3)m1 ) * weights[1];
		resNorm += mul(In.Normal, (float3x3)m2 ) * weights[2];
		resNorm += mul(In.Normal, (float3x3)m3 ) * weights[3];

		float3 resTangent = 0;
		resTangent += mul(In.Tangent, (float3x3)m0 ) * weights[0];
		resTangent += mul(In.Tangent, (float3x3)m1 ) * weights[1];
		resTangent += mul(In.Tangent, (float3x3)m2 ) * weights[2];
		resTangent += mul(In.Tangent, (float3x3)m3 ) * weights[3];

		float4 P = mul(resPos, WorldView);
		Out.Pos  = mul(P, Projection);
		Out.UV1  = float2(In.UV1.x, -In.UV1.y);
		Out.UV2  = float2(In.UV2.x, -In.UV2.y);
		Out.Normal = resNorm;
		Out.Tangent = resTangent;
		Out.Binormal = normalize( cross(In.Tangent, In.Normal) );
		Out.PosV = mul(resPos, World) - View[3];
	}
	else
	{
		float4 P = mul(pos, WorldView);
		Out.Pos  = mul(P, Projection);
		Out.UV1  = float2(In.UV1.x, -In.UV1.y);
		Out.UV2  = float2(In.UV2.x, -In.UV2.y);
		Out.Normal = mul(In.Normal, (float3x3)World);
		Out.Tangent = mul(In.Tangent,(float3x3)World);
		Out.Binormal = normalize( cross(In.Tangent, In.Normal) );
		Out.PosV = mul(pos, World).xyz - cameraPos;
	}

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
	float spec = lightDirectSpec(-lightDir, N, vecI, 200.0f);
	return spec;
	//return specularColor(In.UV1);
	return (light + spec +0.3f)*diffColor(In.UV1, In.UV2);
}

//#define fm wireframe
#define fm solid

technique Animated
{
    pass P0
    {
		VertexShader = compile vs_3_0 AnimatedVS();
        PixelShader  = compile ps_3_0 PS();
		CullMode = none;
		FillMode = fm;
		ZEnable = true;
    }
}
