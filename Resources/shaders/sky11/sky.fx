cbuffer RenderingData : register(b0)
{
	float4x4 View;
	float4x4 Proj;
	float4 vHG;
	float4 vInvWaveLength;
	float4 vSunPos_fRadius;
	float4 fSampleOffset_fExposure_fKrESun_fKmESun;
};

Texture2D    atmosphereMap		: register(t3);
SamplerState atmosphereSampler	: register(s3);

TextureCube    starMap			: register(t4);
SamplerState starSempler		: register(s4);

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

float3 HDR(float3 fLDR, float fExposure)
{
	//return fLDR;
	return 1.0 - exp(-fExposure * fLDR);
}

float GetMiePhase(float fCos, float fCos2, float3 vHG)
{
	return vHG.x * (1.0f + fCos2) / pow(abs(vHG.y - vHG.z * fCos), 1.5f);
}

float GetRayleighPhase(float fCos2)
{
	return 0.75f * (1.0f + fCos2);
}



float3 ShaderFiltering(float2 uv, float2 pixelSize)
{
	//manually filter texture due to multiplier in A-component
	float4 vS00 = atmosphereMap.Sample(atmosphereSampler, uv);
	float4 vS10 = atmosphereMap.Sample(atmosphereSampler, uv + float2(pixelSize.y, 0.0f));
	float4 vS01 = atmosphereMap.Sample(atmosphereSampler, uv + float2(0.0f, pixelSize.x));
	float4 vS11 = atmosphereMap.Sample(atmosphereSampler, uv + pixelSize);		

	float2 vK = frac(uv / pixelSize);
	float3 vD1 = lerp(vS00.rgb*vS00.a, vS10.rgb*vS10.a, vK.x);
	float3 vD2 = lerp(vS01.rgb*vS01.a, vS11.rgb*vS11.a, vK.x);
	return lerp(vD1, vD2, vK.y);
}

void getAtmosphereColorPlane(float2 tex, out float3 Rayleigh, out float3 Mie, float mieScale)
{
	float3 vSamples = ShaderFiltering(tex, fSampleOffset_fExposure_fKrESun_fKmESun.xx);
	float4 vS00 = atmosphereMap.Sample(atmosphereSampler, tex);
	//float3 vSamples = vS00.rgb*vS00.a;

	float fKrESun = fSampleOffset_fExposure_fKrESun_fKmESun.z;
	float fKmESun = fSampleOffset_fExposure_fKrESun_fKmESun.w;

	Rayleigh = vSamples.rgb * (vInvWaveLength.xyz * fKrESun);
	Mie = vSamples.rgb * fKmESun * mieScale;
}

float3 getAtmosphereColorLDR(float3 pos)
{
	float2 tex = getSkyUV(normalize(pos), 1.0f);
	float3 vC0, vC1;

	getAtmosphereColorPlane(tex, vC0, vC1, 1.0f);

	float fCos = dot(-vSunPos_fRadius.xyz, pos) / length(pos);
	float fCos2 = fCos * fCos;

	float3 res;
	res = GetRayleighPhase(fCos2) * vC0 + GetMiePhase(fCos, fCos2, vHG.xyz) * vC1;
	return res;
}

float3 getAtmosphereColor(float3 pos)
{
//	return getAtmosphereColorLDR(pos);
	return HDR(getAtmosphereColorLDR(pos), fSampleOffset_fExposure_fKrESun_fKmESun.y);
}



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

	float3 P = mul(float4(input.pos*vSunPos_fRadius.w, 1), (float4x3)View);
	output.pos = mul(float4(P, 1), Proj);
	
	output.tex0 = input.tex0;
	output.tex1 = input.pos;	
	output.depth = output.pos.w;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	float4 dayLight =  float4(getAtmosphereColor(input.tex1), 1.0f);

	float4 color = starMap.Sample(starSempler, -input.tex1);
	//return float4(input.tex1.x, input.tex1.y, 0,1);

	float param = vSunPos_fRadius.y;
	//float param = 1;
	float k = (param>0) ? 1 : max(1+ param *10, 0);
	float4 result =  lerp(color, dayLight, k);
	result.a = 1;
	return result;

}
