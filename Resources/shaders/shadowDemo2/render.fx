
#define D3D9_RASTER_OFFSET

float4x4 WorldViewTM;
float4x4 ProjTM;

int ObjType = 0;

float4 SkyColor = float4(0.3, 0.8, 1.0, 1.0);


texture BaseMap;

sampler2D sampBase = sampler_state
{
	Texture = <BaseMap>;
//	MaxMipLevel = 0;
	MaxAnisotropy = 16;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
float3 DirLightColor;
float3 AmbientColor;

float3 DirLightDirectionEyeSpace;

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

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct DepthGeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct DepthGeomOutput
{
	float4 Position         : POSITION0;
	float2 TextureUV        : TEXCOORD0;
	float4 Norm_Depth       : TEXCOORD1;
};

DepthGeomOutput DepthGeomVS(DepthGeomInput In)
{
	DepthGeomOutput Out;
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, WorldViewTM));
	
	Out.Position         = mul(posEyeSpace, ProjTM);
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

struct DepthSkyInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
};

struct DepthSkyOutput
{
	float4 Position         : POSITION0;
	float4 Norm_Depth       : TEXCOORD1;
};

DepthSkyOutput DepthSkyVS(DepthSkyInput In)
{
	DepthSkyOutput Out;
	
	float4 posEyeSpace;
	posEyeSpace.xyz = mul(In.Position.xyz, (float3x3)WorldViewTM);
	posEyeSpace.w = 1.0f;
	
	Out.Position         = mul(posEyeSpace, ProjTM);
	Out.Position.z = Out.Position.w; //makes z / w = 1 ( zfar)
	
	Out.Norm_Depth.xyz   = float3(0, 0, -1);
	Out.Norm_Depth.w     = 100000;
	
	return Out;
}

void DepthSkyPS(DepthSkyOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.Norm_Depth.w, 0, 0, 0);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ResolveStencilInput
{
	float4 Position : POSITION;
};

struct ResolveStencilOutput
{
	float4 Position    : POSITION;
	float2 ScreenUV    : TEXCOORD0;
};

ResolveStencilOutput ResolveStencilVS(ResolveStencilInput In)
{
	ResolveStencilOutput Out;
	Out.Position = In.Position;
	
	Out.ScreenUV = (In.Position.xy + 1.0f) * 0.5f;
	Out.ScreenUV.y = 1.0f - Out.ScreenUV.y;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenUV += ScreenTexelUV * 0.5f;
#endif
	return Out;
}

float4 ResolveStencilPS(ResolveStencilOutput In) : COLOR
{
	return 1.0f;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowGeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct ShadowGeomOutput
{
	float4 Position    : POSITION0;
	float2 TextureUV   : TEXCOORD0;
};

ShadowGeomOutput ShadowGeomVS(ShadowGeomInput In)
{
	ShadowGeomOutput Out;
	
	float4 eyeSpacePos = mul(In.Position, WorldViewTM);
	
	Out.Position = mul(eyeSpacePos, ProjTM);
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
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct MainGeomOutput
{
	float4 Position         : POSITION;
	float2 TextureUV        : TEXCOORD0;
	float3 NormalEyeSpace   : TEXCOORD1;
	float4 ScreenProjCoords : TEXCOORD2;
};

MainGeomOutput MainGeomVS(MainGeomInput In)
{
	MainGeomOutput Out;
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, WorldViewTM));
	
	Out.Position       = mul(posEyeSpace, ProjTM);
	Out.TextureUV      = In.TextureUV;
	Out.NormalEyeSpace = normEyeSpace;
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif
	
	return Out;
}

void MainGeomPS(MainGeomOutput In, out float4 outColor : COLOR)
{
	float4 baseColor = tex2D(sampBase, In.TextureUV);
	
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float dotNL = saturate(dot(normalize(In.NormalEyeSpace), DirLightDirectionEyeSpace));
	
	float3 diffColor = DirLightColor * (dotNL * shadow) + AmbientColor;
	
	//outColor.rgb = baseColor.rgb * diffColor;
	outColor.rgb = dotNL * shadow;
	outColor.a = baseColor.a;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct MainSkyInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
};

struct MainSkyOutput
{
	float4 Position    : POSITION;
};

MainSkyOutput MainSkyVS(MainSkyInput In)
{
	MainSkyOutput Out;
	
	float4 posEyeSpace;
	posEyeSpace.xyz = mul(In.Position.xyz, (float3x3)WorldViewTM);
	posEyeSpace.w = 1.0f;
	
	Out.Position         = mul(posEyeSpace, ProjTM);
	Out.Position.z = Out.Position.w; //makes z / w = 1 ( zfar)
	
	return Out;
}

void MainSkyPS(MainSkyOutput In, out float4 outColor : COLOR)
{
	outColor = SkyColor;
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

technique DepthSky
{
	pass p0
	{
		VertexShader     = compile vs_2_0 DepthSkyVS();
		PixelShader      = compile ps_2_0 DepthSkyPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}

technique ResolveStencil
{
	pass p0
	{
		VertexShader     = compile vs_2_0 ResolveStencilVS();
		PixelShader      = compile ps_2_0 ResolveStencilPS();
		
		ZWriteEnable     = false;
		ZEnable          = false;
		
		StencilEnable    = true;
		StencilRef       = 0;
		StencilFunc      = NotEqual;
		StencilPass      = Zero;
		
		
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
		DepthBias           = 0.00001;
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
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}

technique MainSky
{
	pass p0
	{
		VertexShader     = compile vs_2_0 MainSkyVS();
		PixelShader      = compile ps_2_0 MainSkyPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}
