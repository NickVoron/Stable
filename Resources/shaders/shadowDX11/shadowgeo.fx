#define D3D9_RASTER_OFFSET

cbuffer cbPerCamera : register(b0)
{
	int4 DynamicShadowSize;
	float4x4 ProjTM;
	float4x4 mLightView;
	float4x4 mCamView;
	float4 DynamicShadowTexelUV; //zw = -xy
	float4 ScreenTexelUV;
	float4 TexelSize;
};

cbuffer cbPerSplit : register(b1)
{
	float4x4 DynamicShadowTexTM;
	int4 SplitNumber;
};


Texture2D    ScreenDepthMap			: register(t0);
Texture2D    DynamicShadowDepthMap	: register(t1);
Texture2D    LightDepthMap			: register(t2);

SamplerState pointSampler	: register(s0);
SamplerState linearSampler	: register(s1);

// Derivatives of light-space depth with respect to texture coordinates
float2 DepthGradient(float2 uv, float z)
{
	float2 dz_duv = 0;

	float3 duvdist_dx = ddx(float3(uv, z));
	float3 duvdist_dy = ddy(float3(uv, z));

	dz_duv.x = duvdist_dy.y * duvdist_dx.z;
	dz_duv.x -= duvdist_dx.y * duvdist_dy.z;

	dz_duv.y = duvdist_dx.x * duvdist_dy.z;
	dz_duv.y -= duvdist_dy.x * duvdist_dx.z;

	float det = (duvdist_dx.x * duvdist_dy.y) - (duvdist_dx.y * duvdist_dy.x);
	dz_duv /= det;

	return dz_duv;
}

float shadowValue(float2 uv, float z)
{
	return DynamicShadowDepthMap.SampleLevel(linearSampler, uv, 0).r > (z - 0.0001);
}

// Performs PCF filtering on the shadow map using multiple taps in the filter region.
float PCF_Filter(float2 uv, float z0, float2 dz_duv, float2 filterRadiusUV)
{
	#define PCF_FILTER_STEP_COUNT 5

	float sum = 0;

	float2 stepUV = filterRadiusUV / PCF_FILTER_STEP_COUNT;
	for (float x = -PCF_FILTER_STEP_COUNT; x <= PCF_FILTER_STEP_COUNT; ++x)
	{
		for (float y = -PCF_FILTER_STEP_COUNT; y <= PCF_FILTER_STEP_COUNT; ++y)
		{
			float2 offset = float2(x, y) * stepUV;
			float z = z0 + dot(dz_duv, offset);
			sum += shadowValue(uv + offset.xy, z);
		}
	}
			
	float numSamples = (PCF_FILTER_STEP_COUNT * 2 + 1);
	return sum / (numSamples*numSamples);
}


//--------------------------------------------------------------------------------------
float SampleShadowPCF_NV(float4 shadowCoords, int2 ShadowSize, float4 ShadowTexelUV)
{
	float2 shadowUV = shadowCoords.xy / shadowCoords.w;

	float4 offset;
	offset.xy = frac(shadowUV * ShadowSize - 0.5f);
	offset.zw = 1 - offset.xy;

	float4 weight = 1 + offset * 2;

	float4 offsetUV = offset.zwxy + (offset / weight);
	offsetUV *= ShadowTexelUV;
	offsetUV *= shadowCoords.w;

	float4 shadowUVOffset = (offset.zwxy + 0.5f) * ShadowTexelUV;

	float4 shadowMapVals;
	shadowMapVals.x = shadowValue(shadowCoords.xy + offsetUV.zw, shadowCoords.z);
	shadowMapVals.y = shadowValue(shadowCoords.xy + offsetUV.xw, shadowCoords.z);
	shadowMapVals.z = shadowValue(shadowCoords.xy + offsetUV.zy, shadowCoords.z);
	shadowMapVals.w = shadowValue(shadowCoords.xy + offsetUV.xy, shadowCoords.z);

	float4 shadowMapWeights = weight.zxzx * weight.wwyy;
	return dot(shadowMapVals, shadowMapWeights) * (1.0f / 16);
}





/*
// Returns average blocker depth in the search region, as well as the number of found blockers.
// Blockers are defined as shadow-map samples between the surface point and the light.
void FindBlocker(out float avgBlockerDepth,
	out float numBlockers,
	float2 uv,
	float z0,
	float2 dz_duv,
	float2 searchRegionRadiusUV)
{
	float blockerSum = 0;
	numBlockers = 0;
#define BLOCKER_SEARCH_STEP_COUNT 7

	float2 stepUV = searchRegionRadiusUV / BLOCKER_SEARCH_STEP_COUNT;
		for (float x = -BLOCKER_SEARCH_STEP_COUNT; x <= BLOCKER_SEARCH_STEP_COUNT; ++x)
			for (float y = -BLOCKER_SEARCH_STEP_COUNT; y <= BLOCKER_SEARCH_STEP_COUNT; ++y)
			{
				float2 offset = float2(x, y) * stepUV;
					float z = z0 + dot(dz_duv, offset);
				float shadowMapDepth = tex2D(sampShadowDepth_Point, uv + offset).r;
				//if ( shadowMapDepth == 1.0f )
				//if ( shadowMapDepth < 10.0 )
				if (shadowMapDepth < z)
				{
					blockerSum += shadowMapDepth;
					numBlockers++;
				}
			}
	avgBlockerDepth = blockerSum / numBlockers;
}


/*

float g_LightZNear = 0.0f;
float g_LightZFar = 100.0f;

float PCSS_Shadow(float2 uv, float z, float2 dz_duv, float zEye, float2 g_LightRadiusUV)
{
	// ------------------------
	// STEP 1: blocker search
	// ------------------------
	float avgBlockerDepth = 0;
	float numBlockers = 0;
	float2 searchRegionRadiusUV = g_LightRadiusUV * 10;//(zEye - g_LightZNear) / zEye;
		FindBlocker(avgBlockerDepth, numBlockers, uv, z, dz_duv, searchRegionRadiusUV);

	// Early out if no blocker found
	if (numBlockers == 0) return 1.0;

	// ------------------------
	// STEP 2: penumbra size
	// ------------------------
	float avgBlockerDepthWorld = g_LightZFar*g_LightZNear / (g_LightZFar - avgBlockerDepth*(g_LightZFar - g_LightZNear));
	float2 penumbraRadiusUV = g_LightRadiusUV * (zEye - avgBlockerDepthWorld) / avgBlockerDepthWorld;
		float2 filterRadiusUV = penumbraRadiusUV * g_LightZNear / zEye;

		filterRadiusUV = g_LightRadiusUV;
	// ------------------------
	// STEP 3: filtering
	// ------------------------
	return PCF_Filter(uv, z, dz_duv, filterRadiusUV);
}

*/
struct ShadowReceiveInput
{
	float3 Position : POSITION;
};

struct ShadowReceiveOutput
{
	float4 Position    : SV_POSITION;
	float4 ScreenProj  : TEXCOORD0;
	float3 EyeSpacePos : TEXCOORD1;
};

ShadowReceiveOutput VS(ShadowReceiveInput In)
{
	ShadowReceiveOutput Out;
	Out.Position = mul(float4(In.Position, 1), ProjTM);

	Out.ScreenProj.xy = Out.Position.xy * float2(0.5f, -0.5f) + (Out.Position.ww * 0.5f);
	Out.ScreenProj.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProj.xy += (Out.ScreenProj.w * 0.5f) * ScreenTexelUV.xy;
#endif

	Out.EyeSpacePos = In.Position;

	return Out;
}

float4 PS(ShadowReceiveOutput In) : SV_TARGET0
{
	float screenDepth = ScreenDepthMap.SampleLevel(linearSampler, In.ScreenProj.xy / In.ScreenProj.w, 0).r;

	float3 viewPos = In.EyeSpacePos;
	viewPos.xyz /= viewPos.z;
	viewPos.xyz *= screenDepth;
	
	/*
	float4 projectedViewPos = mul(float4(viewPos, 1), transpose(ProjTM));
	projectedViewPos.xy /= projectedViewPos.w;
	float screenDepth1 = ScreenDepthMap.SampleLevel(linearSampler, projectedViewPos.xy, 0).r;

	float3 viewPos1 = In.EyeSpacePos;
	viewPos1.xyz /= viewPos1.z;
	viewPos1.xyz *= screenDepth1;
	*/

	float4 shadowCoordsDyn = mul(float4(viewPos, 1), DynamicShadowTexTM);
	float lightDepth = DynamicShadowDepthMap.SampleLevel(pointSampler, shadowCoordsDyn.xy, 0).r;
	//float lightAmountDyn = (lightDepth + 0.00005) > shadowCoordsDyn.z;

	float2 uv = shadowCoordsDyn.xy / shadowCoordsDyn.w;
	float z = shadowCoordsDyn.z / shadowCoordsDyn.w;
	// Compute gradient using ddx/ddy before any branching
	float2 dz_duv = DepthGradient(uv, z);
	//float lightAmountDyn = PCF_Filter(uv, z, dz_duv, TexelSize.xy * max(saturate(SplitNumber.y - SplitNumber.x), 4.0f));
	float lightAmountDyn = PCF_Filter(uv, z, dz_duv, TexelSize.xy * 4);

	//float lightAmountDyn = SampleShadowPCF_NV(shadowCoordsDyn, DynamicShadowSize.xy, DynamicShadowTexelUV);

	//lightAmountDyn = SplitNumber.x / 4.0;

	float4 outColor;
	outColor.r = lightAmountDyn;
	outColor.gba = 0;
	return outColor;
}