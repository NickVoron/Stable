#include "common.fx"

struct VS_INPUT
{
    float3 pos	: POSITION;
    float2 tex0 : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos	: POSITION;
	float2 tex0 : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};

texture AtmosphereMap;
sampler AtmosphereMapSampler = sampler_state 
{
    Texture = <AtmosphereMap>;
    AddressU = WRAP;
    AddressV = CLAMP;

    MIPFILTER = POINT;
    MINFILTER = POINT;
    MAGFILTER = POINT;
};


struct VS_OUTPUT
{
	float4 pos	: POSITION;
	float2 tex0 : TEXCOORD0;
	float3 tex1 : TEXCOORD1;
	float depth : TEXCOORD2;
};

VS_OUTPUT RenderSky_VS(VS_INPUT input)
{
	VS_OUTPUT output;
	float3 P = mul(float4(input.pos, 1), (float4x3)rendering.View);
	output.pos = mul(float4(P, 1), rendering.Projection);
	//output.pos = mul(float4(pos.xyz, 1.0f), rendering.mWorldViewProj);
	output.tex0 = input.tex0;
	output.tex1 = input.pos;
	output.depth = output.pos.w;
	return output;
}

PS_OUTPUT RenderSky_PS(VS_OUTPUT input)
{
	PS_OUTPUT output;
	output.color = float4(getAtmosphereColor(AtmosphereMapSampler, input.tex1), 1.0f);
	return output;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
technique SkyDefault 
{
	pass RenderSky
	{	 
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		CullMode = none;
		FillMode = Solid;
    
    	VertexShader = compile vs_3_0 RenderSky_VS();
    	PixelShader = compile ps_3_0 RenderSky_PS();
	}
}


