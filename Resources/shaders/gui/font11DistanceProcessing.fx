cbuffer cbPerObject : register( b0 )
{
	matrix proj;
} 


Texture2D    textureMap		: register( t0 );
SamplerState textureSampler	: register( s0 );


struct FONT_VS_INPUT
{
    float2 pos		: POSITION;
	float4 rect		: TEXCOORD0;
	float4 texRect	: TEXCOORD1;
	float4 color	: COLOR0;
};

struct FONT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 tex	: TEXCOORD0;
	float4 color: TEXCOORD1;
};

FONT_VS_OUTPUT VS(FONT_VS_INPUT input)
{
    FONT_VS_OUTPUT output;
	float3 resPos = float3(input.pos, 1.0f);
	resPos.xy *= input.rect.zw;
	resPos.xy += input.rect.xy;
	output.pos = float4(resPos, 1);
	output.pos = mul(output.pos, proj);
	
	output.tex = input.pos.xy * input.texRect.zw;
	output.tex += input.texRect.xy;// -float2(0.5f, 0.5f) / 256;;
	output.color = input.color;
    return output;
}

float4 PS(FONT_VS_OUTPUT input) : SV_TARGET0
{
	//float4 color = textureMap.SampleLevel(textureSampler, input.tex, 0);
	//color.a = color.r;
	//return color.r * input.color;

	float texw = 1024;
	float texh = 2048;
	float onestepu = 1 / texw;
	float onestepv = 1 / texh;

	// Scale texcoords to range ([0,texw], [0,texh])
	float2 uv = input.tex*float2(texw, texh);

	// Compute texel-local (u,v) coordinates for the four closest texels
	float2 uv00 = floor(uv); // Lower left corner of lower left texel
	float2 uvthis = floor(uv); // Lower left corner of texel containing (u,v)
	float2 uvlerp = uv - uv00; // Texel-local lerp blends [0,1]

							   // Perform explicit texture interpolation of D coefficient.
							   // This works around the currently very bad texture interpolation
							   // precision in ATI hardware.

							   // Center st00 on lower left texel and rescale to [0,1] for texture lookup
	
	float2 st00 = (uv00)* float2(onestepu, onestepv);
	//float2 st00 = (uv00);

	//return float4(textureMap.SampleLevel(textureSampler, st00, 0).rrr,1);

	// Compute g_u, g_v, D coefficients from four closest 8-bit RGBA texels
	float4 rawtex00 = textureMap.SampleLevel(textureSampler, st00, 0);
	float4 rawtex10 = textureMap.SampleLevel(textureSampler, st00 + float2(1.0*onestepu, 0.0), 0);
	float4 rawtex01 = textureMap.SampleLevel(textureSampler, st00 + float2(0.0, 1.0*onestepv), 0);
	float4 rawtex11 = textureMap.SampleLevel(textureSampler, st00 + float2(1.0*onestepu, 1.0*onestepv), 0);

	//rawtex10.a = 1;
	//return rawtex10;

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

		float w = 0.50196;
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

	float4 red = float4(1, 0, 0, 1);
	float4 green = float4(0, 0.3, 0.0, 0.8);
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


	/*
	float3 color = float3(0.0, 0.0, 0.0);
	float dist = textureMap.Sample(textureSampler, input.tex).r;
	float width = fwidth(dist);
	float alpha =1.0- smoothstep(0.5 - width, 0.5 + width, dist);
	return float4(color, alpha);	   */
}