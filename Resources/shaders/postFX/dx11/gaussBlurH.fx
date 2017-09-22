#include "gaussCommon.fx"

float4 PS(STD_VS_OUTPUT In) : SV_TARGET0
{
	return gaussH(In.UV);
}