#include "common.fx"

float vx_offset = 0.5f;
float pixel_w = 7.0f; // 15.0
float pixel_h = 6.0f; // 10.0

float4 PS(STD_VS_OUTPUT In ) : COLOR
{
	
	float rt_w = texelSize.x; // GeeXLab built-in
	float rt_h = texelSize.y; // GeeXLab built-in
	
	float2 uv = In.UV;

	float3 tc = float3(1.0, 0.0, 0.0);
	if (uv.x < (vx_offset-0.005))
	{
		float dx = pixel_w*rt_w;
		float dy = pixel_h*rt_h;
		float2 coord = float2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));
		tc = color(coord).rgb;
	}
	else if (uv.x>=(vx_offset+0.005))
	{
		tc = color(uv).rgb;
	}

	return float4(tc, 1.0f);

}

technique RenderScene
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		FillMode = SOLID;
		ZEnable = false;
    }
}
