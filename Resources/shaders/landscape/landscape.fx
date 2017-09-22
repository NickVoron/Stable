texture cloudsTex;
texture atmosphereTex;

texture tex0;
texture tex1;
texture tex2;
texture tex3;
texture tex4;

texture detNormal0;
texture detNormal1;
texture detNormal2;
texture detNormal3;
texture detNormal4;

texture heightMaps;

texture texDiffuse;
texture normalMap;
texture weightMap;
texture uvMap;
texture heightGlobMap;

sampler2D cloudsSam = sampler_state {Texture = <cloudsTex>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

sampler2D atmosphereSam = sampler_state 
{
	Texture = <atmosphereTex>;	
    AddressU = WRAP;        
    AddressV = CLAMP;

    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


sampler2D sampTex0 = sampler_state {Texture = <tex0>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampTex1 = sampler_state {Texture = <tex1>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampTex2 = sampler_state {Texture = <tex2>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampTex3 = sampler_state {Texture = <tex3>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampTex4 = sampler_state {Texture = <tex4>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

sampler2D sampN0 = sampler_state {Texture = <detNormal0>; MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampN1 = sampler_state {Texture = <detNormal1>; MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampN2 = sampler_state {Texture = <detNormal2>; MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampN3 = sampler_state {Texture = <detNormal3>; MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};
sampler2D sampN4 = sampler_state {Texture = <detNormal4>; MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};


sampler2D sampHeightMaps = sampler_state {Texture = <heightMaps>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};


sampler2D sampTexDiffuse = sampler_state 
{
	Texture = <texDiffuse>;	
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	
	AddressU  = Clamp;
	AddressV  = Clamp;
};

sampler2D sampNormal = sampler_state 
{
	Texture = <normalMap>;	
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = Clamp;
	AddressV  = Clamp;
};

sampler2D sampWeight = sampler_state 
{	
	Texture = <weightMap>;		
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU  = Clamp;
	AddressV  = Clamp;

};

sampler2D sampUV = sampler_state 
{	
	Texture = <uvMap>;		
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

// 	AddressU  = Clamp;
// 	AddressV  = Clamp;

};

sampler2D sampGlobHeight = sampler_state 
{
	Texture = <heightGlobMap>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU  = Clamp;
	AddressV  = Clamp;
};

bool displaySeaLevel;

float tileScale[5];
float specularCosinePower[5];
float specularAlphaScale[5];

//
float fHeightMapRange = 0.02f;
float invZoneSize = 1.0f / 1024.0f;

float ExgumatePointGamma( float2 p, float4 rect, float4 conv )
{
	float2 movedPoint = float2( p.x - rect.x, p.y - rect.y );
	float2 shift = float2( movedPoint.x * rect.w, movedPoint.y * rect.w );
	float2 shiftNegated = float2( 1 - shift.x, 1 - shift.y );

	float lodMeasure =
				conv.x * shiftNegated.x * shiftNegated.y +
				conv.y * shift.x * shiftNegated.y +
				conv.w * shift.x * shift.y +
				conv.z * shiftNegated.x * shift.y;

	return lodMeasure;
}

float4 calcTexCoord(float2 pos, float2 wPos, float level, float2 flag)
{
	float2 p = pos - wPos;
	float4 texCoord;
	texCoord.xy = p;
	texCoord.zw = level * flag;
	return texCoord;
}

float calcHeight(float4 tc, float gamma)
{
	
#ifdef USE_VERTEX_TEXTURE
	float4 texCoord = float4(tc.xy, 0, 0) * invZoneSize;
	
	float2 basePt1 = tc.xy;
	float2 basePt2 = tc.xy;
	float2 basePt3 = tc.xy;
	float2 basePt4 = tc.xy;
	
	float2 myTc = tc.zw;
	basePt1 += float2( -myTc.x, -myTc.y );
	basePt2 += float2( -myTc.x, myTc.y );
	basePt3 += float2( myTc.x, myTc.y );
	basePt4 += float2( myTc.x, -myTc.y );

   	float height = tex2Dlod(sampGlobHeight, texCoord);
	float h1 = tex2Dlod(sampGlobHeight, float4( basePt1.xy, 0, 0 ) * invZoneSize);
	float h2 = tex2Dlod(sampGlobHeight, float4( basePt2.xy, 0, 0 ) * invZoneSize);
	float h3 = tex2Dlod(sampGlobHeight, float4( basePt3.xy, 0, 0 ) * invZoneSize);
	float h4 = tex2Dlod(sampGlobHeight, float4( basePt4.xy, 0, 0 ) * invZoneSize);

	float nextLodH = (h1 + h2 + h3 + h4) * 0.25f;
	return lerp( height, nextLodH, gamma );
#endif

	return 1;
}

float2 calcLandVertexWorldXZ(float2 inPos, float2 inWPos, float4 composedOriginAndRenderSize)
{
	return inPos * composedOriginAndRenderSize.zz + composedOriginAndRenderSize.xy + inWPos;	
}

struct LandVrtPos
{
	float3 worldPos;
	float4 screenPos;	
	float4 eyeSpacePos;
};

LandVrtPos calcLandVertexPos(float2 inPos, float2 inWPos, float4 composedOriginAndRenderSize, float level, float4 lodCornerGamma, float2 oddFlag)
{
	LandVrtPos Out;
	Out.worldPos.xz = calcLandVertexWorldXZ(inPos, inWPos, composedOriginAndRenderSize);
	float4 texCoord = calcTexCoord(Out.worldPos.xz, inWPos, level, oddFlag);
	float pointGamma = ExgumatePointGamma(Out.worldPos.xz - inWPos, composedOriginAndRenderSize, lodCornerGamma);
	Out.worldPos.y = calcHeight(texCoord, pointGamma);
	//Out.worldPos.y = 0;
 	Out.eyeSpacePos = mul( float4(Out.worldPos,1), View);
	Out.screenPos = mul(Out.eyeSpacePos, Proj);
	return Out;
}
		   
struct DepthLandInput
{
	float2 Position : POSITION;
	float4 composedOriginAndRenderSize : TEXCOORD0;
	float4 lodCornerGamma : TEXCOORD1;
	float2 wPos : TEXCOORD2;
	float level : TEXCOORD3;
	float2 flag : TEXCOORD4;
};

struct DepthLandOutput
{
	float4 Position : POSITION0;
	float w			: TEXCOORD0;
};

DepthLandOutput DepthLandVS(DepthLandInput In)
{
	DepthLandOutput Out;
	LandVrtPos pos = calcLandVertexPos(In.Position, In.wPos, In.composedOriginAndRenderSize, In.level, In.lodCornerGamma, In.flag);	
	Out.Position = pos.screenPos;
	Out.w = pos.eyeSpacePos.z;

	return Out;
}

void DepthLandPS(DepthLandOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.w, 0, 0, 0);
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct ShadowLandInput
{
	float2 Position : POSITION;
	float4 composedOriginAndRenderSize : TEXCOORD0;
	float4 lodCornerGamma : TEXCOORD1;
	float2 wPos : TEXCOORD2;
	float level : TEXCOORD3;
	float2 flag : TEXCOORD4;
};

struct ShadowLandOutput
{
	float4 Position    : POSITION0;
};

ShadowLandOutput ShadowLandVS(ShadowLandInput In)
{
	ShadowLandOutput Out;
	LandVrtPos pos = calcLandVertexPos(In.Position, In.wPos, In.composedOriginAndRenderSize, In.level, In.lodCornerGamma, In.flag);
	Out.Position = pos.screenPos;

#ifdef D3D9_RASTER_OFFSET
	Out.Position.x -= ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += ScreenTexelUV.y * Out.Position.w;
#endif

	return Out;
}

void ShadowLandPS(ShadowLandOutput In, out float4 outColor : COLOR) 
{
	outColor = 0;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct MainLandInput
{
	float2 Position : POSITION;
	float4 composedOriginAndRenderSize : TEXCOORD0;
	float4 lodCornerGamma : TEXCOORD1;
	float2 wPos : TEXCOORD2;
	float level : TEXCOORD3;
	float2 flag : TEXCOORD4;
};

struct MainLandOutput
{
	float4 Position         : POSITION;
	float2 TextureUV        : TEXCOORD0;
	float4 ScreenProjCoords : TEXCOORD1;
	float3 vViewWS          : TEXCOORD2;   // view vector in world space, denormalized
	float4 fog				: TEXCOORD3;
	float  Depth			: TEXCOORD4;
};


MainLandOutput MainLandVS(MainLandInput In)
{
	MainLandOutput Out;

	LandVrtPos pos = calcLandVertexPos(In.Position, In.wPos, In.composedOriginAndRenderSize, In.level, In.lodCornerGamma, In.flag);
	Out.Position = pos.screenPos;
	Out.Depth = pos.eyeSpacePos.z;
	float2 baseUV = (pos.worldPos.xz - In.wPos) * invZoneSize;
	//	float2 uv = tex2Dlod(sampUV, float4( baseUV , 0, 0 )).rg;
	Out.TextureUV = baseUV;
    Out.vViewWS = (cameraPosition.xyz - pos.worldPos);

	// туман
	float sf = computeSolidFog(Out.Position.z, minMaxDist);
	Out.fog = float4(sf, pos.worldPos.y, Out.Position.z, 1);


	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

	return Out;
}

float3 convertNormal(float3 sampledNormal)
{
	return normalize(sampledNormal * 2.0f - 1.0f);
}

texture shAmb;
sampler2D shAmbSam = sampler_state {Texture = <shAmb>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

texture shDir;
sampler2D shDirSam = sampler_state {Texture = <shDir>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;	AddressU = WRAP; AddressV = WRAP;};

float4 SHDirHeight;
float4 SHAmbHeight;

void LandPixelFunc(MainLandOutput In, out float4 outColor : COLOR, out float4 depth : COLOR1, float shd)
{
	//	outColor = texCUBE(skyCubeSam, -In.vViewWS);
//	outColor.zw = 1;
//	return;

	//In.TextureUV = tex2D( sampUV, In.TextureUV).gb;
	
	float4 weights = tex2D( sampWeight, In.TextureUV);
	//weights = float4(1,0,0,0);
	float fifthWeight = 1.0f - (weights.x + weights.y + weights.z + weights.w);

	float4 globNorm = tex2D( sampNormal, In.TextureUV);
	float3 N = convertNormal( globNorm.xzy );

	float3 X = float3(1,0,0);
	float3 Y = N.xyz;
	float3 Z = cross(X, Y);
	float3 XX = cross(Y, Z);
	float3x3 bnmat = float3x3(XX, Y, Z);
    //float3 vViewTS  = mul( bnmat, In.vViewWS );

	float2 tileUV0 = In.TextureUV * tileScale[0];
	float2 tileUV1 = In.TextureUV * tileScale[1];
	float2 tileUV2 = In.TextureUV * tileScale[2];
	float2 tileUV3 = In.TextureUV * tileScale[3];
	float2 tileUV4 = In.TextureUV * tileScale[4];

	float3 vViewTS  = mul( bnmat, In.vViewWS );
	float2 vParallaxDirection = normalize( vViewTS.xz );

	float fLength = length( vViewTS );
	float fParallaxLength = sqrt( fLength * fLength - vViewTS.y * vViewTS.y ) / vViewTS.y;

//	fHeightMapRange = 0.02f;// * (dot(normalize(In.vViewWS), normalize(N)));
	float2 vParallaxOffsetTS = vParallaxDirection * fParallaxLength * fHeightMapRange;
	float3 vLightTS = mul( bnmat, dirLightDirection );

	//float3 parallax = Parallax(tileUV0, vParallaxOffsetTS, In.vViewWS, vLightTS, N);
	float2 vParallaxOffset = 0.0f;//parallax.xy;//vNormalWS);
	//float2 vParallaxOffset = Parallax(tileUV0, vParallaxOffsetTS, normalize(In.vViewWS), N).xy;
					
	float4 colorDet0 = tex2D( sampTex0, tileUV0 - vParallaxOffset * weights.r);

	//float4 colorDet0 = tex2D( sampTex0, tileUV0);
    float4 colorDet1 = tex2D( sampTex1, tileUV1);
    float4 colorDet2 = tex2D( sampTex2, tileUV2);
    float4 colorDet3 = tex2D( sampTex3, tileUV3);
	float4 colorDet4 = tex2D( sampTex4, tileUV4);

    float4 baseColor = 
			weights.x * colorDet0 + 
			weights.y * colorDet1 +
			weights.z * colorDet2 +
			weights.w * colorDet3 +
			fifthWeight * colorDet4 ;

	float4 nao0 = tex2D( sampN0, tileUV0 - vParallaxOffset * weights.r);

	//float4 nao0 = tex2D( sampN0, tileUV0);
	float4 nao1 = tex2D( sampN1, tileUV1);
	float4 nao2 = tex2D( sampN2, tileUV2);
	float4 nao3 = tex2D( sampN3, tileUV3);
	float4 nao4 = tex2D( sampN4, tileUV4);

    float AO = 
		weights.x * nao0.a + 
		weights.y * nao1.a +
		weights.z * nao2.a +
		weights.w * nao3.a +
		fifthWeight * nao4.a;

	float3 oN0 = weights.x * convertNormal(nao0.xyz);
	float3 oN1 = weights.y * convertNormal(nao1.xyz);
	float3 oN2 = weights.z * convertNormal(nao2.xyz);
	float3 oN3 = weights.w * convertNormal(nao3.xyz);
	float3 oN4 = fifthWeight * convertNormal(nao4.xyz);

	float3 oN = normalize(oN0 + oN1 + oN2 + oN3 + oN4);
	float3 tsN = mul( oN, bnmat );
	float gNdotL = saturate(dot(N, dirLightDirection));

	tsN = normalize(lerp(N, tsN, gNdotL));
//	tsN = N;

	// Compute the specular component
	float4 specular;
	float3 lightTS  = mul( bnmat, dirLightDirection );
	float3 vLightTSAdj = float3( lightTS.x, -lightTS.y, lightTS.z );
  	float3 vReflectionTS = normalize( 2 * dot( vViewTS, tsN ) * tsN - vViewTS );
	float4 fRdotL = saturate( dot( vReflectionTS, vLightTSAdj ));

	float4 specCosPow = float4(specularCosinePower[0], specularCosinePower[1], specularCosinePower[2], specularCosinePower[3]);
	float4 alphas = float4(colorDet0.a, colorDet1.a, colorDet2.a, colorDet3.a);
	float4 specScales = float4(specularAlphaScale[0], specularAlphaScale[1], specularAlphaScale[2], specularAlphaScale[3]);
	float4 specScaleFactor = alphas * specScales;
	specular = saturate( pow( fRdotL, specCosPow )) * specScaleFactor;

	float lastScaleFactor = colorDet4.a * specularAlphaScale[4];
	float lastSpecular = saturate( pow( fRdotL.x, specularCosinePower[4] )) * lastScaleFactor;
																	  
	float specValue =	weights.x * specular[0] + 
						weights.y * specular[1] +
						weights.z * specular[2] +
						weights.w * specular[3] +
						fifthWeight * lastSpecular;

	float dotNL = saturate(dot(tsN, dirLightDirection));
// 	if(sunLightDir.y > 0)
// 	{
// 		dotNL = ;
// 	}
// 	else
// 	{
// 		specValue = 0.0f;
// 	}
	   	
    float shadow = lerp(1.0f, tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r, clamp(0.5f*(shadowMaxDepth-In.Depth), 0.0f, 1.0f));
	shadow = max(shadow, shd);
	//shadow = 1;
	float4 clouds = 1;// - saturate(tex2D( cloudsSam, In.TextureUV)* 3) ;

	//dotNL = 1;
	float4 sunColor = sunLightColor();
	dotNL = gNdotL;
	//sunColor = 1;
	float ambient = lightAmbient() * globNorm.a;// * AO;
	float4 diffColor = (sunColor * dotNL * clouds + specValue * sunColor) * shadow + ambient;
	float4 colorDiff = tex2D( sampTexDiffuse, In.TextureUV);
		
	outColor.rgb = baseColor.rgb * diffColor * colorDiff;

	float4 shpAmb = tex2D(shAmbSam, In.TextureUV - invZoneSize*2);
	float4 shpDir = tex2D(shDirSam, In.TextureUV - invZoneSize*2);
	//outColor.rgb = shpDir.rgb;
	//outColor.rgb = baseColor.rgb * diffColor * colorDiff;
	outColor.rgb = (dot(shpDir, SHDirHeight) * sunColor * 0.6f * shadow + dot(shpAmb, SHAmbHeight) * lightAmbient() * 0.4f) * baseColor.rgb * colorDiff.rgb;
	//outColor.rgb = shpDir.rgb;
	outColor.a = 1;

	outColor.rgb = baseColor.rgb * dotNL * colorDiff + 0.3;

	float3 tc = -In.vViewWS;
	tc.y = max(tc.y, 0);
	
	float4 fc = float4( getAtmosphereColor(tc), 1.0f );
	//outColor = lerp(outColor, fc, In.fog.x);

	//outColor = colorDet0 * parallax.z;
	//outColor = dot(normalize(globNorm.xzy), float3(0,1, 0) );
	
	depth = In.ScreenProjCoords.w;
	//outColor = 1;
//	outColor = 0;
//	outColor.rg = stc;
//	fc.xyz = HDR(fc.xyz);
 //	outColor = fc;
//	return;
}

void MainLandPS(MainLandOutput In, out float4 outColor : COLOR)
{
	float4 depth;
	LandPixelFunc(In, outColor, depth, 0.0f);
}

void MainLandOceanReflPS(MainLandOutput In, out float4 outColor : COLOR, out float4 depth : COLOR1)
{
	LandPixelFunc(In, outColor, depth, 1.0f);
}

void MainLandOceanRefrPS(MainLandOutput In, out float4 outColor : COLOR, out float4 depth : COLOR1)
{
	LandPixelFunc(In, outColor, depth, 0.0f);
}


//
//
//

void SimpleLandPixelFunc(MainLandOutput In, out float4 outColor : COLOR)
{
	float4 weights = tex2D( sampWeight, In.TextureUV);
	float fifthWeight = 1.0f - (weights.x + weights.y + weights.z + weights.w);

	float4 globNorm = tex2D( sampNormal, In.TextureUV);
	float3 N = convertNormal( globNorm.xzy );

	float3 X = float3(1,0,0);
	float3 Y = N.xyz;
	float3 Z = cross(X, Y);
	float3 XX = cross(Y, Z);
	float3x3 bnmat = float3x3(XX, Y, Z);

	float2 tileUV0 = In.TextureUV * tileScale[0];
	float2 tileUV1 = In.TextureUV * tileScale[1];
	float2 tileUV2 = In.TextureUV * tileScale[2];
	float2 tileUV3 = In.TextureUV * tileScale[3];
	float2 tileUV4 = In.TextureUV * tileScale[4];

	float3 vViewTS  = mul( bnmat, In.vViewWS );
	float2 vParallaxDirection = normalize( vViewTS.xz );

	float fLength = length( vViewTS );
	float fParallaxLength = sqrt( fLength * fLength - vViewTS.y * vViewTS.y ) / vViewTS.y;

//	fHeightMapRange = 0.02f;// * (dot(normalize(In.vViewWS), normalize(N)));
	float2 vParallaxOffsetTS = vParallaxDirection * fParallaxLength * fHeightMapRange;
	float3 vLightTS = mul( bnmat, dirLightDirection );

	//float3 parallax = Parallax(tileUV0, vParallaxOffsetTS, In.vViewWS, vLightTS, N);
	float2 vParallaxOffset = 0.0f;//parallax.xy;//vNormalWS);
	//float2 vParallaxOffset = Parallax(tileUV0, vParallaxOffsetTS, normalize(In.vViewWS), N).xy;
					
	float4 colorDet0 = tex2D( sampTex0, tileUV0 - vParallaxOffset * weights.r);

	//float4 colorDet0 = tex2D( sampTex0, tileUV0);
    float4 colorDet1 = tex2D( sampTex1, tileUV1);
    float4 colorDet2 = tex2D( sampTex2, tileUV2);
    float4 colorDet3 = tex2D( sampTex3, tileUV3);
	float4 colorDet4 = tex2D( sampTex4, tileUV4);

    float4 baseColor = 
			weights.x * colorDet0 + 
			weights.y * colorDet1 +
			weights.z * colorDet2 +
			weights.w * colorDet3 +
			fifthWeight * colorDet4 ;

	float4 nao0 = tex2D( sampN0, tileUV0 - vParallaxOffset * weights.r);

	//float4 nao0 = tex2D( sampN0, tileUV0);
	float4 nao1 = tex2D( sampN1, tileUV1);
	float4 nao2 = tex2D( sampN2, tileUV2);
	float4 nao3 = tex2D( sampN3, tileUV3);
	float4 nao4 = tex2D( sampN4, tileUV4);

    float AO = 
		weights.x * nao0.a + 
		weights.y * nao1.a +
		weights.z * nao2.a +
		weights.w * nao3.a +
		fifthWeight * nao4.a;

	float3 oN0 = weights.x * convertNormal(nao0.xyz);
	float3 oN1 = weights.y * convertNormal(nao1.xyz);
	float3 oN2 = weights.z * convertNormal(nao2.xyz);
	float3 oN3 = weights.w * convertNormal(nao3.xyz);
	float3 oN4 = fifthWeight * convertNormal(nao4.xyz);

	float3 oN = normalize(oN0 + oN1 + oN2 + oN3 + oN4);
	float3 tsN = mul( oN, bnmat );
	float gNdotL = saturate(dot(N, -dirLightDirection));

	tsN = normalize(lerp(N, tsN, gNdotL));

	// Compute the specular component
	float4 specular;
	float3 lightTS  = mul( bnmat, dirLightDirection );
	float3 vLightTSAdj = float3( lightTS.x, -lightTS.y, lightTS.z );
  	float3 vReflectionTS = normalize( 2 * dot( vViewTS, tsN ) * tsN - vViewTS );
	float4 fRdotL = saturate( dot( vReflectionTS, vLightTSAdj ));

	float4 specCosPow = float4(specularCosinePower[0], specularCosinePower[1], specularCosinePower[2], specularCosinePower[3]);
	float4 alphas = float4(colorDet0.a, colorDet1.a, colorDet2.a, colorDet3.a);
	float4 specScales = float4(specularAlphaScale[0], specularAlphaScale[1], specularAlphaScale[2], specularAlphaScale[3]);
	float4 specScaleFactor = alphas * specScales;
	specular = saturate( pow( fRdotL, specCosPow )) * specScaleFactor;

	float lastScaleFactor = colorDet4.a * specularAlphaScale[4];
	float lastSpecular = saturate( pow( fRdotL.x, specularCosinePower[4] )) * lastScaleFactor;
																	  
	float specValue =	weights.x * specular[0] + 
						weights.y * specular[1] +
						weights.z * specular[2] +
						weights.w * specular[3] +
						fifthWeight * lastSpecular;

	float dotNL = saturate(dot(tsN, dirLightDirection));
	float4 clouds = 1;// - saturate(tex2D( cloudsSam, In.TextureUV)* 3) ;

	//dotNL = 1;
	float4 sunColor = sunLightColor();
	dotNL = gNdotL;
	//sunColor = 1;
	float ambient = lightAmbient() * globNorm.a;// * AO;
	float4 diffColor = (sunColor * dotNL * clouds + specValue * sunColor) + ambient;
	float4 colorDiff = tex2D( sampTexDiffuse, In.TextureUV);
		
	float4 shpAmb = tex2D(shAmbSam, In.TextureUV - invZoneSize*2);
	float4 shpDir = tex2D(shDirSam, In.TextureUV - invZoneSize*2);
	//outColor.rgb = shpDir.rgb;
	//outColor.rgb = baseColor.rgb * diffColor * colorDiff;
	outColor.rgb = (dot(shpDir, SHDirHeight)) * baseColor.rgb * colorDiff.rgb;
	//outColor.rgb = (dotNL + ambient) * baseColor.rgb * colorDiff.rgb;

//	outColor.rgb = dotNL + ambient;
//	outColor.rgb = tex2D(shMapSam, In.TextureUV);
	
	outColor.a = 1;
	float3 tc = -In.vViewWS;
	tc.y = max(tc.y, 0);
	
	float4 fc = float4( getAtmosphereColor(tc), 1.0f );
	//outColor = lerp(outColor, fc, In.fog.x);
}

void SimpleLandPS(MainLandOutput In, out float4 outColor : COLOR)
{
	SimpleLandPixelFunc(In, outColor);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#define fm Wireframe
//#define fm Solid
 
technique DepthLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 DepthLandVS();
		PixelShader      = compile ps_3_0 DepthLandPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
		//CullMode         = None;
	}
}

technique ShadowLand
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowLandVS();
		PixelShader  = compile ps_3_0 ShadowLandPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
		CullMode         = None;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.00001;
	}
}

technique MainLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainLandVS();
		PixelShader      = compile ps_3_0 MainLandPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
	}
}

technique OceanReflLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainLandVS();
		PixelShader      = compile ps_3_0 MainLandOceanReflPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
	}
}

technique OceanRefrLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainLandVS();
		PixelShader      = compile ps_3_0 MainLandOceanRefrPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
	}
}

technique SimpleLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainLandVS();
		PixelShader      = compile ps_3_0 SimpleLandPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = fm;
		//CullMode         = NONE;
	}
}
