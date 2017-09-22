cbuffer cbPerObject : register( b0 )
{
	float4x4 viewProj;
	float4 ts;
} 

Texture2D    textureMap		: register(t0);
SamplerState textureSampler	: register(s0);

struct SOLID_RECT_VS_INPUT
{
    float3 pos  : POSITION;
	float2 uv	: TEXCOORD0;
};

struct SOLID_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 uv: TEXCOORD0;
};

SOLID_RECT_VS_OUTPUT VS(SOLID_RECT_VS_INPUT input)
{
    SOLID_RECT_VS_OUTPUT output;	 
	output.pos = mul(float4(input.pos, 1), viewProj);
	output.uv = float2(1,1) - input.uv;

    return output;
}

float4 PS(SOLID_RECT_VS_OUTPUT input) : SV_TARGET0
{	
	float texw = ts.x;
	float texh = ts.y;
	float onestepu = ts.z;
	float onestepv = ts.w;

	// Scale texcoords to range ([0,texw], [0,texh])
	float2 uv = input.uv * float2(texw, texw);

	// Compute texel-local (u,v) coordinates for the four closest texels
	float2 uv00 = floor(uv); // Lower left corner of lower left texel
	float2 uvthis = floor(uv); // Lower left corner of texel containing (u,v)
	float2 uvlerp = uv - uv00; // Texel-local lerp blends [0,1]

	// Perform explicit texture interpolation of D coefficient.
	// This works around the currently very bad texture interpolation
	// precision in ATI hardware.

	// Center st00 on lower left texel and rescale to [0,1] for texture lookup
	float2 st00 = (uv00) * float2(onestepu, onestepv);

	//return float4(textureMap.SampleLevel(textureSampler, st00, 0).rrr,1);

	// Compute g_u, g_v, D coefficients from four closest 8-bit RGBA texels
	float4 rawtex00 = textureMap.SampleLevel(textureSampler, st00, 0);
	float4 rawtex10 = textureMap.SampleLevel(textureSampler, st00 + float2(1.0*onestepu, 0.0), 0);
	float4 rawtex01 = textureMap.SampleLevel(textureSampler, st00 + float2(0.0, 1.0*onestepv), 0);
	float4 rawtex11 = textureMap.SampleLevel(textureSampler, st00 + float2(1.0*onestepu, 1.0*onestepv), 0);

	float w = 0.50196;

	// Restore the value for D from its 8-bit encoding
	float2 D00_10 = 16.0*(float2(rawtex00.r, rawtex10.r) - w);
	float2 D01_11 = 16.0*(float2(rawtex01.r, rawtex11.r) - w);

	// Interpolate D between four closest texels
	float2 uvlocal = frac(uv) - float2(0.5, 0.5); // Texel-local uv coordinates [-0.5,0.5]
	// Interpolate along v
	float2 D0_1 = lerp(D00_10, D01_11, uvlerp.y);
	// Interpolate along u

	float D = lerp(D0_1.x, D0_1.y, uvlerp.x);

	// Perform anisotropic analytic antialiasing (fwidth() is slightly wrong)
	float aastep = length(float2(ddx(D), ddy(D)));

	// 'pattern' is 1 where D>0, 0 where D<0, with proper AA around D=0.
	float pattern = smoothstep(-aastep, aastep, D);


	
	float d = 0;
	{
		float w = 0.56;
		float2 D00_10 = 16.0*(float2(rawtex00.r, rawtex10.r) - w);
		float2 D01_11 = 16.0*(float2(rawtex01.r, rawtex11.r) - w);

		// Interpolate D between four closest texels
		float2 uvlocal = frac(uv) - float2(0.5, 0.5); // Texel-local uv coordinates [-0.5,0.5]
		// Interpolate along v
		float2 D0_1 = lerp(D00_10, D01_11, uvlerp.y);
		// Interpolate along u

		float D = lerp(D0_1.x, D0_1.y, uvlerp.x);

		// Perform anisotropic analytic antialiasing (fwidth() is slightly wrong)
		float aastep = length(float2(ddx(D), ddy(D)));

		// 'pattern' is 1 where D>0, 0 where D<0, with proper AA around D=0.
		d = smoothstep(-aastep, aastep, D);
	}

	float4 red = float4(1, 0, 0 , 1);
	float4 green= float4(0, 0.3, 0.0 , 0.8);					
/*
	if(pattern==0 )
	{
		return red;	
	}

	if(pattern!=0 && pattern !=1)
	{
		float4 result = lerp(red, green, pattern);

		return result;
	}
	
	

	if(d!=0 && d !=1)
	{
		
		return float4(d*green.x, d*green.y, d*green.z, 0.5-d);	
	}

	if(d==0)
		return green;*/

	// Final fragment color
	return float4(1, 1, 1, D);
	return float4(uv, 0.0f, 1.0);
	//return float4(rawtex00.xxx, 1.0f);
}