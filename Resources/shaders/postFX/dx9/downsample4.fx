#include "common.fx"

float2 PixelCoordsDownFilter[16] =
{
    { 1.5,  -1.5 },
    { 1.5,  -0.5 },
    { 1.5,   0.5 },
    { 1.5,   1.5 },

    { 0.5,  -1.5 },
    { 0.5,  -0.5 },
    { 0.5,   0.5 },
    { 0.5,   1.5 },

    {-0.5,  -1.5 },
    {-0.5,  -0.5 },
    {-0.5,   0.5 },
    {-0.5,   1.5 },

    {-1.5,  -1.5 },
    {-1.5,  -0.5 },
    {-1.5,   0.5 },
    {-1.5,   1.5 },
};


float4 PS(STD_VS_OUTPUT In ) : COLOR
{
    float4 Color = 0;

    for (int i = 0; i < 16; i++)
    {
        Color += color(In.UV + PixelCoordsDownFilter[i]*texelSize);
    }

    return Color / 16;
}


technique RenderScene
{
    pass p0
    {
        PixelShader = compile ps_2_0 PS();
        ZEnable = false;
    }
}
