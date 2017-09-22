float fHeightMapRange = 0.05f;
float3 Parallax(float2 texCoord, float2 vParallaxOffsetTS, float3 vViewWS, float3 vLightTS, float3 vNormalWS)
{
	float2 dx = ddx( texCoord );
	float2 dy = ddy( texCoord );

	float h = vViewWS.y*vViewWS.y+1;
	float ang = acos( dot(normalize(vViewWS), normalize(vNormalWS) ) );
	int nNumSteps = (int) min(60, (ang*ang*ang*40/h*fHeightMapRange*10.0f + 15));

	float fStepSize = 1.0 / (float) nNumSteps;
	float2 vTexOffsetPerStep = fStepSize * vParallaxOffsetTS;

	float fCurrHeight = 0.0;
	float fPrevHeight = 1.0;

	float2 vTexCurrentOffset = texCoord;
	float  fCurrentBound     = 1.0;

	float2 pt1 = 0;
	float2 pt2 = 0;

	int nStepIndex = 0;
	while ( nStepIndex < nNumSteps )
	{
		vTexCurrentOffset -= vTexOffsetPerStep;
		fCurrHeight = tex2Dgrad( norm1Sam, vTexCurrentOffset, dx, dy ).a;

		fCurrentBound -= fStepSize;

		if( fCurrHeight > fCurrentBound )
		{
			pt1 = float2( fCurrentBound, fCurrHeight );
			pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );
 
			nStepIndex = nNumSteps + 1;
		}
		else
		{
			nStepIndex++;
			fPrevHeight = fCurrHeight;
		}
	}

	float fDelta2 = pt2.x - pt2.y;
	float fDelta1 = pt1.x - pt1.y;
	float fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / ( fDelta2 - fDelta1 );

	float3 ret;
	ret.xy = vParallaxOffsetTS * (1 - fParallaxAmount );
	ret.z = 1.0f;

	// Self-shadow
	float2 texSample = texCoord - ret.xy;
	float2 vLightRayTS = vLightTS.xz * fHeightMapRange;

	float fShadowSoftening = 0.2f;
	float sh0 =  tex2Dgrad( norm1Sam, texSample, dx, dy ).a;
	float shA = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.88, dx, dy ).a - sh0 - 0.88 ) *  1 * fShadowSoftening;
	float sh9 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.77, dx, dy ).a - sh0 - 0.77 ) *  2 * fShadowSoftening;
	float sh8 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.66, dx, dy ).a - sh0 - 0.66 ) *  4 * fShadowSoftening;
	float sh7 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.55, dx, dy ).a - sh0 - 0.55 ) *  6 * fShadowSoftening;
	float sh6 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.44, dx, dy ).a - sh0 - 0.44 ) *  8 * fShadowSoftening;
	float sh5 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.33, dx, dy ).a - sh0 - 0.33 ) * 10 * fShadowSoftening;
	float sh4 = (tex2Dgrad( norm1Sam, texSample + vLightRayTS * 0.22, dx, dy ).a - sh0 - 0.22 ) * 12 * fShadowSoftening;
	ret.z = 1 - max( max( max( max( max( max( shA, sh9 ), sh8 ), sh7 ), sh6 ), sh5 ), sh4 );

	return ret;
}

float2 ParallaxOcclusionMap(in float2 baseTC, in float lod, in float3 viewDirNrm, in int numSteps, in float displacement)
{
	// Xzero - Added a step multiplier.
	lod = 0;
	numSteps *= displacement*25;
	
	// Clamp the step number to 100,  that way we can't have more than 100 steps.
	numSteps = clamp(numSteps, 0, 100);
	
	float step =  1.0f / numSteps;
	
	float2 delta = viewDirNrm.xy * displacement / (-viewDirNrm.z * numSteps);
	
	float4 offset = float4(baseTC, 0, lod);
	float NB0 = tex2Dlod(norm1Sam, offset).w;
			
	float height = 1 - step;
	offset.xy += delta;
	float NB1 = tex2Dlod(norm1Sam, offset).w;

	for (int i=0; i < numSteps; ++i)
	{
		[flatten]
		if (NB1 >= height)
			break;
			
		NB0 = NB1;
		height -= step;
		offset.xy += delta;
		NB1 = tex2Dlod(norm1Sam, offset).w;
	}
	
	float4 offsetBest = offset;

 	float error = 1.0;
 
 	float t1 = height;
 	float t0 = t1 + step;
 	
 	float delta1 = t1 - NB1;
 	float delta0 = t0 - NB0;
 
 	float4 intersect = float4(delta * numSteps, delta * numSteps + baseTC);
 
 	for (i = 0; i < 10; ++i)
 	{
 		if (abs(error) <= 0.001f)
 			break;
 	
 		float denom = delta1 - delta0;
 		float t = (t0 * delta1 - t1 * delta0) / denom;
 		offsetBest.xy = -t * intersect.xy + intersect.zw;
 		
 		float NB = tex2Dlod(norm1Sam, offsetBest).w;
 
 		error = t - NB;
 
 		if (error < 0)
 		{
 			delta1 = error;
 			t1 = t;
 		}
 		else
 		{
 			delta0 = error;
 			t0 = t;
 		}
 	}

	return offsetBest.xy;
}
