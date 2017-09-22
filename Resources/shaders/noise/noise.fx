#include "common.fx"

float perlin(float3 coord, float frequency, float lacunarity, float persistence, int octaveCount, int seed)
{
	float value = 0;
	float curPersistence = 1.0f;
	coord *= frequency;
	for (int curOctave = 0; curOctave < octaveCount; ++curOctave)
	{
		value += noise(coord) * curPersistence;
		coord *= lacunarity;
		curPersistence *= persistence;
	}

	return value;
}

float billow(float3 coord, float frequency, float lacunarity, float persistence, int octaveCount, int seed)
{
	float value = 0;
	float curPersistence = 1.0f;
	coord *= frequency;
	for (int curOctave = 0; curOctave < octaveCount; ++curOctave)
	{
		float signal = 2 * abs(noise(coord)) - 1;
		value += signal * curPersistence;

		coord *= lacunarity;
		curPersistence *= persistence;
	}

	return value;
}

float voronoi(float3 coord, int cellsCount, float pixelShift)
{
	float3 minColor = 0.0f;
	float minLen = 10.0f;
	for(int i = 0; i < cellsCount; ++i)
	{
		float2 tc = float2(pixelShift * i, pixelShift * 0.5);
		float3 cellCenter = tex2D(voronoiDataSam, tc).rgb;
		cellCenter.y = 0;
		coord.y = 0;
		float len = length(cellCenter - coord);
		if(len < minLen)
		{
			minLen = len;
			minColor = cellCenter;
		}
	}

	return length(minColor);
}


float ridged_multi(float3 coord, float frequency, float lacunarity, int octaveCount, int seed, int dataShift)
{
	float signal = 0.0;
	float value  = 0.0;
	float weight = 1.0;

	float offset = 1.0;
	float gain = 2.0;

	for (int curOctave = 0; curOctave < octaveCount; ++curOctave) 
	{
		signal = noise(coord * frequency);
   		signal = abs(signal);
		signal = offset - signal;
		signal *= signal;
		signal *= weight;
		weight = saturate(signal * gain);
		value += signal * getSpectralWeightForOctave(curOctave, dataShift);

		coord *= lacunarity;
	}

	return (value * 1.25) - 1.0;
}


float constant(float3 coord, float c)
{
	return c;
}

float spheres(float3 coord, float frequency)
{
	coord -= float3(0.5f, 0, 0.5f);
	coord *= frequency;

	float distFromCenter = length(coord);
	float distFromSmallerSphere = distFromCenter - int(distFromCenter);
	float distFromLargerSphere = 1.0 - distFromSmallerSphere;
	float nearestDist = min(distFromSmallerSphere, distFromLargerSphere);
	return 1.0 - (nearestDist * 2.0);
}

float cylinders(float3 coord, float frequency)
{
	coord -= float3(0.5f, 0, 0.5f);
	coord *= frequency;

	float distFromCenter = length(coord.xy);
	float distFromSmallerSphere = distFromCenter - int(distFromCenter);
	float distFromLargerSphere = 1.0 - distFromSmallerSphere;
	float nearestDist = min(distFromSmallerSphere, distFromLargerSphere);
	return 1.0 - (nearestDist * 2.0);
}

float checkboard(float3 coord, float frequency)
{
	float2 c = 0;
	coord *= frequency;
	if( frac(coord.x * 0.5f) > 0.5f) c.x = 1;
	if( frac(coord.z * 0.5f) > 0.5f) c.y = 1;

	return abs(c.x-c.y);
}

//
//
//
float multiply(float v1, float v2)	{ return v1 * v2;	  }
float add(float v1, float v2)		{ return v1 + v2;	  }
//float minimum(float v1, float v2)	{ return min(v1, v2); }
//float maximum(float v1, float v2)	{ return max(v1, v2); }
//float power(float v1, float v2)		{ return pow(v1, v2); }





//
//
//
float exponent(float val, float exponent)
{
	return (pow (abs ((val + 1.0) / 2.0), exponent) * 2.0 - 1.0);
}

float invert(float val)
{
	return -val;
}

float scaleBias(float val, float scale, float bias)
{
	return val * scale + bias;
}



//
//
//
float3 turbulence(float3 c, float power, float frequency, int roughness, int seed)
{
	float3 c0, c1, c2;
	c0.x = c.x + (12414.0 / 65536.0);
	c0.y = c.y + (65124.0 / 65536.0);
	c0.z = c.z + (31337.0 / 65536.0);

	c1.x = c.x + (26519.0 / 65536.0);
	c1.y = c.y + (18128.0 / 65536.0);
	c1.z = c.z + (60493.0 / 65536.0);

	c2.x = c.x + (53820.0 / 65536.0);
	c2.y = c.y + (11213.0 / 65536.0);
	c2.z = c.z + (44845.0 / 65536.0);

	float3 distort = float3(perlin(c0, frequency, 2, 0.5f, roughness, seed + 0),
							perlin(c1, frequency, 2, 0.5f, roughness, seed + 1),
							perlin(c2, frequency, 2, 0.5f, roughness, seed + 2)	);

	distort *= power;

	return distort;
}

float3 displace(float3 baseCoord, float3 disp)
{
	return baseCoord + disp;
}

float3 translate(float3 baseCoord, float3 t)
{
	return baseCoord + t;
}

float3 scale(float3 baseCoord, float3 scale)
{
	return baseCoord * scale;
}

float3 rotate(float3 baseCoord, float3x3 rotMat)
{
	return mul(baseCoord, rotMat);
}

//
//
//
float3 normalMap(float2 tc, float2 pixelSize)
{
	/*
	Coordinates are laid out as follows:

	0,0 | 1,0 | 2,0
	----+-----+----
	0,1 | 1,1 | 2,1
	----+-----+----
	0,2 | 1,2 | 2,2
	*/

	// Compute the necessary offsets:
	float2 o00 = tc + float2( -pixelSize.x, -pixelSize.y );
	float2 o10 = tc + float2(          0.0f, -pixelSize.y );
	float2 o20 = tc + float2(  pixelSize.x, -pixelSize.y );

	float2 o01 = tc + float2( -pixelSize.x, 0.0f          );
	float2 o21 = tc + float2(  pixelSize.x, 0.0f          );

	float2 o02 = tc + float2( -pixelSize.x,  pixelSize.y );
	float2 o12 = tc + float2(          0.0f,  pixelSize.y );
	float2 o22 = tc + float2(  pixelSize.x,  pixelSize.y );

	// Use of the sobel filter requires the eight samples
	// surrounding the current pixel:
	float h00 = tex2D(heightsSam, o00).r;
	float h10 = tex2D(heightsSam, o10 ).r;
	float h20 = tex2D(heightsSam, o20 ).r;

	float h01 = tex2D(heightsSam, o01 ).r;
	float h21 = tex2D(heightsSam, o21 ).r;

	float h02 = tex2D(heightsSam, o02 ).r;
	float h12 = tex2D(heightsSam, o12 ).r;
	float h22 = tex2D(heightsSam, o22 ).r;

	// The Sobel X kernel is:
	//
	// [ 1.0  0.0  -1.0 ]
	// [ 2.0  0.0  -2.0 ]
	// [ 1.0  0.0  -1.0 ]

	float Gx = h00 - h20 + 2.0f * h01 - 2.0f * h21 + h02 - h22;

	// The Sobel Y kernel is:
	//
	// [  1.0    2.0    1.0 ]
	// [  0.0    0.0    0.0 ]
	// [ -1.0   -2.0   -1.0 ]

	float Gy = h00 + 2.0f * h10 + h20 - h02 - 2.0f * h12 - h22;

	// Generate the missing Z component - tangent
	// space normals are +Z which makes things easier
	// The 0.5f leading coefficient can be used to control
	// how pronounced the bumps are - less than 1.0 enhances
	// and greater than 1.0 smoothes.
	float Gz = 0.5f * sqrt( 1.0f - Gx * Gx - Gy * Gy );

	// Make sure the returned normal is of unit length
	return normalize( float3( 2.0f * Gx, 2.0f * Gy, Gz ) );
}