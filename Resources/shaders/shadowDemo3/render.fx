#include "../material/common.fx"
#include "../common/downsampling.fx"

#define D3D9_RASTER_OFFSET

int ObjType = 1;

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
float3 dirLightDirection;
float2 ScreenTexelUV;

texture ScreenShadowMap;
sampler sampScreenShadow = sampler_state
{
	texture = <ScreenShadowMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

// pointLights
float3 lightPos0;
float3 lightPos1;
float3 lightPos2;
float3 lightPos3;

int pointShadowMapsCount;

texture pointShadowMap0;
texture pointShadowMap1;
texture pointShadowMap2;
texture pointShadowMap3;

samplerCUBE pointShadowMapSam0 = sampler_state { Texture = <pointShadowMap0>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};
samplerCUBE pointShadowMapSam1 = sampler_state { Texture = <pointShadowMap1>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};
samplerCUBE pointShadowMapSam2 = sampler_state { Texture = <pointShadowMap2>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};
samplerCUBE pointShadowMapSam3 = sampler_state { Texture = <pointShadowMap3>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct DepthGeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float4 TextureUV   : TEXCOORD0;
};

struct DepthGeomOutput
{
	float4 Position         : POSITION0;
	float4 TextureUV        : TEXCOORD0;
	float4 Norm_Depth       : TEXCOORD1;
};

DepthGeomOutput DepthGeomVS(DepthGeomInput In)
{
	DepthGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)WorldViewTM));
	
	Out.Position         = mul(posEyeSpace, Proj);
	Out.TextureUV        = In.TextureUV;
	Out.Norm_Depth.xyz   = normEyeSpace;
	Out.Norm_Depth.w     = posEyeSpace.z;
	
	return Out;
}

void DepthGeomPS(DepthGeomOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.Norm_Depth.w, 0, 0, 0);
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct ShadowGeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float4 TextureUV   : TEXCOORD0;
};

struct ShadowGeomOutput
{
	float4 Position    : POSITION0;
	float4 TextureUV   : TEXCOORD0;
};

ShadowGeomOutput ShadowGeomVS(ShadowGeomInput In)
{
	ShadowGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);
	
	float4 eyeSpacePos = mul(In.Position, WorldViewTM);
	
	Out.Position = mul(eyeSpacePos, Proj);
#ifdef D3D9_RASTER_OFFSET
	Out.Position.x -= ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += ScreenTexelUV.y * Out.Position.w;
#endif
	Out.TextureUV = In.TextureUV;
	
	return Out;
}

void ShadowGeomPS(ShadowGeomOutput In, out float4 outColor : COLOR) 
{
	outColor = 0;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct MainGeomInput
{
	float4 Position		: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float4 UV1			: TEXCOORD0;
	float2 UV2			: TEXCOORD1;
	float4 SHDir1		: TEXCOORD2;
	float4 SHDir2		: TEXCOORD3;
	float4 SHAmb1		: TEXCOORD4;
	float4 SHAmb2		: TEXCOORD5;
	float2 SHDirAmb 	: TEXCOORD6;
};

struct MainGeomOutput
{
	float4 Position         : POSITION;
	float4 UV1				: TEXCOORD0;
	float2 UV2				: TEXCOORD1;
	float3 Normal			: TEXCOORD2;
	float3 Tangent			: TEXCOORD3;
	float3 Binormal			: TEXCOORD4;
	float4 ScreenProjCoords : TEXCOORD5;
	float3 PRT				: TEXCOORD6;
	float3 worldPos 		: TEXCOORD7;
};

MainGeomOutput MainGeomVS(MainGeomInput In)
{
	MainGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	
	Out.worldPos		= mul(In.Position, World).xyz;
	Out.Position		= mul(posEyeSpace, Proj);
	Out.UV1  = float4(In.UV1.x, -In.UV1.y, In.UV1.z, -In.UV1.w);
	Out.UV2  = float2(In.UV2.x, -In.UV2.y);

	Out.Normal	= mul(In.Normal,(float3x3)World);
	Out.Tangent = mul(In.Tangent,(float3x3)World);
	Out.Binormal = cross(In.Tangent, In.Normal);
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;

 	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
 	float amb1 = dot(In.SHDir1, SHAmb1) + dot(In.SHDir1, SHAmb2) + In.SHDirAmb.x*SHDirAmb.y;
 	Out.PRT.r = dir;
 	Out.PRT.g = amb1;
 	Out.PRT.b = amb;
	
	return Out;
}

float3 computePointLight(float3 lightPos, float3 lightColor, float3 worldPos, float3 normalWS, samplerCUBE smpl)
{
	float3 lightDir = lightPos - worldPos;
 	float lightDirLen = length(lightDir);
 	float invLightDist = 1.0f / lightDirLen;
 	float3 ld = lightDir * invLightDist;
 	float light = saturate( dot( ld, normalWS ) )* invLightDist;
  	if( texCUBE(smpl, -normalize(lightDir) ).r + 0.1 < lightDirLen ) 
  	{
  		light = 0.0f;
  	}

	return light * lightColor;
}

void MainGeomPS(MainGeomOutput In, out float4 outColor : COLOR)
{
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	//float3 dotNL = saturate(dot(normalize(In.Tangent), dirLightDirection));
	float3 N = normal(In.UV1.xy, In.Tangent, In.Binormal, In.Normal);
	float3 dotNL = saturate(dot(N, dirLightDirection));

	dotNL *= getSunColor();

	float3 dir = (In.PRT.r) * getSunColor();
	float3 amb = (In.PRT.b) * getAmbientColor();
// 	dir = (In.PRT.r + In.PRT.b);
// 	amb = (In.PRT.b + In.PRT.g);

	//float4 res = 0.7f *dir + 0.3f * amb;
	float3 diffuse = getAmbientColor()*0.3;
	diffuse = dotNL * shadow + 0.3;//  + dir + amb;
	//diffuse =  (dotNL * shadow) * getSunColor() + getAmbientColor();
	//diffuse =  (dotNL * shadow) + 0.3f;
	//diffuse =   dir*20 +  amb*10;

 	//if(pointShadowMapsCount > 0) diffuse.rgb += computePointLight(lightPos0, float3(1, 0.6, 0.5)*2, In.worldPos, N, pointShadowMapSam0);
  	//if(pointShadowMapsCount > 1) diffuse.rgb += computePointLight(lightPos1, float3(0, 1, 0)*2, In.worldPos, N, pointShadowMapSam1);

	float4 diff = diffColor(In.UV1);
	outColor.rgb = diffuse * diff.rgb;
	//outColor.rgb = diffuse;
	outColor.a = diff.a;

	//outColor.rgb = N;
	





	//light = lightDirLen;
	//light = texCUBE(pointShadowMapSam, -normalize(lightDir) ).r;
	//outColor = lightDirLen/10;

//	outColor.rgb = res * diff.rgb;
}

struct PointLightGeomInput
{
	float4 Position    : POSITION;
	float2 TextureUV   : TEXCOORD0;
};

struct PointLightGeomOutput
{
	float4 Position         : POSITION0;
	float3 TextureUV_Depth  : TEXCOORD0;
	float3 worldPos			: TEXCOORD2;
};

PointLightGeomOutput PointLightGeomVS(PointLightGeomInput In)
{
	PointLightGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);

	Out.worldPos		 = posEyeSpace.xyz;
	Out.Position         = mul(posEyeSpace, Proj);
	Out.TextureUV_Depth.xy   = In.TextureUV;
	Out.TextureUV_Depth.z    = posEyeSpace.z;
	
	return Out;
}

void PointLightGeomPS(PointLightGeomOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(length(In.worldPos), 0, 0, 0);
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
technique DepthGeom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 DepthGeomVS();
		PixelShader      = compile ps_2_0 DepthGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}

technique ShadowGeom
{
	pass p0
	{
		VertexShader = compile vs_2_0 ShadowGeomVS();
		PixelShader  = compile ps_2_0 ShadowGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.0; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.000001;
	}
}

technique MainGeom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 MainGeomVS();
		PixelShader      = compile ps_2_0 MainGeomPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
		AlphaRef = 100;
		AlphaFunc = Greater;
	}
}

technique PointLightGeom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 PointLightGeomVS();
		PixelShader      = compile ps_2_0 PointLightGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}
