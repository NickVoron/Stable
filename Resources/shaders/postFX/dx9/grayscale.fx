#include "common.fx"

float4 PS(STD_VS_OUTPUT In ) : COLOR
{
	float4 c = color(In.UV);
	return lerp( c, grayscale(c), 1.0f );
}

technique RenderScene
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		FillMode = SOLID;
		ZEnable = false;
    }
}
