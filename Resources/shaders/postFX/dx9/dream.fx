#include "common.fx"

float4 PS(STD_VS_OUTPUT In ) : COLOR
{
	float2 uv = In.UV;
	float4 c = color(uv);


	int count = 5;
	
	float d = 0.001f;
	for(int i = 0; i < count; ++i)
	{
		c += color(uv+d);
		c += color(uv-d);
		d+=0.002f;
	}

	
// 	c += tex2D(inTex, uv+0.003);
// 	c += tex2D(inTex, uv+0.005);
// 	c += tex2D(inTex, uv+0.007);
// 	c += tex2D(inTex, uv+0.009);
// 	c += tex2D(inTex, uv+0.011);
// 
// 	c += tex2D(inTex, uv-0.001);
// 	c += tex2D(inTex, uv-0.003);
// 	c += tex2D(inTex, uv-0.005);
// 	c += tex2D(inTex, uv-0.007);
// 	c += tex2D(inTex, uv-0.009);
// 	c += tex2D(inTex, uv-0.011);

	c = grayscale(c);
	//c.rgb = (c.r+c.g+c.b)/3.0;
	c /= count*2*0.8f;
	return c;
}

technique RenderScene
{
    pass P0
    {
        PixelShader  = compile ps_3_0 PS();
		CullMode = None;
		FillMode = SOLID;
		ZEnable = false;
    }
}
