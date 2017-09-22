
struct GPU_LINE_VS_INPUT
{
	float pos		: POSITION;
	float4 color	: COLOR0;
	float3 p0		: TEXCOORD0;
	float3 p1		: TEXCOORD1;
	float4 flag		: TEXCOORD2;
	float2 widthR	: TEXCOORD3;
};


struct QUAD_PS_INPUT
{
	float4 pos		: TEXCOORD3;
	float4 color	: COLOR0;
	float4 ex		: TEXCOORD0;
	float4 ey		: TEXCOORD1;
	float4 ez		: TEXCOORD2;
};

float4x4 viewProjection;

float2 lineVtx( float2 p0, float2 p1, float4 flag, float width )
{
	float3 v1 = normalize(p1 - p0);
	float3 v2 = cross( v1, float3(0, 1, 0 )) * ( width * 0.5f );

	float2 res =	(p0.xy - v2.xz) * flag.y +
					(p1.xy - v2.xz) * flag.z +
					(p1.xy + v2.xz) * flag.w +
					(p0.xy + v2.xz) * flag.x;

	return res;
}

QUAD_PS_INPUT GPULineVS(GPU_LINE_VS_INPUT input)
{
	QUAD_PS_INPUT Out;

	float4 sp0 = mul(input.p0, viewProjection);
	float4 sp1 = mul(input.p1, viewProjection);

	float2 pos = lineVtx(sp0.xy, sp1.xy, input.flag, input.widthR.x);
	

	return Out;
}

float filter[64];

float4 QuadPS(QUAD_PS_INPUT input) : COLOR
{
	float3 pos = input.pos.xyz;

	float3 e0 = float3(input.ex.x, input.ey.x, input.ez.x);
	float3 e1 = float3(input.ex.y, input.ey.y, input.ez.y);
	float3 e2 = float3(input.ex.z, input.ey.z, input.ez.z);
	float3 e3 = float3(input.ex.w, input.ey.w, input.ez.w);

	float4 d = float4(dot(e0, pos), dot(e1, pos),	dot(e2, pos), dot(e3, pos));

	if ((d.x < 0) ||(d.y < 0)||(d.z < 0)||(d.w < 0) ) 
	{
		return 0;
	}

	float4 color = input.color; 
	int idx0 = saturate(min(d.z, d.x))*63;
	int idx1 = saturate(min(d.y, d.w))*63;
	color.w = min(filter[idx1], filter[idx0]);

	return color;
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

technique GPULLine
{
	pass P0
	{
		VertexShader	= compile vs_3_0 GPULineVS();
		PixelShader		= compile ps_3_0 QuadPS();

		FillMode = Solid;
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}
}