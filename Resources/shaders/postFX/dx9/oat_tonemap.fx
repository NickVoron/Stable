#include "common.fx"

float4 PS(STD_VS_OUTPUT In ) : COLOR
{
	return color(In.UV);
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
