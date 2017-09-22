cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 ScreenTexelUV;
	float4 cameraPos;
};

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 color;
};


struct VS_INPUT
{
	float3 Pos			: POSITION;
	float2 TileUV		: TEXCOORD0;
	float4 TileCoord	: TEXCOORD1;
	float3 TileColor	: TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float4 Norm_Depth	: TEXCOORD2;
};

struct HS_OUTPUT
{
	float4 Pos			: SV_POSITION;
};

struct DS_OUTPUT
{
	float4 Pos			: SV_POSITION;
	float4 Norm_Depth	: TEXCOORD2;
};

Texture2D    heightMap		: register(t3);
SamplerState heightSampler	: register(s3);


VS_OUTPUT VS(VS_INPUT In)
{
	VS_OUTPUT Out;

	float2 addition = (In.TileCoord.xy * In.TileCoord.zw) + In.TileUV.xy * In.TileCoord.zw;
	Out.Pos = float4(float3(addition.x, 0, addition.y), 1);

	return Out;
}

struct HS_ConstantOutput
{
	// Tess factor for the FF HW block
	float fTessFactor[4]    : SV_TessFactor;
	float fInsideTessFactor[2] : SV_InsideTessFactor;
};

HS_ConstantOutput HS_Constant(InputPatch<VS_OUTPUT, 4> I)
{
	float maxf = 500.0;
	HS_ConstantOutput O;

	float4 camera = float4(cameraPos.x, 0, cameraPos.z, 1);

	float f0 = min(length(I[0].Pos - camera), length(I[3].Pos - camera));
	float f1 = min(length(I[0].Pos - camera), length(I[1].Pos - camera));
	float f2 = min(length(I[1].Pos - camera), length(I[2].Pos - camera));
	float f3 = min(length(I[2].Pos - camera), length(I[3].Pos - camera));
	O.fTessFactor[0] = max(maxf / f0, 1);
	O.fTessFactor[1] = max(maxf / f1, 1);
	O.fTessFactor[2] = max(maxf / f2, 1);
	O.fTessFactor[3] = max(maxf / f3, 1);
	O.fInsideTessFactor[0] = (O.fTessFactor[0] + O.fTessFactor[1] + O.fTessFactor[2] + O.fTessFactor[3]) / 4;
	O.fInsideTessFactor[1] = O.fInsideTessFactor[0];
	return O;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[patchconstantfunc("HS_Constant")]
[outputcontrolpoints(4)]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 4> I, uint uCPID : SV_OutputControlPointID)
{
	VS_OUTPUT input = (VS_OUTPUT)I[uCPID];

	HS_OUTPUT output;
	output.Pos = input.Pos;

	return output;
}

[domain("quad")]
DS_OUTPUT DS(HS_ConstantOutput HSConstantData, const OutputPatch<HS_OUTPUT, 4> I, float2 uvt : SV_DomainLocation)
{
	DS_OUTPUT Out;

	float4 p = I[0].Pos + uvt.x * (I[1].Pos - I[0].Pos) + uvt.y * (I[3].Pos - I[0].Pos);

	float2 geoUV = p.xz / 4096;
	p.y = (heightMap.SampleLevel(heightSampler, geoUV, 0).x - 1) * 350;


	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float4 posEyeSpace = mul(p, WorldViewTM);
	Out.Pos = mul(posEyeSpace, ProjTM);

	//depth
	Out.Norm_Depth.w = posEyeSpace.z;

	return Out;
}

float4 PS(DS_OUTPUT In) : SV_TARGET0
{
	return float4(In.Norm_Depth.w, 0, 0, 1);
}

