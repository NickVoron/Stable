#include "common.fx"

float4 PS(STD_VS_OUTPUT In) : SV_TARGET0
{
	float4 c = color(In.UV);
	return lerp( c, grayscale(c), 1.0f );
}