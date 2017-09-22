float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Proj : PROJECTION;

float3 cameraPosition;
float3 sunDir;

float3 sunClr;
float3 ambClr;

float3 getSunColor()	{ return sunClr;	return float3(1.0f, 0.8f, 0.7f);	}
float3 getAmbientColor(){ return ambClr;	return float3(0.3f, 0.3f, 0.5f);	}

float4 constBaseColor1 = float4(1, 0, 0, 1);
float4 constBaseColor2 = float4(0, 1, 0, 1);
float4 constSpecular = float4(0, 0, 1, 1);
float4 constEmission = float4(0, 0, 0, 1);

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

float4 diffColor(float4 uv)
{
	float4 c1 = useBase1Tex ? tex2D(base1Sam, uv.xy) : constBaseColor1;
	float4 c2 = useBase2Tex ? tex2D(base2Sam, uv.zw) : constBaseColor2;
	//return c2;
	if(baseBlendMode == 1) return c1 * c2;
	if(baseBlendMode == 2) return c1 + c2;
	return 0;
}

float4 diffColor1(float2 uv1)
{
	return tex2D(base1Sam, uv1);
}

float3 normal(float2 uv, float3 t, float3 b, float3 n)
{
	if(useNormal1Tex)
	{
		float3 normLocal;
		normLocal = float3(0,1,0);
		normLocal = normalize( (tex2D(norm1Sam, uv).rgb - float3(0.5f, 0.5f, 0.0f)) * float3(2,2,1) ).rbg;
		
		return normalize( normLocal.x*t + normLocal.y*n + normLocal.z*b);
		return normalize( n );
	}

	return n;
}

float4 specularColor(float2 uv)	{ return useSpecular ? tex2D(specularSam, uv) : 0;}
float4 emissionColor(float2 uv) { return useEmission ? tex2D(emissionSam, uv) : constEmission;}
float4 transparentColor(float2 uv) { return useEmission ? tex2D(emissionSam, uv) : constEmission;}