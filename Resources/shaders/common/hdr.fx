
float4 TexelStep;
float exposure;
texture ScreenColorRT;
sampler ScreenColorRTSam = sampler_state
{
	texture = <ScreenColorRT>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};


struct HDRInput
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
};

struct HDROutput
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
};

HDROutput HDRVS(HDRInput In)
{
	HDROutput Out;
	Out.Position = In.Position;
	Out.UV = In.UV;
	return Out;
}


#define DOWN_ORDER 3
float4 HDRPS(HDROutput In) : COLOR
{
	float4 res = float4(0,0,0,0);
	int2 i;
	for(i.x=0; i.x<DOWN_ORDER; ++i.x)
	{
		for(i.y=0; i.y<DOWN_ORDER; ++i.y)
		{
			res += tex2D(ScreenColorRTSam, In.UV + TexelStep.xy*i);
		}
	}
	return res/(DOWN_ORDER*DOWN_ORDER);
}

float4 FinalPS(HDROutput In) : COLOR
{
	return tex2D(ScreenColorRTSam, In.UV + TexelStep.zw) * exposure;
}


technique HDR
{
	pass p0
	{
		VertexShader = compile vs_3_0 HDRVS();
		PixelShader  = compile ps_3_0 HDRPS();
		
		ZWriteEnable     = false;
		ZEnable          = false;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}

technique Final
{
	pass p0
	{
		VertexShader = compile vs_3_0 HDRVS();
		PixelShader  = compile ps_3_0 FinalPS();
		
		ZWriteEnable     = false;
		ZEnable          = false;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}
