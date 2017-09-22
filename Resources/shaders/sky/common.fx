#include "../common/utility.fx"

struct RenderingData
{
	float4x4 View;
	float4x4 Projection;
	float4 vHG;
	float4 vInvWaveLength;
	float4 vSunPos;
	float4 fSampleOffset_fExposure_fKrESun_fKmESun;
};

RenderingData rendering;


float3 ShaderFiltering(sampler s, float2 uv, float2 pixelSize)
{
	//manually filter texture due to multiplier in A-component
	float4 vS00 = tex2D(s, uv);
	float4 vS10 = tex2D(s, uv + float2(pixelSize.y, 0.0f));
	float4 vS01 = tex2D(s, uv + float2(0.0f, pixelSize.x));
	float4 vS11 = tex2D(s, uv + pixelSize);

	float2 vK = frac(uv / pixelSize);
	float3 vD1 = lerp(vS00.rgb*vS00.a, vS10.rgb*vS10.a, vK.x);
	float3 vD2 = lerp(vS01.rgb*vS01.a, vS11.rgb*vS11.a, vK.x);
	return lerp(vD1, vD2, vK.y);
}

float3 HDR(float3 fLDR)
{
	//return fLDR;
	return 1.0 - exp(-rendering.fSampleOffset_fExposure_fKrESun_fKmESun.y * fLDR);
}

//
float GetMiePhase(float fCos, float fCos2)
{
	return rendering.vHG.x * (1.0f + fCos2) / pow(abs(rendering.vHG.y - rendering.vHG.z * fCos), 1.5f);
}

float GetRayleighPhase(float fCos2)
{
	return 0.75f * (1.0f + fCos2);
}

void getAtmosphereColorPlane(sampler atmosphereSampler, float2 tex, out float3 Rayleigh, out float3 Mie, float mieScale)
{
	float3 vSamples = ShaderFiltering(atmosphereSampler, tex, rendering.fSampleOffset_fExposure_fKrESun_fKmESun.xx);

	Rayleigh = vSamples.rgb *(rendering.vInvWaveLength.xyz * rendering.fSampleOffset_fExposure_fKrESun_fKmESun.z);
	Mie = vSamples.rgb * rendering.fSampleOffset_fExposure_fKrESun_fKmESun.w * mieScale;
}

float3 getAtmosphereColorLDR(sampler atmosphereSampler, float3 pos)
{
	float2 tex = getSkyUV(normalize(pos), 1.0f);
	float3 vC0, vC1;

	getAtmosphereColorPlane(atmosphereSampler, tex, vC0, vC1, 1.0f);

	float fCos = dot(-rendering.vSunPos.xyz, pos) / length(pos);
	float fCos2 = fCos * fCos;

	float3 res;
	res = GetRayleighPhase(fCos2) * vC0 + GetMiePhase(fCos, fCos2) * vC1;
	return res; 
}

float3 getCloudsColorLDR(sampler atmosphereSampler, float3 pos, float density)
{
	float2 tex = getSkyUV(normalize(pos), 1.0f);
	float3 vC0, vC1;
	getAtmosphereColorPlane(atmosphereSampler, tex, vC0, vC1, density);

	float fCos = dot(-rendering.vSunPos.xyz, pos) / length(pos);
	float fCos2 = fCos * fCos;

	float3 res;
	res =  GetMiePhase(fCos, fCos2) * vC1;
	return res; 
}

float3 getAtmosphereColor(sampler atmosphereSampler, float3 pos)
{
//	return getAtmosphereColorLDR(atmosphereSampler, pos);
	return HDR( getAtmosphereColorLDR(atmosphereSampler, pos) ); 
}

float3 getCloudsColor(sampler atmosphereSampler, float3 pos, float density)
{
	return HDR( getCloudsColorLDR(atmosphereSampler, pos, density) ); 
}

float3 sunLightColor(sampler atmosphereSampler)
{
	return normalize(rendering.vSunPos.xyz);// * fExposure;
}

float3 skyAmbientColor(sampler atmosphereSampler)
{
	return length(getAtmosphereColorLDR(atmosphereSampler, float3(0.009, 0.5, 0.009)))*rendering.fSampleOffset_fExposure_fKrESun_fKmESun.y;
}