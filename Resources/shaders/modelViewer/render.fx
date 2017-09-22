#include "../material/common.fx"
#include "../material/parallax.fx"
#include "../common/func.fx"
//#include "../common/downsampling.fx"
#include "../common/hdr.fx"


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

texture lightMapTex0;
sampler lightMap0 = sampler_state {Texture = <lightMapTex0>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};


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


bool reflection;
bool refraction;

texture reflectionCube;
samplerCUBE reflectionCubeSam = sampler_state { Texture = <reflectionCube>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct DepthGeomInput
{
	float4 Position    : POSITION;
 	float3 Normal      : NORMAL;
// 	float3 Tangent     : TANGENT;
	float4 TextureUV   : TEXCOORD0;
};

struct DepthGeomOutput
{
	float4 Position         : POSITION0;
	float4 TextureUV        : TEXCOORD0;
	float4 Norm_Depth       : TEXCOORD1;
// 	float3 vViewWS			: TEXCOORD2;
// 	float3 Normal			: NORMAL;
// 	float3 Tangent			: TANGENT;
// 	float3 Binormal			: BINORMAL;
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
// 	Out.Normal	= mul(In.Normal,(float3x3)World);
// 	Out.Tangent = mul(In.Tangent,(float3x3)World);
// 	Out.Binormal = cross(In.Tangent, In.Normal);
// 	Out.vViewWS = cameraPosition - In.Position;

	return Out;
}

void DepthGeomPS(DepthGeomOutput In, out float4 outDepth : COLOR)
{
// 	float3x3 bnmat = float3x3(In.Tangent, In.Normal, In.Binormal);
// 	float3 vLightTS = mul( bnmat, -dirLightDirection);
// 	float3 vViewTS  = mul( bnmat, In.vViewWS );
// 	float fParallaxLength = sqrt( dot( vViewTS, vViewTS ) - vViewTS.y * vViewTS.y ) / vViewTS.y;
// 	float2 vParallaxOffsetTS = normalize( vViewTS.xz ) * fParallaxLength * fHeightMapRange;
// 	float3 parallax = Parallax(In.TextureUV, vParallaxOffsetTS, vViewTS, vLightTS, normalize(In.Normal));
// #ifdef PARALLAX
// 	parallax = float3(0,0,1);
// #endif
// 	float2 vParallaxOffset = parallax.xy;
// 
// 	float h = tex2D(norm1Sam, In.TextureUV - vParallaxOffset).a * fHeightMapRange;

	float4 diffTex = diffColor(In.TextureUV);
	outDepth = float4(In.Norm_Depth.w, 0, 0, diffTex.a);
	outDepth.a = 1;
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
	outColor = diffColor(In.TextureUV);
	outColor.a = 1;
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
};

struct MainGeomOutput
{
	float4 Position         : POSITION;
	float3 Normal			: NORMAL;
	float3 Tangent			: TANGENT;
	float3 Binormal			: BINORMAL;
	float4 UV1				: TEXCOORD0;
	float2 UV2				: TEXCOORD1;
	float4 ScreenProjCoords : TEXCOORD2;
	float3 worldPos 		: TEXCOORD3;
	float3 PosV		 		: TEXCOORD4;
	float3 vViewWS	 		: TEXCOORD5;
};

MainGeomOutput MainGeomVS(MainGeomInput In)
{
	MainGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	
	Out.worldPos = mul(In.Position, World).xyz;
	Out.Position = mul(posEyeSpace, Proj);
	Out.UV1  	 = In.UV1;
	Out.UV2  	 = In.UV2;

	Out.Normal	= mul(In.Normal,(float3x3)World);
	Out.Tangent = mul(In.Tangent,(float3x3)World);
	Out.Binormal = cross(In.Tangent, In.Normal);
	Out.PosV = mul(In.Position, World).xyz - cameraPosition;
	Out.vViewWS = cameraPosition - In.Position.xyz;
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif
	
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


float3 computeReflectionCube(float3 norm, samplerCUBE smpl)
{
	return texCUBE(smpl, norm).xyz;
}

float fresnel(float3 vecI, float3 norm, float reflectivity)
{
	return reflectivity + (1.0-reflectivity) * pow(1.0 - saturate(dot(vecI, norm)), 5.0);
}

float OrenNayar(float3 light, float3 view, float3 norm, float roughness)
{
	float VdotN = max(0.0f, dot(view, norm));
	float LdotN = max(0.0f, dot(light, norm));

	float theta_r = acos(VdotN);
	float theta_i = acos(LdotN);

	//float cos_phi = max(0.0f, dot( normalize(view - norm*VdotN), normalize(light - norm*LdotN) ));

	float alpha = max(theta_r, theta_i);
	float beta =  min(theta_r, theta_i);

	float sigma2 = roughness*roughness;
	float A = 1.0f - 0.5f * (sigma2 / (sigma2 + 0.33f));
	float B = 0.45f * (sigma2 / (sigma2 + 0.09f));

	//B *= (cos_phi >= 0) ? sin(alpha)*tan(beta) : 0.0f;
	
	return LdotN * (A + B*sin(alpha)*tan(beta));	
}

float CookTorrance(float3 Normal, float3 LightDir, float3 ViewDir, float reflectivity, float Roughness)
{
  // Sample the textures
//   float3  Normal        = normalize( ( 2.0f * tex2D( sampNormMap, v.TexCoord ).xyz ) - 1.0f );
//   float3  Specular      = tex2D( sampSpecular, v.TexCoord ).rgb;
//   float3  Diffuse       = tex2D( sampDiffuse, v.TexCoord ).rgb;
//   float2  Roughness     = tex2D( sampRoughness, v.TexCoord ).rg;
  
//	Roughness.r           *= 3.0f;

	// Correct the input and compute aliases
	float3  vHalf           = normalize( LightDir + ViewDir );
	float  NormalDotHalf    = dot( Normal, vHalf );
	float  ViewDotHalf      = dot( vHalf,  ViewDir );
	float  NormalDotView    = dot( Normal, ViewDir );
	float  NormalDotLight   = dot( Normal, LightDir );

	// Compute the geometric term
	float  G1          = ( 2.0f * NormalDotHalf * NormalDotView ) / ViewDotHalf;
	float  G2          = ( 2.0f * NormalDotHalf * NormalDotLight ) / ViewDotHalf;
	float  G           = min( 1.0f, max( 0.0f, min( G1, G2 ) ) );

	// Compute the fresnel term
	float  F = fresnel(ViewDir, Normal, reflectivity);

	// Compute the roughness term
	float  R_2        = Roughness * Roughness;
	float  NDotH_2    = NormalDotHalf * NormalDotHalf;
	float  A          = 1.0f / ( 4.0f * R_2 * NDotH_2 * NDotH_2 );
	float  B          = exp( -( 1.0f - NDotH_2 ) / ( R_2 * NDotH_2 ) );
	float  R          = A * B;

	// Compute the final term
	float  S          = ( ( G * F * R ) / ( NormalDotLight * NormalDotView + 0.000001f ) );
	return S;
}

float WardAnisotropic(float3 n, float3 l, float3 v, float2 fAnisotropicRoughness)
{
    // Make sure the interpolated inputs and
    // constant parameters are normalized
//     float3 n = normalize( f.normal );
//     float3 l = normalize( -vLightDirection );
//     float3 v = normalize( pCameraPosition - f.world );
    float3 h = normalize( l + v );
 
    // Apply a small bias to the roughness
    // coefficients to avoid divide-by-zero
    fAnisotropicRoughness += float2( 1e-5f, 1e-5f );
 
    // Define the coordinate frame
    float3 epsilon   = float3(0.0f,  1.0f, 0.0f );
    float3 tangent   = normalize( cross( n, epsilon ) );
    float3 bitangent = normalize( cross( n, tangent ) );
 
    // Generate any useful aliases
    float VdotN = dot( v, n );
    float LdotN = dot( l, n );
	if(VdotN < 0 || LdotN < 0) return 0.0f;
    float HdotN = dot( h, n );
    float HdotT = dot( h, tangent );
    float HdotB = dot( h, bitangent );
 
    // Evaluate the specular exponent
    float beta_a  = HdotT / fAnisotropicRoughness.x;
    beta_a       *= beta_a;
 
    float beta_b  = HdotB / fAnisotropicRoughness.y;
    beta_b       *= beta_b;
 
    float beta = -2.0f * ( ( beta_a + beta_b ) / ( 1.0f + HdotN ) );
 
    // Evaluate the specular denominator
    float s_den  = 4.0f * 3.14159f; 
    s_den       *= fAnisotropicRoughness.x;
    s_den       *= fAnisotropicRoughness.y;
    s_den       *= sqrt( (LdotN * VdotN) );
 
    // Compute the final specular term
    float Specular = ( exp( beta ) / (s_den) );
 
    // Composite the final value:
    return Specular;
}

void MainGeomPS(MainGeomOutput In, out float4 outColor : COLOR)
{
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;

	float3x3 bnmat = float3x3(In.Tangent, In.Binormal, In.Normal);
	float3 vecI = normalize( -In.PosV );

//  	float3 vLightTS = mul( bnmat, -dirLightDirection);
 		float3 vViewTS  = normalize( mul( bnmat, In.vViewWS ) );
//		float3 vViewTS  = normalize( mul( bnmat, vecI ) );
//  	float fParallaxLength = sqrt( dot( vViewTS, vViewTS ) - vViewTS.y * vViewTS.y ) / vViewTS.y;
//  	float2 vParallaxOffsetTS = normalize( vViewTS.xz ) * fParallaxLength * fHeightMapRange;
//  	float3 parallax = Parallax(In.UV1.xy, vParallaxOffsetTS, vViewTS, vLightTS, normalize(In.Normal));
//  	float2 vParallaxOffset = parallax.xy;

// 	In.UV1.xy = ParallaxOcclusionMap(In.UV1.xy, 0.1f, vViewTS, 150, 0.05f);
// 	outColor.rgb = 0;
// 	outColor.rg = In.UV1.xy/5;
// 	outColor.a = 1;
// 	return;




	float3 N = normal(In.UV1.xy, In.Tangent, In.Binormal, In.Normal);

	float4 diffUV = float4(In.UV1.xy, In.UV1.zw);
	float4 specTex = specularColor(diffUV.xy);
	float4 diffTex = diffColor(diffUV);

	float diffFace	= saturate(lightDirectDiff(-dirLightDirection, normalize(In.Normal)) * 5);
	
	float reflectivity = specTex.x;
	
	//float orenNayar = OrenNayar(dirLightDirection, -vecI, N, 0.2f);
	//float cookTorrance = CookTorrance(N, dirLightDirection, vecI, reflectivity, 0.1);
	//float wardAnisotropic = WardAnisotropic(normalize(In.Normal), dirLightDirection, vecI, float2(0.91f, 0.11f));

	float3 diff	= lightDirectDiff(-dirLightDirection, N);
	float3 spec = lightDirectSpec(-dirLightDirection, N, vecI, exp(specTex.y * specTex.y * 6.9)) * reflectivity * diffFace;

	//wspec = wardAnisotropic * reflectivity;
	float fresnelI = fresnel(vecI, N, reflectivity);
	float3 refl = computeReflectionCube(reflect(-vecI, N), reflectionCubeSam);
	float3 sp = 0;
 	if(reflection)
 	{
 		sp += refl * fresnelI;
 	}
 	else
	{
		sp += spec * shadow;
	}
//sp = 0;
	float3 reflectedLM = tex2D(lightMap0, In.UV2).rgb;
	float3 result = (diff * shadow * getSunColor() + sp + getAmbientColor()) * diffTex.rgb;

	//if(pointShadowMapsCount > 0) outColor.rgb = computePointLight(lightPos0, float3(1, 0.6, 0.5), In.worldPos, N, pointShadowMapSam0);
  	//if(pointShadowMapsCount > 1) outColor.rgb += computePointLight(lightPos1, float3(0, 1, 0)*2, In.worldPos, N, pointShadowMapSam1);

	outColor.rgb = result + emissionColor(diffUV.xy).rgb;
// 	outColor.rgb = 0;
// 	outColor.rg = In.UV1.xy/3;
	//outColor.rgb = diffTex.rgb;
//	outColor.a = diffTex.a;
	outColor.a = 1;

//	outColor.rgb= tex2D(lightMap0, In.UV2).rgb;
// 	outColor.rgb = 0; 
// 	outColor.rg = In.UV2;

}

//
//
//
//
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

//
//
//
//
struct ReflectionGeomInput
{
	float4 Position    : POSITION;
	float4 TextureUV   : TEXCOORD0;
	float3 Normal		: NORMAL;
};

struct ReflectionGeomOutput
{
	float4 Position  : POSITION0;
	float4 TextureUV : TEXCOORD0;
	float nDotL		 : TEXCOORD1;
};

ReflectionGeomOutput ReflectionGeomVS(ReflectionGeomInput In)
{
	ReflectionGeomOutput Out;

	float4x4 WorldViewTM = mul(World, View);

	float4 posEyeSpace = mul(In.Position, WorldViewTM);

	Out.Position	= mul(posEyeSpace, Proj);
	Out.TextureUV	= float4(In.TextureUV.x, -In.TextureUV.y, In.TextureUV.z, -In.TextureUV.w);
	Out.nDotL = max(0.0f, dot(In.Normal, dirLightDirection));
	return Out;
}

void ReflectionGeomPS(ReflectionGeomOutput In, out float4 outColor : COLOR)
{
	float4 diffTex = diffColor(In.TextureUV);
	outColor = (In.nDotL + float4(getAmbientColor(), 1)) * diffTex;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
technique DepthGeom
{
	pass p0
	{
		VertexShader     = compile vs_3_0 DepthGeomVS();
		PixelShader      = compile ps_3_0 DepthGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;

		AlphaRef = 150;
		AlphaFunc = Greater;
	}
}

technique ShadowGeom
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowGeomVS();
		PixelShader  = compile ps_3_0 ShadowGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
		
		ColorWriteEnable = 1;
		
		SlopeScaleDepthBias = 1.0; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.00001;

		AlphaRef = 150;
		AlphaFunc = Greater;
	}
}

technique MainGeom
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainGeomVS();
		PixelShader      = compile ps_3_0 MainGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;
		AlphaRef = 150;
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

technique ReflectionGeom
{
	pass p0
	{
		VertexShader     = compile vs_3_0 ReflectionGeomVS();
		PixelShader      = compile ps_3_0 ReflectionGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
	}
}