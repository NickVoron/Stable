cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 DirLightDirectionEyeSpace;
	float4 ScreenTexelUV;
	float4 cameraPos;
}

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 colorOBJ;
};


float2 getSphereUV(float3 pos, float r)
{
	static const float pi = 3.1415926535897932385f;
	static const float two_pi = pi * 2.0f;
	static const float invPi = 1.0f / pi;
	static const float invTwoPi = 1.0f / two_pi;

	float v = acos(pos.y / r) * invPi;
	//	v = 0.5 - pos.y / r;
	float u = (atan2(pos.x, -pos.z) + pi / 2.0)  * invTwoPi + 0.5f;
	return float2(u, v);
}


float2 getSkyUV(float3 pos, float r)
{
	float2 tex = getSphereUV(pos, r);
	tex.y *= 1.7;
	tex.y = pow(tex.y, 4);
	return tex;
}

Texture2D    atmosphereMap		: register(t3);
SamplerState atmosphereSampler	: register(s3);


struct VS_INPUT
{
	float3 pos	: POSITION;
	float2 tex0 : TEXCOORD0;						 
};

struct VS_OUTPUT
{
	float4 pos	: SV_POSITION;
	float2 tex0 : TEXCOORD0;
    float3 tex1 : TEXCOORD1;
    float depth : TEXCOORD2;
};


VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT output;

	float4x4 ViewTMZero = ViewTM;
	ViewTMZero._41 = 0.0f;
	ViewTMZero._42 = 0.0f;
	ViewTMZero._43 = 0.0f;

	float3 P = mul(float4(input.pos, 1), (float4x3)ViewTMZero);
	output.pos = mul(float4(P, 1), ProjTM);

	output.tex0 = input.tex0;
	output.tex1 = input.pos;	
	output.depth = output.pos.w;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	float2 tex = getSkyUV( normalize(input.tex1), 1.0f);	 
	
	float4 vS01;
	//float4 vS00 = float4 (1,1,1,1);//atmosphereMap.Sample(atmosphereSampler, tex);
	float4 vS00 = atmosphereMap.Sample(atmosphereSampler, tex);
	vS00.a = 1;	

	return vS00;
	
}
