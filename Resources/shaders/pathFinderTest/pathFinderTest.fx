texture maskMap;
texture pathMap;

sampler MaskSampler = sampler_state 
{
    texture = <maskMap>;
    MIPFILTER = NONE;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

sampler PathSampler = sampler_state 
{
    texture = <pathMap>;
    MIPFILTER = NONE;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

struct PS_INPUT
{
	float4 pos	: POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};

PS_OUTPUT MaskPS(PS_INPUT input)
{
	PS_OUTPUT output;
	output.color = /*tex2D(MaskSampler, input.tex0) + */tex2D(PathSampler, input.tex0);
	//output.color.rg = input.tex0;
	return output;
}


technique MaskTech
{
	pass P0
	{	 
		ZEnable = true;
		ZWriteEnable = true;
		AlphaBlendEnable = false;
		CullMode = none;
		FillMode = Solid;
    
    	PixelShader = compile ps_3_0 MaskPS();
	}
}