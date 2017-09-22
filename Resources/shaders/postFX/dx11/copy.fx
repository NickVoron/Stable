#include "common.fx"

float4 PS(STD_VS_OUTPUT In ) : SV_TARGET0
{
	//return float4(color(In.UV).xyz, 1);
	return color(In.UV);
}
