#include "gaussCommon.fx"

float4 PS( STD_VS_OUTPUT In ) : COLOR0
{
	return gaussV(In.UV);
}

technique RenderScene
{
    pass p0
    {
        PixelShader = compile ps_2_0 PS();
        ZEnable = false;
    }
}