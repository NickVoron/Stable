float2 screenOffset;
float2 invTextureSize;
float4x4 proj : PROJECTION;

texture texRect;

sampler samRect = sampler_state
{
	texture = <texRect>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

//
struct FONT_VS_INPUT
{
    float2 pos  : POSITION;
	float4 rect	: TEXCOORD0;
	float4 texRect	: TEXCOORD1;
	float zOrder	 : TEXCOORD2;
	float4 color: COLOR0;
};

struct FONT_VS_OUTPUT
{
    float4 pos  : POSITION;
	float2 tex	: TEXCOORD0;
	float4 color: TEXCOORD1;
};

FONT_VS_OUTPUT FontVS(FONT_VS_INPUT input)
{
    FONT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.tex = input.pos.xy * input.texRect.zw;
	output.tex += input.texRect.xy;
	output.color = input.color;
	//output.color = 1;
    return output;
}

//#define fm Wireframe
#define fm Solid


float4 FontPS(FONT_VS_OUTPUT input) : COLOR
{
	//return input.tex.y;
	//return tex2D(samRect, input.tex);
	float4 color = tex2D(samRect, input.tex).r;// * input.color; 
	color.a = length(color.xxx);
	//return float4(input.tex, 0, 1);
	return color * input.color;
}

//
struct TEX_RECT_VS_INPUT
{
    float2 pos  : POSITION;
	float4 rect	: TEXCOORD0;
	float4 texRect	: TEXCOORD1;
	float zOrder	 : TEXCOORD2;
	float4 color: COLOR0;
};

struct TEX_RECT_VS_OUTPUT
{
    float4 pos  : POSITION;
	float2 tex	: TEXCOORD0;
	float4 color: COLOR0;
};

TEX_RECT_VS_OUTPUT TexRectVS(TEX_RECT_VS_INPUT input)
{
    TEX_RECT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.tex = input.pos.xy * input.texRect.zw;
	output.tex += input.texRect.xy;
// 	output.tex *= invTextureSize;
// 	output.tex += invTextureSize * 0.5f;

	output.color = input.color;
    return output;
}

float4 TexRectPS(TEX_RECT_VS_OUTPUT input) : COLOR
{
	float4 clr = tex2D(samRect, input.tex);
	//return length(clr.a);
//	return (clr.a  > 0 );
//	return length(clr.b);
	//return (length(clr.rgb) > 0) || (clr.a > 0);
//	return (length(clr.rgb) > 0);// || (clr..a > 0);
 //	return float4(input.tex, 1, 1);
	return clr * input.color;
}

//
struct SOLID_RECT_VS_INPUT
{
    float3 pos  : POSITION;
	float4 rect	: TEXCOORD0;
	float4 color: COLOR0;
};

struct SOLID_RECT_VS_OUTPUT
{
    float4 pos  : POSITION;
	float4 color: COLOR0;
};

SOLID_RECT_VS_OUTPUT SolidRectVS(SOLID_RECT_VS_INPUT input)
{
    SOLID_RECT_VS_OUTPUT output;
	float3 resPos = input.pos;
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.color = input.color;
    return output;
}

float4 SolidRectPS(SOLID_RECT_VS_OUTPUT input) : COLOR
{
//	return 1;
	return input.color;
}



//
struct LINE_VS_INPUT
{
    float4 pos   : POSITION;
	float4 p0p1	 : TEXCOORD0;
	float4 color : COLOR0;
};

struct LINE_VS_OUTPUT
{
    float4 pos  : POSITION;
	float4 color : TEXCOORD0;
};

LINE_VS_OUTPUT LineVS(LINE_VS_INPUT input)
{
    LINE_VS_OUTPUT output;
	float2 pos = input.pos.xy * input.p0p1.xy + input.pos.zw * input.p0p1.zw;
	
	output.pos = float4(pos, 1, 1);
	output.pos = mul(output.pos, proj);
	output.color = input.color;
	
    return output;
}

float4 LinePS(LINE_VS_OUTPUT input) : COLOR
{
	return input.color;
}

struct QUAD_VS_INPUT
{
    float4 pos   : POSITION;
	float4 color : COLOR;
};

struct QUAD_VS_OUTPUT
{
    float4 pos  : POSITION;
	float4 color : COLOR;
};

QUAD_VS_OUTPUT QuadVS(QUAD_VS_INPUT input)
{
    QUAD_VS_OUTPUT output;
	float4 pos = float4(input.pos.xy, 1, 1);
	
	//output.pos = float4(pos, 1, 1);
	output.pos = mul(pos, proj);
	output.color = input.color;
	
    return output;
}

float4 QuadPS(QUAD_VS_OUTPUT input) : COLOR
{
	return input.color;
}



//
technique RenderFont
{
    pass P0
    {
		VertexShader = compile vs_3_0 FontVS();
        PixelShader  = compile ps_3_0 FontPS();
		FillMode = fm;
		CullMode = NONE;

		ZEnable = true;
		ZWriteEnable = true;
		
		AlphaTestEnable = true;
 		AlphaBlendEnable = true;
 		SrcBlend = SRCALPHA;
 		DestBlend = INVSRCALPHA;
// 		AlphaRef = 50;
// 		AlphaFunc = Greater;
		BlendOp = ADD;
    }
}

technique RenderTexture
{
    pass P0
    {
		VertexShader = compile vs_3_0 TexRectVS();
        PixelShader  = compile ps_3_0 TexRectPS();
		FillMode = fm;
		CullMode = NONE;

		ZEnable = true;
		ZWriteEnable = true;

		AlphaTestEnable = false;
 		AlphaBlendEnable = true;
 		SrcBlend = SRCALPHA;
 		DestBlend = INVSRCALPHA;
		AlphaRef = 0;
		AlphaFunc = Greater;
		BlendOp = ADD;
    }
}

technique RenderSolid
{
    pass P0
    {
		VertexShader = compile vs_3_0 SolidRectVS();
        PixelShader  = compile ps_3_0 SolidRectPS();
		FillMode = Solid;
		CullMode = NONE;

		ZEnable = true;
		ZWriteEnable = true;

		AlphaTestEnable = true;
		AlphaRef = 0;
		AlphaFunc = Greater;
    }
}

technique RenderLine
{
    pass P0
    {
		VertexShader = compile vs_3_0 LineVS();
        PixelShader  = compile ps_3_0 LinePS();
		FillMode = Wireframe;
		CullMode = NONE;

		ZEnable = true;
		ZWriteEnable = true;

		AlphaTestEnable = false;
		AlphaRef = 0;
		AlphaFunc = Greater;
    }
}

technique RenderQuad
{
    pass P0
    {
		VertexShader = compile vs_3_0 QuadVS();
        PixelShader  = compile ps_3_0 QuadPS();
		
		FillMode = Solid;
		CullMode = NONE;
    }
}

technique RenderQuadT
{
    pass P0
    {
        PixelShader  = compile ps_3_0 QuadPS();

		FillMode = Solid;
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
 		DestBlend = INVSRCALPHA;
    }
}
