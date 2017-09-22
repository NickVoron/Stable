#define ONE 0.00390625
#define ONEHALF 0.001953125

const float2 resultPixelSize;

texture heightsTexture;
texture permTexture;
texture voronoiDataTex;

texture float1Tex;
texture float4Tex;

sampler2D heightsSam = sampler_state 
{
	Texture = <heightsTexture>;	
	MinFilter = POINT;	
	MagFilter = POINT;	
	MipFilter = POINT;	
};

sampler2D voronoiDataSam = sampler_state 
{
	Texture = <voronoiDataTex>;	
	MinFilter = POINT;	
	MagFilter = POINT;	
	MipFilter = POINT;	
};

sampler2D permTextureSam = sampler_state
{
	Texture = <permTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
};

sampler2D float1Sam = sampler_state 
{
	Texture = <float1Tex>;	
	MinFilter = POINT;	
	MagFilter = POINT;	
	MipFilter = POINT;	
};

sampler2D float4Sam = sampler_state 
{
	Texture = <float4Tex>;	
	MinFilter = POINT;	
	MagFilter = POINT;	
	MipFilter = POINT;	
};

const float floatDataShift = 1.0f / 8192.0f;

float getSpectralWeightForOctave(int octave, int dataShift)
{
	float tc = (octave + dataShift) * floatDataShift;
	return tex2D(float1Sam, float2(tc, floatDataShift * 0.5f) );
}

float fade(float t)
{
	//return t*t*(3.0-2.0*t); // Old fade
	return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade
}

float noise(float3 P)
{
	float3 Pi = ONE*floor(P)+ONEHALF;

	float3 Pf = P-floor(P);

	// Noise contributions from (x=0, y=0), z=0 and z=1
	float perm00 = tex2D(permTextureSam, Pi.xy).a ;
	float3  grad000 = tex2D(permTextureSam, float2(perm00, Pi.z)).rgb * 4.0 - 1.0;
	float n000 = dot(grad000, Pf);
	float3  grad001 = tex2D(permTextureSam, float2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n001 = dot(grad001, Pf - float3(0.0, 0.0, 1.0));

	// Noise contributions from (x=0, y=1), z=0 and z=1
	float perm01 = tex2D(permTextureSam, Pi.xy + float2(0.0, ONE)).a ;
	float3  grad010 = tex2D(permTextureSam, float2(perm01, Pi.z)).rgb * 4.0 - 1.0;
	float n010 = dot(grad010, Pf - float3(0.0, 1.0, 0.0));
	float3  grad011 = tex2D(permTextureSam, float2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n011 = dot(grad011, Pf - float3(0.0, 1.0, 1.0));

	// Noise contributions from (x=1, y=0), z=0 and z=1
	float perm10 = tex2D(permTextureSam, Pi.xy + float2(ONE, 0.0)).a ;
	float3  grad100 = tex2D(permTextureSam, float2(perm10, Pi.z)).rgb * 4.0 - 1.0;
	float n100 = dot(grad100, Pf - float3(1.0, 0.0, 0.0));
	float3  grad101 = tex2D(permTextureSam, float2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n101 = dot(grad101, Pf - float3(1.0, 0.0, 1.0));

	// Noise contributions from (x=1, y=1), z=0 and z=1
	float perm11 = tex2D(permTextureSam, Pi.xy + float2(ONE, ONE)).a ;
	float3  grad110 = tex2D(permTextureSam, float2(perm11, Pi.z)).rgb * 4.0 - 1.0;
	float n110 = dot(grad110, Pf - float3(1.0, 1.0, 0.0));
	float3  grad111 = tex2D(permTextureSam, float2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n111 = dot(grad111, Pf - float3(1.0, 1.0, 1.0));

	// Blend contributions along x
	float4 n_x = lerp(float4(n000, n001, n010, n011), float4(n100, n101, n110, n111), fade(Pf.x));

	// Blend contributions along y
	float2 n_xy = lerp(n_x.xy, n_x.zw, fade(Pf.y));

	// Blend contributions along z
	float n_xyz = lerp(n_xy.x, n_xy.y, fade(Pf.z));

	return n_xyz;
}


float3 plane(float x, float z)
{
	return float3(x, 0, z);
}

float3 sphere(float lon, float lat)
{
	float3 res;
	float r	= cos (lat);
	return float3(	r * cos (lon), sin (lat), r * sin (lon) );
}

float3 cylinder(float angle, float height)
{
	return float3( cos(angle), height, -sin(angle) );
}

//
//
//
//
float3 normalMap(sampler2D heightsSampler, float2 tc, float2 pixelSize)
{
	/*
	Coordinates are laid out as follows:

	0,0 | 1,0 | 2,0
	----+-----+----
	0,1 | 1,1 | 2,1
	----+-----+----
	0,2 | 1,2 | 2,2
	*/

	//return tex2D(heightsSam, tc).r;
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
	
	float h00 = tex2D(heightsSampler, o00).r;

	float h10 = tex2D(heightsSampler, o10 ).r;
	float h20 = tex2D(heightsSampler, o20 ).r;

	float h01 = tex2D(heightsSampler, o01 ).r;
	float h21 = tex2D(heightsSampler, o21 ).r;

	float h02 = tex2D(heightsSampler, o02 ).r;
	float h12 = tex2D(heightsSampler, o12 ).r;
	float h22 = tex2D(heightsSampler, o22 ).r;

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
	return normalize( float3( 2.0f * Gx, Gz, 2.0f * Gy ) );
}

float getAngle(sampler2D heightsSampler, float2 tc, float pixelSize)
{
	float3 n = normalMap(heightsSam, tc, pixelSize);
	float cosA = dot(n, float3(0, 1, 0) );
	return acos(cosA);
}