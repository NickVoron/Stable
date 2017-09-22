
texture tex0;
sampler2D previewSam = sampler_state
{
	Texture = <tex0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
}; 

float4 PreviewPS( float2 Tex  : TEXCOORD0  ) : COLOR
{
	return tex2D(previewSam, Tex);
}

technique Preview
{
	pass P0
	{
		PixelShader  = compile ps_3_0 PreviewPS();
		ZEnable = false;
		CullMode = None;
	}
}
