#include "common.fx"


float4 NormalsPS( float2 Tex  : TEXCOORD0  ) : COLOR
{
	float3 color = normalMap(heightsSam, Tex, resultPixelSize);
	return float4(color, 1);
}

technique Normals
{
	pass P0
	{
		PixelShader  = compile ps_3_0 NormalsPS();
		ZEnable = false;
	}
}
