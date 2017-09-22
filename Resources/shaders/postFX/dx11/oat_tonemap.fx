#include "common.fx"

float4 PS(STD_VS_OUTPUT In) : SV_TARGET0
{
	return color(In.UV);
}