//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

texture geomTexture;
sampler2D sampGeom = sampler_state {Texture = <geomTexture>; MaxAnisotropy = 16; MinFilter = ANISOTROPIC; MagFilter = LINEAR; MipFilter = LINEAR; };


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
	
	float4 posEyeSpace = mul(In.Position, WorldView);
	float3 normEyeSpace = normalize(mul(In.Normal, WorldView));
	
	Out.Position         = mul(posEyeSpace, Projection);
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
	
	float4 eyeSpacePos = mul(In.Position, WorldView);
	
	Out.Position = mul(eyeSpacePos, Projection);
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
	
	float4 posEyeSpace = mul(In.Position, WorldView);
	float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)WorldView));
	
	Out.Position       = mul(posEyeSpace, Projection);
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
	float4 baseColor = tex2D(sampGeom, In.TextureUV);
	
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float3 DirLightDirectionEyeSpace = mul( sunLightDir, (float3x3)View );
	float dotNL = saturate(dot(normalize(In.NormalEyeSpace), DirLightDirectionEyeSpace));
	
	float3 diffColor = sunLightColor() * (dotNL * shadow) + lightAmbient();
	
	outColor.rgb = baseColor.rgb * diffColor;
	outColor.a = baseColor.a;
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
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
	}
}


technique ResolveStencil
{
	pass p0
	{
		VertexShader     = compile vs_3_0 ResolveStencilVS();
		PixelShader      = compile ps_3_0 ResolveStencilPS();
		
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
		VertexShader = compile vs_3_0 ShadowGeomVS();
		PixelShader  = compile ps_3_0 ShadowGeomPS();
		
		ZWriteEnable     = true;
		ZEnable          = true;			
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.000001;
	}
}

technique MainGeom
{
	pass p0
	{
		VertexShader     = compile vs_3_0 MainGeomVS();
		PixelShader      = compile ps_3_0 MainGeomPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
	}
}


