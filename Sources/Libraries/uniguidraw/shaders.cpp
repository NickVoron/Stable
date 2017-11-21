#include "shaders.h"

namespace unigui{
namespace draw{
namespace shaders{

/*
	static const char* fontProg =	"struct FONT_VS_INPUT									"\
									"{														"\
									"   float2 pos  : POSITION;								"\
									"	float4 rect	: TEXCOORD0;							"\
									"	float4 texRect	: TEXCOORD1;						"\
									"	float4 color: COLOR0;								"\
									"};														"\
									"struct FONT_VS_OUTPUT									"\
									"{														"\
									"   float4 pos  : POSITION;								"\
									"	float2 tex	: TEXCOORD0;							"\
									"	float4 color: TEXCOORD1;							"\
									"};														"\
									"FONT_VS_OUTPUT FontVS(FONT_VS_INPUT input)				"\
									"{														"\
									"   FONT_VS_OUTPUT output;								"\
									"	float3 resPos = float3(input.pos, 1);				"\
									"	resPos.xy *= input.rect.zw;							"\
									"	resPos.xy += input.rect.xy;							"\
									"	output.pos = float4(resPos, 1);						"\
									"	output.pos = mul(output.pos, proj);					"\
									"	output.tex = input.pos.xy * input.texRect.zw;		"\
									"	output.tex += input.texRect.xy;						"\
									"	output.color = input.color;							"\
									"   return output;										"\
									"}														"\
									"float4 FontPS(FONT_VS_OUTPUT input) : COLOR			"\
									"{														"\
									"	float4 color = tex2D(samFont, input.tex);			"\
									"	color.a = length(color.xyz);						"\
									"	return color * input.color;							"\
									"}														";

	void create()
	{
		LPD3DXBUFFER vsBuf, psBuf;
		LPD3DXBUFFER vsErr, psErr;
		LPD3DXCONSTANTTABLE vsTable, psTable;

		int fontLen = strlen(fontProg);
		D3DXCompileShader(fontProg, fontLen, 0, 0, "FontVS", "vs_3_0", 0, &shaderBuf, &errBuf, &constTable);

		LPDIRECT3DPIXELSHADER9 ps;
		LPDIRECT3DVERTEXSHADER9 vs;
		LPDIRECT3DDEVICE9 dev = dx9::device;
		dev->CreateVertexShader(&shaderBuf->GetBufferPointer(), &vs);
		dev->CreatePixelShader(&shaderBuf->GetBufferPointer(), &vs);
	}

	void destroy();

	void set_font();
	void set_rect();
	void set_line();
	void set_texrect();*/


}}}
