texture texMain;
sampler sampMain = sampler_state
{
	texture = <texMain>;
	MipFilter = point;
	MinFilter = point;
	MagFilter = point;
};


struct VS_INPUT
{
    float3 Pos  : POSITION;
    float2 uv  : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT MainVS( VS_INPUT In)
{
    VS_OUTPUT Out;
	Out.Pos = float4(In.Pos, 1);
	Out.uv = In.uv;
    return Out;
}




float4 MainPS( VS_OUTPUT In) : COLOR
{
	return abs(tex2D(sampMain, In.uv).r);
}


technique Main
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = none;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaTestEnable = false;
		AlphaBlendEnable = false;
    }
}
