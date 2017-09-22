float4 color = 1;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float2 UV	: TEXCOORD0;
};

float4 PS(VS_OUTPUT In ) : COLOR
{
    return color;
}

technique RenderScene
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		FillMode = Solid;
		ZEnable = true;
    }
}
