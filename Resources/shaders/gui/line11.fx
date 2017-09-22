/*
cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 

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

LINE_VS_OUTPUT VS(LINE_VS_INPUT input)
{
    LINE_VS_OUTPUT output;
	float2 pos = input.pos.xy * input.p0p1.xy + input.pos.zw * input.p0p1.zw;
	
	output.pos = float4(pos, 1, 1);
	output.pos = mul(output.pos, proj);
	output.color = input.color;
	
    return output;
}

float4 PS(LINE_VS_OUTPUT input) : SV_TARGET0
{
	return 1;
	return input.color;
}
*/

cbuffer cbPerObject : register(b0)
{
	matrix proj;
}

struct SOLID_RECT_VS_INPUT
{
	float2 pos			: POSITION;
	float4 rect			: TEXCOORD0;
	float4 p0p1			: TEXCOORD1;
	float4 thickness	: TEXCOORD2;
	float4 color		: COLOR0;
};

struct SOLID_RECT_VS_OUTPUT
{
	float4 pos			: SV_POSITION;
	float4 color		: TEXCOORD0;
	float4 p0p1			: TEXCOORD1;
	float4 thickness	: TEXCOORD2;
	float2 spos			: TEXCOORD3;
};

SOLID_RECT_VS_OUTPUT VS(SOLID_RECT_VS_INPUT input)
{
	SOLID_RECT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1.0);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);

	output.spos = resPos.xy;
	output.color = input.color;
	output.p0p1 = input.p0p1;
	output.thickness = input.thickness;
	return output;
}
		  /*
float distance(float2 p, float2 p0, float2 p1)
{
	float2 c = p - p0;	// Vector from a to Point
	float2 v = normalize(p1-p0);// Unit Vector from a to b
	float d = length(v);	// Length of the line segment
	v /= d;
	float t = dot(v, c);	// Intersection point Distance from a

								// Check to see if the point is on the line
								// if not then return the endpoint else move from point segment.p0 to the nearest point on the segment
	float2 rp0 = p;
	float2 rp1 = (t <= 0) ? p0 : ((t >= d) ? p1 : p0 + v * t);

	return length(rp0 - rp1);
}		*/

float distance(float2 pt1, float2 pt2, float2 testPt)
{
	float2 lineDir = pt2 - pt1;
	float2 perpDir = float2(lineDir.y, -lineDir.x);
	float2 dirToPt1 = pt1 - testPt;
	return abs(dot(normalize(perpDir), dirToPt1));
}

float4 PS(SOLID_RECT_VS_OUTPUT input) : SV_TARGET0
{
	float d = distance(input.p0p1.xy, input.p0p1.zw, input.spos);

	clip(d > input.thickness ? -1 : 0);
	
//	return input.thickness/(d + 3);
	//float a = 0.5f;


	return input.color;
}