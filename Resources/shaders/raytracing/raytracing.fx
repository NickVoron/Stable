
texture resTex;
sampler res = sampler_state {Texture = <resTex>; MinFilter = NONE; MagFilter = NONE; MipFilter = NONE; AddressU = WRAP; AddressV = WRAP;};


struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float2 UV : TEXCOORD0;
};

float4 PS(VS_OUTPUT In ) : COLOR
{
	return tex2D(res, In.UV);
}

technique RenderScene
{
    pass P0
    {
		//VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		//FillMode = Wireframe;
		ZEnable = true;
		DepthBias = 1000;
    }
}
