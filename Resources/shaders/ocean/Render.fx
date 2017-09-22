float4x4 WorldViewTM : WORLDVIEW;
float4x4 ProjTM : PROJECTION;

float2 ScreenTexelUV;


texture texScreen;

sampler sampScreen = sampler_state
{
	texture = <texScreen>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};



float2 DrawScale = { 1.0f, 1.0f };
float2 DrawShift = { 0.0f, 0.0f };

texture texSource;
texture texSource1;
texture texSource2;
texture texSource3;

sampler sampSource = sampler_state
{
	texture = <texSource>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSource1 = sampler_state
{
	texture = <texSource1>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSource2 = sampler_state
{
	texture = <texSource2>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSource3 = sampler_state
{
	texture = <texSource3>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

struct DrawInput
{
    float4 Position : POSITION;
};

struct DrawOutput
{
    float4 HPos : POSITION;
    float2 UV : TEXCOORD0;
};

DrawOutput DrawVS(DrawInput In)
{
    DrawOutput Out;
    Out.HPos = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.UV = uv + ScreenTexelUV * 0.5f;
    return Out;
}

float4 DrawPS(DrawOutput In) : COLOR
{
	return tex2D(sampScreen, In.UV);
}


////

DrawOutput TestDrawVS(DrawInput In)
{
    DrawOutput Out;
    Out.HPos.xy = In.Position.xy * DrawScale + DrawShift;
    Out.HPos.zw = In.Position.zw;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.UV = uv;
    return Out;
}

float4 TestDrawPS(DrawOutput In) : COLOR
{
    //float4 c = tex2D(sampSource, In.SourceUV.xy);
    //return float4(c.xy*0.1f + 0.5f, 0, 0);
    
    float3 n;
/*    
    n.xy = tex2D(sampSource, In.SourceUV.xy*8);
    n.xy += tex2D(sampSource1, In.SourceUV.xy*4);
    n.xy += tex2D(sampSource2, In.SourceUV.xy*2);
    n.xy += tex2D(sampSource3, In.SourceUV.xy*1);
/*/
    n.xy = tex2D(sampSource, In.UV.xy);
//*/    
    
    n.z = 0.2f;
    n = normalize(n);
    
    return float4(n*0.5 + 0.5, 0);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
texture TexClipDepth;

sampler sampClipDepth = sampler_state
{
	texture = <TexClipDepth>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

texture BaseMap
<
	string ResourceType = "2D";
	string ResourceName = "default_color.dds";
>;

sampler2D sampBase = sampler_state
{
	Texture = <BaseMap>;
	MaxAnisotropy = 16;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	//SRGBTexture = True;
};

struct GeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct GeomOutput
{
	float4 Position         : POSITION;
	float2 TextureUV        : TEXCOORD0;
	float3 NormalEyeSpace   : TEXCOORD1;
	float4 ScreenProjCoords : TEXCOORD2;
};

GeomOutput GeomVS(GeomInput In)
{
	GeomOutput Out;
	
	float4 posEyeSpace = mul(In.Position, WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, WorldViewTM));
	
	Out.Position       = mul(posEyeSpace, ProjTM);
	Out.TextureUV      = In.TextureUV;
	Out.NormalEyeSpace = normEyeSpace;
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * InvViewportSize;
#endif
	return Out;
}

void GeomPS(uniform bool doClip, GeomOutput In, out float4 outColor : COLOR, out float4 depth : COLOR1)
{
	depth = In.ScreenProjCoords.w;
	if (doClip)
	{
		float clipDepth = tex2Dproj(sampClipDepth, In.ScreenProjCoords);
		clip(depth - clipDepth);
	}
	
	float4 baseColor = tex2D(sampBase, In.TextureUV);

	float3 diffColor = 1.0f;
	
	outColor.rgb = baseColor.rgb * diffColor;
	outColor.a = baseColor.a;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

textureCUBE texSkyCube;

samplerCUBE sampSkyCube = sampler_state
{
	texture = <texSkyCube>;
	AddressU  = Wrap;
	AddressV  = Wrap;
	AddressW  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

struct SkyInput
{
    float4 Position    : POSITION;
    float3 Normal      : NORMAL;
};

struct SkyOutput
{
    float4 Position    : POSITION;
    float3 TexCubeCoord : TEXCOORD0;
};

SkyOutput SkyVS(SkyInput In)
{
    SkyOutput Out;

    float4 posEyeSpace;
    posEyeSpace.xyz = mul(In.Position.xyz, (float3x3)WorldViewTM);
    posEyeSpace.w   = 1.0f;

    Out.Position   = mul(posEyeSpace, ProjTM);
    Out.Position.z = Out.Position.w; //makes z / w = 1 (zfar)
    
    Out.TexCubeCoord = In.Position.xyz;

    return Out;
}

void SkyPS(SkyOutput In, out float4 color : COLOR0, out float4 depth : COLOR1)
{
    color = texCUBE(sampSkyCube, In.TexCubeCoord);
    depth = 10000.0f;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

technique TestDraw
{
    pass p0
    {
        VertexShader = compile vs_2_0 TestDrawVS();
        PixelShader  = compile ps_2_0 TestDrawPS();
        
        CullMode = None;
    }
}

technique Draw
{
    pass p0
    {
        VertexShader = compile vs_2_0 DrawVS();
        PixelShader  = compile ps_2_0 DrawPS();
        
        CullMode = None;
		ZEnable = false;
		ZWriteEnable = false;
    }
}

technique Geom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 GeomVS();
		PixelShader      = compile ps_2_0 GeomPS(false);
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		//CullMode         = CCW;
	}
}
technique GeomClipped
{
	pass p0
	{
		VertexShader     = compile vs_2_0 GeomVS();
		PixelShader      = compile ps_2_0 GeomPS(true);
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		//CullMode         = CCW;
	}
}

technique Sky
{
	pass p0
	{
		VertexShader     = compile vs_2_0 SkyVS();
		PixelShader      = compile ps_2_0 SkyPS();
		
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}
