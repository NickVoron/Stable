cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 					  

Texture2D    textureMap		: register( t0 );
SamplerState textureSampler	: register( s0 );

Texture2D    alphaMap		: register( t1 );
SamplerState alphaSampler	: register( s1 );

struct TEX_RECT_VS_INPUT
{
    float3 pos		: POSITION;
	float2 tex		: TEXCOORD0;
};

struct TEX_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 tex	: TEXCOORD0;
};

TEX_RECT_VS_OUTPUT VS(TEX_RECT_VS_INPUT input)
{
    TEX_RECT_VS_OUTPUT output;
	
	output.pos = mul(float4(input.pos, 1.0f), proj);
	output.tex = input.tex;
	
    return output;
}

#define SIGMA 3.0
#define BSIGMA 0.1
#define MSIZE 15

float normpdf(in float x, in float sigma)
{
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

float normpdf3(in float3 v, in float sigma)
{
	return 0.39894*exp(-0.5*dot(v,v)/(sigma*sigma))/sigma;
}


float4 PS(TEX_RECT_VS_OUTPUT input) : SV_TARGET0
{
	float2 iRes = float2(750, 1334);
	float2 uv0 = input.tex;
	float3 c = textureMap.SampleLevel(textureSampler, uv0, 0).rgb;
	//return float4(c, 1.0);
	//declare stuff
	const int kSize = (MSIZE-1)/2;
	float kernel[MSIZE];
	float3 final_colour = float3(0.0, 0.0, 0.0);
	
	//create the 1-D kernel
	float Z = 0.0;
	for (int j = 0; j <= kSize; ++j)
	{
		kernel[kSize+j] = kernel[kSize-j] = normpdf(float(j), SIGMA);
	}
	
	
	float3 cc;
	float factor;
	float bZ = 1.0/normpdf(0.0, BSIGMA);
	for (int i=-kSize; i <= kSize; ++i)
	{
		for (int j=-kSize; j <= kSize; ++j)
		{
			float2 uv = (input.tex+float2(i,j) / iRes);
			cc = textureMap.SampleLevel(textureSampler, uv, 0).rgb;
			factor = normpdf3(cc-c, BSIGMA)*bZ*kernel[kSize+j]*kernel[kSize+i];
			Z += factor;
			final_colour += factor*cc;

		}
	}
	
	
	return float4(final_colour/Z, 1.0);
}
