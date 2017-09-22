float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Proj		: PROJECTION;

float4 color;

texture textureBack2D;
sampler sampTextureBack2D = sampler_state { texture = <textureBack2D>; AddressU  = Clamp; AddressV  = Clamp; MipFilter = None; MinFilter = Point; MagFilter = Point;};

texture textureBackCube;
samplerCUBE sampTextureBackCube = sampler_state { Texture = <textureBackCube>; MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;};


struct CUBE_VS_INPUT
{
    float3 Pos  : POSITION;
};

struct CUBE_VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float2 Tex : TEXCOORD0;
	float4 Color : COLOR0;
};



float4 ColorModePS(VS_OUTPUT In) : COLOR
{
	return color;
}

float4 GradientModePS(VS_OUTPUT In) : COLOR
{
	return In.Color;
}

float4 Texture2DModePS(VS_OUTPUT In) : COLOR
{
	return tex2D(sampTextureBack2D, In.Tex);
}

CUBE_VS_OUTPUT TextureCubeModeVS(CUBE_VS_INPUT In)
{
	CUBE_VS_OUTPUT Out;
	float4x4 ViewProj = mul(View, Proj);
	float3 pos = mul(In.Pos*100, (float3x3)View);
	Out.Pos = mul(float4(pos,1), Proj);
	Out.Tex = -In.Pos * 2.0f;

	return Out;
}

float4 TextureCubeModePS(CUBE_VS_OUTPUT In) : COLOR
{
	return texCUBE(sampTextureBackCube, normalize(In.Tex));
}


technique ColorMode
{
    pass P0
    {
        PixelShader  = compile ps_2_0 ColorModePS();
		CullMode = None;
		FillMode = Solid;
		ZEnable = true;
		ZWriteEnable = false;
    }
}

technique Texture2DMode
{
    pass P0
    {
        PixelShader  = compile ps_2_0 Texture2DModePS();
		CullMode = None;
		FillMode = Solid;
		ZEnable = true;
		ZWriteEnable = false;
    }
}

technique TextureCubeMode
{
    pass P0
    {
		VertexShader  = compile vs_2_0 TextureCubeModeVS();
        PixelShader  = compile ps_2_0 TextureCubeModePS();
		CullMode = None;
		FillMode = Solid;
		ZEnable = true;
		ZWriteEnable = true;
    }
}


technique GradientMode
{
    pass P0
    {
        PixelShader  = compile ps_2_0 GradientModePS();
		CullMode = None;
		FillMode = Solid;
		ZEnable = true;
		ZWriteEnable = false;
    }
}