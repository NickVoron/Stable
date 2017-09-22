//
// Simple Lighting Model
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Note: This effect file works with EffectEdit.
//



// texture
texture windTex;
texture seaTex;
texture shipTex;
texture sailTex;

sampler SamplerWind = sampler_state
{
    Texture   = <windTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerSea = sampler_state
{
    Texture   = <seaTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


sampler SamplerShip = sampler_state
{
    Texture   = <shipTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerSail = sampler_state
{
    Texture   = <sailTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};



// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

float time;
float2 gridDir = float2(1, 1);


struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float2 Tex : TEXCOORD0;
};


VS_OUTPUT GridVS(
    float3 Pos  : POSITION,
    float2 Tex  : TEXCOORD0)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldView = mul(World, View);
    
    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);  // position (view space)

    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
	Out.Tex	= Tex;

    return Out;
}



float4 SeaPS( float2 Tex : TEXCOORD0 ) : COLOR
{
	float2 texC = float2(Tex.x, 1 - Tex.y);
	float4 c = tex2D(SamplerSea, texC);
	
	return c.a;
	//return 2 * (c * float4(0, 0.5, 0.4, 1));
}

float4 WindPS( float2 Tex : TEXCOORD0 ) : COLOR
{
	//float2 texC = float2(Tex.x, 1 - Tex.y);
    return tex2D(SamplerWind, Tex);
}


float4 ShipPS( float2 Tex : TEXCOORD0 ) : COLOR
{
//return 1;
    return tex2D(SamplerShip, Tex);
}

float4 SailPS( float2 Tex : TEXCOORD0 ) : COLOR
{
    return tex2D(SamplerSail, Tex);
}

technique RenderWind
{
    pass P0
    {
		VertexShader = compile vs_3_0 GridVS();
        PixelShader  = compile ps_3_0 WindPS();
		CullMode = None;
		AlphaBlendEnable = False;
        AlphaTestEnable = True;
        AlphaRef = 0x000000CF;
        AlphaFunc = GreaterEqual;
    }
}

technique RenderBackground
{
    pass P0
    {
		VertexShader = compile vs_3_0 GridVS();
        PixelShader  = compile ps_3_0 SeaPS();
		CullMode = None;
		//AlphaBlendEnable = True;
        AlphaTestEnable = True;
        AlphaRef = 5;
        AlphaFunc = GreaterEqual;
    }
}

technique RenderShip
{
    pass P0
    {
		VertexShader = compile vs_3_0 GridVS();
        PixelShader  = compile ps_3_0 ShipPS();
		CullMode = None;
		 AlphaBlendEnable = False;
         AlphaTestEnable = True;
         AlphaRef = 0x000000CF;
         AlphaFunc = GreaterEqual;
		 
		//FillMode = WireFrame;
    }
}

technique RenderSail
{
    pass P0
    {
		VertexShader = compile vs_3_0 GridVS();
        PixelShader  = compile ps_3_0 SailPS();
		CullMode = None;
		AlphaBlendEnable = False;
        AlphaTestEnable = True;
        AlphaRef = 0x000000CF;
        AlphaFunc = GreaterEqual;
    }
}
