#define DOWN_ORDER 3
#define INV_SQ_DOWN_ORDER (1.0f / DOWN_ORDER*DOWN_ORDER )

float4 downsample(sampler2D ColorRT, float2 uv, float2 TexelStep)
{
	float4 res = float4(0,0,0,0);
	int2 i;
	for(i.x = 0; i.x < DOWN_ORDER; ++i.x)
	{
		for(i.y = 0; i.y < DOWN_ORDER; ++i.y)
		{
			res += tex2D(ColorRT, uv + TexelStep * i);
		}
	}
	return res * INV_SQ_DOWN_ORDER;
}
