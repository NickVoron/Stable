#include "noise.fx" 

float4 NoisePS( float2 Tex  : TEXCOORD0  ) : COLOR
{
	float3 coord = sphere(Tex.x, Tex.y);
	float value = abs(exponent(ridged_multi(coord, 1, 2, 15, 0, 1024),3));
	return value;
}

technique Noise
{
	pass P0
	{
		PixelShader  = compile ps_3_0 NoisePS();
		ZEnable = false;
	}
}
