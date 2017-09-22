cbuffer cbPerObject : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 cameraPos;
}


struct VS_INPUT
{
	float3 Pos			: POSITION;

};

struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
};

struct GS_OUTPUT
{
	float3 Pos : SV_POSITION;
};

VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;

	Out.Pos = float4(In.Pos, 1);

	return Out;
}

struct HS_ConstantOutput
{
	// Tess factor for the FF HW block
	float fTessFactor[3]    : SV_TessFactor;
	float fInsideTessFactor : SV_InsideTessFactor;
};

HS_ConstantOutput HS_Constant( InputPatch<VS_OUTPUT, 3> I )
{
	float maxf = 500.0;
	HS_ConstantOutput O;

	float f0 = min(length(I[0].Pos - cameraPos), length(I[2].Pos - cameraPos));
	float f1 = min(length(I[0].Pos - cameraPos), length(I[1].Pos - cameraPos));
	float f2 = min(length(I[1].Pos - cameraPos), length(I[2].Pos - cameraPos));
	O.fTessFactor[0] = max( maxf / f0, 1 );
	O.fTessFactor[1] = max( maxf / f1, 1 );
	O.fTessFactor[2] = max( maxf / f2, 1 );

	O.fInsideTessFactor = (O.fTessFactor[0] + O.fTessFactor[1] + O.fTessFactor[2]);
	return O;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[patchconstantfunc("HS_Constant")]
[outputcontrolpoints(3)]
VS_OUTPUT HS( InputPatch<VS_OUTPUT, 3> I, uint uCPID : SV_OutputControlPointID )
{
	VS_OUTPUT O = (VS_OUTPUT)I[uCPID];
	return O;
}

[domain("tri")]
VS_OUTPUT DS( HS_ConstantOutput HSConstantData, const OutputPatch<VS_OUTPUT, 3> I, float3 uvt : SV_DomainLocation )
{
	VS_OUTPUT Out = I[0];
	float4 p = uvt.x * I[0].Pos + uvt.y * I[1].Pos + uvt.z * I[2].Pos;

	float3 P = mul(p, (float4x3)mul(World, View));
	Out.Pos  = mul(float4(P, 1), Projection);

	return Out;
}

[maxvertexcount(3)]
void GS( triangle VS_OUTPUT i[3], inout TriangleStream<GS_OUTPUT>  TriStream )
{
	GS_OUTPUT v0; v0.Pos = i[0].Pos.xyz;
	GS_OUTPUT v1; v1.Pos = i[1].Pos.xyz;
	GS_OUTPUT v2; v2.Pos = i[2].Pos.xyz;

	TriStream.Append( v0 );
	TriStream.Append( v1 );
	TriStream.Append( v2 );
	TriStream.RestartStrip();
}

float4 PS(VS_OUTPUT In ) : SV_TARGET0
{
	return 1.0f;
}
