float4x4 view;
float4x4 projection;
float3 cameraPos;
float size;

struct VS_INPUT
{
	float3 pos	: POSITION;
    float4 wpos : TEXCOORD1;
	float4 color : COLOR0;
};

struct VS_OUTPUT
{
	float4 pos	: POSITION;
	float4 color : TEXCOORD1;

	float4 pPos : TEXCOORD2;
	float4 pWpos : TEXCOORD3;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};											    

VS_OUTPUT RenderStars_VS(VS_INPUT input) 
{
	VS_OUTPUT output;
	//+ 
	float3 wpos = mul( float4(input.wpos.xyz + cameraPos, 1.0f), (float4x3)view); 
	float sz = input.wpos.w * size;
	float3 pos = wpos + input.pos*sz;
	output.pPos		= float4(pos, 0.5f * input.wpos.w);
	output.pWpos	= float4(wpos, 2.0f / sz);
	output.pos		= mul( float4( pos, 1.0f ), projection );
	output.color	= input.color;
	return output;
}

PS_OUTPUT RenderStars_PS(VS_OUTPUT input)
{
	PS_OUTPUT output;
	float dist = length(input.pPos.xyz - input.pWpos.xyz);
	float lf = dist * input.pWpos.w;
	output.color = lerp(input.color, 0, pow(abs(lf), input.pPos.w));
	return output;
}

technique StarsTech
{
	pass Pass0
	{
		ZEnable = false;
		ZWriteEnable = false;

		CullMode = none;
		FillMode = solid;
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;

    	VertexShader = compile vs_3_0 RenderStars_VS();
    	PixelShader = compile ps_3_0 RenderStars_PS();
	}
}

