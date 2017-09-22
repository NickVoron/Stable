
float computeLayeredFogDistance(float2 fogStartEnd, float camAbsY, float pointAbsY, float pointDepth)
{
	float fs = fogStartEnd.x;
	float fe = fogStartEnd.y;
	
	float2 c = float2(0, camAbsY);
	float2 p = float2(pointDepth, pointAbsY);
	
	float xx = (p.x-c.x);
	float yy = (p.y-c.y);
	
	float k = yy / xx;
	float b = c.y - (yy * c.x) / xx;
	
	float py = clamp(p.y, fs, fe);
	float cy = clamp(c.y, fs, fe);
	
	float px = (py - b) / k;
	float cx = (cy - b) / k;
	
	float2 c1 = float2(cx, cy);
	float2 p1 = float2(px, py);
	
	return length(c1 - p1);	
}

float computeSolidFog(float pointDepth, float2 minMaxDist )
{
	//линейный туман
	float norm_dist = saturate((pointDepth - minMaxDist.x) / (minMaxDist.y - minMaxDist.x) );

	float fog = 0;
	//fog = norm_dist;
	// exp. туман
	fog = 1 - saturate(1 / exp(norm_dist * fog_density));
	return fog;
}

float computeLayeredFog(float2 lfStartEnd, float2 minMaxDist, float camAbsY, float pointAbsY, float pointDepth)
{
	float lfDist 	= computeLayeredFogDistance(lfStartEnd, camAbsY, pointAbsY, pointDepth);
	float lfND 		= saturate( (lfDist - minMaxDist.x) / (minMaxDist.y - minMaxDist.x) );
	float lf 		= 1 - saturate( 1 / exp(lfND * fog_density) );
	return lf;	
}

// Calculate sky color (based on algorithms presented in "Advanced Sky Dome Rendering" by Spoerl).
float4 computeSkyColor( float NDotL )
{
	//float4 res = normalize(dot( vNormal, vLightDir ));
	//res.w = 1;
	//float4 res = float4(vNormal, 1);
	//float4 res = float4(vLightDir, 1);
	//return res / 2200;
	// The color frequencies for molecules and aerosols.
	const float4 vKr = float4( 0.0005f, 0.00025f, 0.0003f, 0.0f );
	const float4 vKm = float4( 0.00001f, 0.0004f, 0.0008f, 0.0f );

	// Calculate molecule scattering.
	float fFr = 0.0f;
	if ( NDotL > 0.0f )
	{
		fFr = NDotL < 0.0f ? 0.0f : NDotL * NDotL;	
	}
	
	fFr = fFr * 0.65f + 0.65f;

	float4 fRayleigh = fFr * vKr;

	// Calculate aerosol scattering.
	float fFm = max( NDotL, -NDotL );
	fFm = ( fFm >= 1.0f ) ? 1.65f : 0.65f;

	float4 fMie = fFm * vKm;

	// Calculate the final Sky Color.
	return ( fRayleigh + fMie );
}
