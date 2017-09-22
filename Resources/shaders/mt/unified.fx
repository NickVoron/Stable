float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float2 ScreenTexelUV;
int MovementType;

//#define PARALLAX
#define fm Wireframe;

//Sunlight
float4 SunDir;
float4 getSunColor()	{	return float4(1.0f, 0.8f, 0.5f, 1.0f);	}
float4 getAmbientColor()	{	return float4(0.6f, 0.6f, 1.0f, 1.0f);	}
float4 SHDir1, SHDir2, SHAmb1, SHAmb2;
float2 SHDirAmb;
float4 SHAmbHeight;

float4 CameraPosition;


float3 lightColor = float3(1,0,0);
float4 lightPos = float4(0,0,0,1);

//textures
texture shadowMap;
samplerCUBE shadowMapSam = sampler_state 
{
	Texture = <shadowMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};



//textures
texture diffuseTex;
sampler2D diffuseSam = sampler_state 
{
	Texture = <diffuseTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture heightTex;
sampler2D heightSam = sampler_state 
{
	Texture = <heightTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture parallaxSHAmbTex;
sampler2D parallaxSHAmbSam = sampler_state 
{
	Texture = <parallaxSHAmbTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture shadowTex;
sampler2D shadowSam = sampler_state 
{
	Texture = <shadowTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};



struct VS_INPUT
{
	float3 vPos : POSITION;
	float3 vNorm : NORMAL;
	float2 uvDiffuse : TEXCOORD0;
	float4 SHDir1 : TEXCOORD1;
	float4 SHDir2 : TEXCOORD2;
	float4 SHAmb1 : TEXCOORD3;
	float4 SHAmb2 : TEXCOORD4;
	float2 SHDirAmb : TEXCOORD5;
	float3 vTangent : TANGENT;
};





float fHeightMapRange = 0.15f;
float3 Parallax(float2 texCoord, float2 vParallaxOffsetTS, float3 vViewWS, float3 vLightTS, float3 vNormalWS)
{
	float2 dx = ddx( texCoord );
	float2 dy = ddy( texCoord );

	float h = vViewWS.y*vViewWS.y+1;
	float ang = acos( dot(normalize(vViewWS), normalize(vNormalWS) ) );
	int nNumSteps = 10;//(int) min(60, (ang*ang*ang*40/h*fHeightMapRange*10.0f + 15));

	float fStepSize = 1.0 / (float) nNumSteps;
	float2 vTexOffsetPerStep = fStepSize * vParallaxOffsetTS;

	float fCurrHeight = 0.0;
	float fPrevHeight = 1.0;

	float2 vTexCurrentOffset = texCoord;
	float  fCurrentBound     = 1.0;

	float2 pt1 = 0;
	float2 pt2 = 0;

	int nStepIndex = 0;
	while ( nStepIndex < nNumSteps )
	{
		vTexCurrentOffset -= vTexOffsetPerStep;
		fCurrHeight = tex2Dgrad( heightSam, vTexCurrentOffset, dx, dy ).a;

		fCurrentBound -= fStepSize;

		if( fCurrHeight > fCurrentBound )
		{
			pt1 = float2( fCurrentBound, fCurrHeight );
			pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );
 
			nStepIndex = nNumSteps + 1;
		}
		else
		{
			nStepIndex++;
			fPrevHeight = fCurrHeight;
		}
	}

	float fDelta2 = pt2.x - pt2.y;
	float fDelta1 = pt1.x - pt1.y;
	float fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / ( fDelta2 - fDelta1 );

	float3 ret;
	ret.xy = vParallaxOffsetTS * (1 - fParallaxAmount );
	ret.z = 1.0f;

	// Self-shadow
	float2 texSample = texCoord - ret.xy;
	float2 vLightRayTS = vLightTS.xz * fHeightMapRange;

	float fShadowSoftening = 0.2f;
	float sh0 =  tex2Dgrad( heightSam, texSample, dx, dy ).a;
	float shA = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.88, dx, dy ).a - sh0 - 0.88 ) *  1 * fShadowSoftening;
	float sh9 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.77, dx, dy ).a - sh0 - 0.77 ) *  2 * fShadowSoftening;
	float sh8 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.66, dx, dy ).a - sh0 - 0.66 ) *  4 * fShadowSoftening;
	float sh7 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.55, dx, dy ).a - sh0 - 0.55 ) *  6 * fShadowSoftening;
	float sh6 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.44, dx, dy ).a - sh0 - 0.44 ) *  8 * fShadowSoftening;
	float sh5 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.33, dx, dy ).a - sh0 - 0.33 ) * 10 * fShadowSoftening;
	float sh4 = (tex2Dgrad( heightSam, texSample + vLightRayTS * 0.22, dx, dy ).a - sh0 - 0.22 ) * 12 * fShadowSoftening;
	ret.z = 1 - max( max( max( max( max( max( shA, sh9 ), sh8 ), sh7 ), sh6 ), sh5 ), sh4 );

	return ret;
}



//
//DEPTH
//
struct VS_DEPTH
{
	float4 Position : POSITION;
	float2 Diffuse : TEXCOORD0;
	float  Depth : TEXCOORD1;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 vViewWS : TEXCOORD2;
};
VS_DEPTH DepthVS( VS_INPUT In)
{
	VS_DEPTH Out;

	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(In.vPos, 1), worldView);
	Out.Depth = pos.z;

	Out.Position = mul(pos, Projection);

	Out.Normal = In.vNorm;
	Out.Tangent = In.vTangent;
	Out.vViewWS = (CameraPosition.xyz - In.vPos);	//don't remember to multiply by world

	Out.Diffuse = In.uvDiffuse;
	return Out;
}

struct PS_DEPTH
{
	float4 color : COLOR0;
};
PS_DEPTH DepthPS( VS_DEPTH In ) 
{ 
	PS_DEPTH Out;
	Out.color = 1;
	//return Out;

	//повернём локальную нормаль матрицей нормали в вершине
	float3 bitangent = normalize( cross(In.Tangent, In.Normal) );
	float3x3 bnmat = float3x3(In.Tangent, In.Normal, bitangent);

	float3 vViewTS  = mul( bnmat, In.vViewWS );
	float fParallaxLength = sqrt( dot( vViewTS, vViewTS ) - vViewTS.y * vViewTS.y ) / vViewTS.y;
	float2 vParallaxOffsetTS = normalize( vViewTS.xz ) * fParallaxLength * fHeightMapRange;
	float3 parallax = Parallax(In.Diffuse, vParallaxOffsetTS, vViewTS, float3(0,0,0), normalize(In.Normal));
	float2 vParallaxOffset = parallax.xy;

	//float h = (1-tex2D(heightSam, In.Diffuse - vParallaxOffset).a) * fHeightMapRange;
	//float D = -h * dot( normalize(In.Normal), normalize(In.vViewWS) ) / dot( normalize(In.vViewWS), float3(View[0][2], View[1][2], View[2][2]) );

	float h = tex2D(heightSam, In.Diffuse - vParallaxOffset).a * fHeightMapRange;
#ifdef PARALLAX
	h = 0;
#endif
	Out.color = float4(In.Depth.x - h, 0, 0, tex2D(diffuseSam, In.Diffuse).a);
	return Out;
}
technique Depth
{
	pass P0
	{          
		VertexShader = compile vs_3_0 DepthVS();
		PixelShader  = compile ps_3_0 DepthPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		StencilEnable    = true;
		StencilRef       = <MovementType>;
		StencilPass      = Replace;
		CullMode = CCW;
		FillMode = fm;
	}
}







//
//SHADOW
//
struct VS_SHADOW
{
	float4 Position : POSITION;
	float2 Diffuse : TEXCOORD0;
};
VS_SHADOW ShadowVS( VS_INPUT In)
{
	VS_SHADOW Out;

	float3 vPos = In.vPos;// - normalize(In.vNorm)*fHeightMapRange;
	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(vPos, 1), worldView);
	Out.Position = mul(pos, Projection);
	Out.Position.x -= ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += ScreenTexelUV.y * Out.Position.w;

	Out.Diffuse = In.uvDiffuse;

	return Out;
}

struct PS_SHADOW
{
	float4 color : COLOR0;
};
PS_SHADOW ShadowPS( VS_SHADOW In ) 
{ 
	PS_SHADOW Out;
	Out.color = float4(0,0,0, tex2D(diffuseSam, In.Diffuse).a);
	return Out;
}
technique Shadow
{
	pass P0
	{          
		VertexShader = compile vs_3_0 ShadowVS();
		PixelShader  = compile ps_3_0 ShadowPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = NONE;
		SlopeScaleDepthBias = 1.001;
		DepthBias           = 0.00001;
		FillMode = fm;
	}
}






struct VS_LIGHT
{
	float4 Position : POSITION;
	float2 Diffuse : TEXCOORD0;
	float  Depth : TEXCOORD1;
	float3 Pos : TEXCOORD2;
};
VS_LIGHT LightVS( VS_INPUT In)
{
	VS_LIGHT Out;

	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(In.vPos, 1), worldView);
	Out.Pos = pos;

	Out.Position = mul(pos, Projection);
	Out.Depth = 1.0f/length(pos);

	Out.Diffuse = In.uvDiffuse;
	return Out;
}

struct PS_LIGHT
{
	float4 color : COLOR0;
};
PS_LIGHT LightPS( VS_LIGHT In )
{ 
	PS_LIGHT Out;
	//Out.color = float4(length(In.Depth), 0, 0, tex2D(diffuseSam, In.Diffuse).a);
	Out.color = float4(length(In.Pos), 0, 0, tex2D(diffuseSam, In.Diffuse).a);
	return Out;
}
technique Light
{
	pass P0
	{          
		VertexShader = compile vs_3_0 LightVS();
		PixelShader  = compile ps_3_0 LightPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = CCW;
	}
}






//
//FINAL
//
struct VS_FINAL
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Color : COLOR;
	float2 Diffuse : TEXCOORD0;
	float4 ShadowUV : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 vViewWS : TEXCOORD2;
	float3 LightDir : TEXCOORD3;
	float3 Pos : TEXCOORD4;
	float Depth : TEXCOORD5;
};
VS_FINAL FinalVS( VS_INPUT In )
{
	VS_FINAL Out;

	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(In.vPos, 1), worldView);
	Out.Pos = In.vPos;
	Out.Position = mul(pos, Projection);

	Out.ShadowUV.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ShadowUV.zw = Out.Position.zw;
	Out.ShadowUV.xy += Out.ShadowUV.ww * 0.5f * ScreenTexelUV;

	Out.Diffuse = In.uvDiffuse;

	Out.Normal = In.vNorm;
	Out.Tangent = In.vTangent;

	Out.LightDir = lightPos.xyz - In.vPos;
	Out.Depth = 1.0f/length(lightPos.xyz - In.vPos);

	Out.vViewWS = (CameraPosition.xyz - In.vPos);	//don't remember to multiply by world

	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;
	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
	Out.Color.r = dir;
	Out.Color.gb = amb;
	return Out;
}








struct PS_FINAL
{
	float4 color : COLOR0;
};
PS_FINAL FinalPS( VS_FINAL In ) 
{ 
	PS_FINAL Out;
	
	float shadow = tex2Dproj(shadowSam, In.ShadowUV);

	//повернём локальную нормаль матрицей нормали в вершине
	float3 bitangent = normalize( cross(In.Tangent, In.Normal) );
	float3x3 bnmat = float3x3(In.Tangent, In.Normal, bitangent);

	float3 vLightTS = mul( bnmat, -SunDir.xyz );
	float3 vViewTS  = mul( bnmat, In.vViewWS );
	float fParallaxLength = sqrt( dot( vViewTS, vViewTS ) - vViewTS.y * vViewTS.y ) / vViewTS.y;
	float2 vParallaxOffsetTS = normalize( vViewTS.xz ) * fParallaxLength * fHeightMapRange;
	float3 parallax = Parallax(In.Diffuse, vParallaxOffsetTS, vViewTS, vLightTS, normalize(In.Normal));
#ifdef PARALLAX
	parallax = float3(0,0,1);
#endif
	float2 vParallaxOffset = parallax.xy;
	float3 diffuseColor = tex2D(diffuseSam, In.Diffuse - vParallaxOffset);

	float3 normLocal = normalize( (tex2D(heightSam, In.Diffuse - vParallaxOffset) - float3(0.5f, 0.5f, 0.0f))*float3(2,-2,1) ).rbg;
	float3 norm = normalize( normLocal.x*In.Tangent + normLocal.y*In.Normal + normLocal.z*bitangent);
#ifdef PARALLAX
	norm = normalize( In.Normal );
#endif

	float lightDist2 = dot( In.LightDir, In.LightDir );
	float3 ld = In.LightDir/sqrt(lightDist2);
	float3 light = 0;
	//нормаль параллакса смотрит на источник, но нормаль треугольника нет
	if( dot(In.Normal, ld ) >0.0f ) light = lightColor * clamp( dot( ld, norm ), 0, 1000) / sqrt(lightDist2);
	if( texCUBE(shadowMapSam, -normalize(lightPos - In.Pos) ).r + 0.03f < length(lightPos - In.Pos) ) light = 0.0f;

	float nDotL = max(0, -dot(SunDir.xyz, norm));
	float4 shpAmb = tex2D(parallaxSHAmbSam, In.Diffuse - vParallaxOffset);
	Out.color.rgb = light + (In.Color.r + nDotL*shadow*parallax.z)*getSunColor()*0.8f + dot(shpAmb, SHAmbHeight)*In.Color.g*getAmbientColor()*0.4f;
	Out.color.rgb *= diffuseColor;

	Out.color.a = 1;
	return Out;
}
technique Final
{
	pass P0
	{          
		VertexShader = compile vs_3_0 FinalVS();
		PixelShader  = compile ps_3_0 FinalPS(); 
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = CCW;
		FillMode = fm;
	}
}
