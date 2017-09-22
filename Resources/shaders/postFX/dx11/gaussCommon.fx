#include "common.fx"

static const int kernelSize = 13;

static const float PixelKernel[kernelSize] =
{
    -6,
    -5,
    -4,
    -3,
    -2,
    -1,
     0,
     1,
     2,
     3,
     4,
     5,
     6,
};

static const float BlurWeights[kernelSize] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

float4 gaussV(float2 UV)
{
	float4 Color = 0;

    for (int i = 0; i < kernelSize; i++)
    {    
        Color += color(UV + float2(0, PixelKernel[i]*texelSize.y) ) * BlurWeights[i];
    }

    return Color;
}

float4 gaussH(float2 UV)
{
	float4 Color = 0;

    for (int i = 0; i < kernelSize; i++)
    {    
        Color += color(UV + float2(PixelKernel[i]*texelSize.x, 0)) * BlurWeights[i];
    }

	return Color;
}
