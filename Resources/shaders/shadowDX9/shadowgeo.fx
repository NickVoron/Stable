#define D3D9_RASTER_OFFSET

struct PerCamera
{
	int4 DynamicShadowSize;
	float4x4 ProjTM;
	float4x4 mLightView;
	float4x4 mCamView;	
	float4 DynamicShadowTexelUV; //zw = -xy
	float4 ScreenTexelUV;
	float4 TexelSize;
};

struct PerSplit
{
	float4x4 DynamicShadowTexTM;
};

PerCamera camera;
PerSplit split;


texture ScreenDepthMap;
sampler sampScreenDepth = sampler_state
{
	texture = <ScreenDepthMap>;
	AddressU = Clamp;
	AddressV = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

texture DynamicShadowDepthMap;
sampler sampShadowDepth_Linear = sampler_state
{
	texture = <DynamicShadowDepthMap>;
	AddressU = Clamp;
	AddressV = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

texture LightDepth;
sampler sampShadowDepth_Point = sampler_state
{
	texture = <LightDepth>;
	AddressU = Clamp;
	AddressV = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};


// Performs PCF filtering on the shadow map using multiple taps in the filter region.
float PCF_Filter(float2 uv, float z0, float2 dz_duv, float2 filterRadiusUV)
{
	float sum = 0;
#define PCF_FILTER_STEP_COUNT 5

	float2 stepUV = filterRadiusUV / PCF_FILTER_STEP_COUNT;
		for (float x = -PCF_FILTER_STEP_COUNT; x <= PCF_FILTER_STEP_COUNT; ++x)
			for (float y = -PCF_FILTER_STEP_COUNT; y <= PCF_FILTER_STEP_COUNT; ++y)
			{
				float2 offset = float2(x, y) * stepUV;
					float z = z0 + dot(dz_duv, offset);
				sum += tex2Dproj(sampShadowDepth_Linear, float4(uv + offset, z, 1)).r;
			}
	float numSamples = (PCF_FILTER_STEP_COUNT * 2 + 1);
	return sum / (numSamples*numSamples);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowReceiveInput
{
	float3 Position : POSITION;
};

struct ShadowReceiveOutput
{
	float4 Position    : POSITION;
	float4 ScreenProj  : TEXCOORD0;
	float3 EyeSpacePos : TEXCOORD1;
};

ShadowReceiveOutput ShadowReceiveVS(ShadowReceiveInput In)
{
	ShadowReceiveOutput Out;
	Out.Position = mul(float4(In.Position, 1), camera.ProjTM);

	Out.ScreenProj.xy = Out.Position.xy * float2(0.5f, -0.5f) + (Out.Position.ww * 0.5f);
	Out.ScreenProj.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProj.xy += (Out.ScreenProj.w * 0.5f) * camera.ScreenTexelUV.xy;
#endif

	Out.EyeSpacePos = In.Position;

	return Out;
}



float SampleShadowPCF_NV(float4 shadowCoords, sampler sampShadowDepth, int2 ShadowSize, float4 ShadowTexelUV)
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
	shadowMapVals.x = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.zw, 0, 0)).r;
	shadowMapVals.y = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.xw, 0, 0)).r;
	shadowMapVals.z = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.zy, 0, 0)).r;
	shadowMapVals.w = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.xy, 0, 0)).r;

	float4 shadowMapWeights = weight.zxzx * weight.wwyy;
		return dot(shadowMapVals, shadowMapWeights) * (1.0f / 16);
}


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


float4 ShadowReceiveDynPS(ShadowReceiveOutput In) : COLOR
{
	float3 viewPos = In.EyeSpacePos;
	viewPos.xyz /= viewPos.z;
	viewPos.xyz *= tex2Dproj(sampScreenDepth, In.ScreenProj).r;

	float4 shadowCoordsDyn = mul(float4(viewPos, 1), split.DynamicShadowTexTM);
	//float lightAmountDyn = tex2Dproj(sampShadowDepth_Linear, shadowCoordsDyn).r;
	//float lightAmountDyn = SampleShadowPCF_NV(shadowCoordsDyn, sampShadowDepth_Linear, commonData.DynamicShadowSize.xy, commonData.DynamicShadowTexelUV);

	
	float2 uv = shadowCoordsDyn.xy / shadowCoordsDyn.w;
	float z = shadowCoordsDyn.z / shadowCoordsDyn.w;
	// Compute gradient using ddx/ddy before any branching
	float2 dz_duv = DepthGradient(uv, z);
	float lightAmountDyn = PCF_Filter(uv, z, dz_duv, camera.TexelSize.xy*2);

	//float zEye = mul(In.EyeSpacePos, camera.mLightView).z;
	//float lightAmountDyn = PCSS_Shadow(uv, z, dz_duv, zEye, commonData.TexelSize*1.0);

	float3 wp = mul((float3)viewPos, (float3x3)camera.mCamView);
	//lightAmountDyn = wp.y > 1 ? 1:0;
	//
	float4 outColor;
	outColor.r = lightAmountDyn;
	outColor.gba = 0;
	return outColor;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


technique ShadowReceiveDyn_NV
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowReceiveVS();
		PixelShader = compile ps_3_0 ShadowReceiveDynPS();

		ZEnable = true;
		ZWriteEnable = false;		
		ZFunc = Greater;

		StencilEnable = true;
		StencilFunc = Equal;
		StencilRef = 1;
		StencilPass = Zero;

		AlphaTestEnable = false;
		AlphaBlendEnable = false;
		FillMode = Solid;
		CullMode = NONE;
	}
}
