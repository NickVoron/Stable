texture texMain;
sampler sampMain = sampler_state
{
	texture = <texMain>;
	MipFilter = point;
	MinFilter = point;
	MagFilter = point;
};

texture texMask;
sampler sampMask = sampler_state
{
	texture = <texMask>;
	MipFilter = point;
	MinFilter = point;
	MagFilter = point;
};

struct VS_INPUT
{
    float3 Pos  : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT MainVS( VS_INPUT In)
{
    VS_OUTPUT Out;
	Out.Pos = float4(In.Pos*2, 1);
	Out.uv = In.Pos.xy+0.5;
	Out.uv.y = 1-Out.uv.y;
    return Out;
}



float dropAmount = 0.0f;
float2 dropPoint = float2(0.7f, 0.7f);
float d = 1.0f/512;
float4 MainPS( VS_OUTPUT In) : COLOR
{
	float sum =
		tex2D(sampMain, In.uv-float2(d,0)).r +
		tex2D(sampMain, In.uv+float2(d,0)).r +
		tex2D(sampMain, In.uv+float2(0,d)).r +
		tex2D(sampMain, In.uv-float2(0,d)).r;
	
	float2 cc = tex2D(sampMain, In.uv).rg;
	float h = 0.9995f * ( cc.r - cc.g ) + sum*0.25f;

	if(In.uv.x<0.01f || In.uv.x>0.99f || In.uv.y<0.01f || In.uv.y>0.99f)	h = 0.95f*cc.r;
	h += saturate(0.02f - length(dropPoint-In.uv)) * dropAmount;
	//h *= tex2D(sampMask, In.uv).r;

	return float4(h, cc.r, 0, 1);
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
