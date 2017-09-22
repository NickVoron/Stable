
texture t1;
sampler t1Sam	= sampler_state {Texture = <t1>;	MaxAnisotropy = 16;	MinFilter = POINT;	MagFilter = POINT;	MipFilter = POINT;	AddressU = WRAP; AddressV = WRAP;};


struct VS_OUTPUT
{
    float4 Pos  : POSITIONT;
	float2 UV	: TEXCOORD0;
};

float4 PS(VS_OUTPUT In ) : COLOR
{
	return tex2D(t1Sam, In.UV);
}

technique RenderScene
{
    pass P0
    {
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		//FillMode = wireframe;
		ZEnable = true;
    }
}