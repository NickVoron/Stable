//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

#define D3D9_RASTER_OFFSET


float2 ScreenTexelUV;

int2 StaticShadowSize;
int2 DynamicShadowSize;

float4 StaticShadowTexelUV; //zw = -xy
float4 DynamicShadowTexelUV; //zw = -xy


float4x4 ProjTM;
float4x4 ViewToLastViewProjTexTM;

float4x4 StaticShadowTexTM;
float4x4 DynamicShadowTexTM;


texture ScreenDepthMap;
texture ScreenLastDepthMap;
texture ScreenLastShadowMap;

texture ShadowColorMap;
texture StaticShadowDepthMap;
texture DynamicShadowDepthMap;

sampler sampScreenDepth = sampler_state
{
	texture = <ScreenDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampScreenLastDepth = sampler_state
{
	texture = <ScreenLastDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampScreenLastShadow = sampler_state 
{
	texture = <ScreenLastShadowMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

sampler sampStaticShadowDepth_Point : register(s0) = sampler_state 
{
	texture = <StaticShadowDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampStaticShadowDepth_Linear : register(s0) = sampler_state 
{
	texture = <StaticShadowDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	//Linear for NVidia hardware PCF
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampDynamicShadowDepth_Point : register(s1) = sampler_state 
{
	texture = <DynamicShadowDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampDynamicShadowDepth_Linear : register(s1) = sampler_state 
{
	texture = <DynamicShadowDepthMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	//Linear for NVidia hardware PCF
	MinFilter = Linear;
	MagFilter = Linear;
};

/*
texture ScreenObjTypeMap;

sampler sampScreenObjType = sampler_state
{
	texture = <ScreenObjTypeMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
*/

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowReceiveInput
{
	float4 Position : POSITION;
};

struct ShadowReceiveOutput
{
	float4 Position    : POSITION;
	float4 ScreenProj  : TEXCOORD0;
	float4 EyeSpacePos : TEXCOORD1;
};

ShadowReceiveOutput ShadowReceiveVS(ShadowReceiveInput In)
{
//*
	ShadowReceiveOutput Out;
	Out.Position = mul(In.Position, ProjTM);
	
	Out.ScreenProj.xy = Out.Position.xy * float2(0.5f, -0.5f) + (Out.Position.ww * 0.5f);
	Out.ScreenProj.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProj.xy += (Out.ScreenProj.w * 0.5f) * ScreenTexelUV.xy;
#endif
	
	Out.EyeSpacePos = In.Position;
	
	return Out;
/*/
	ShadowReceiveOutput Out;
	Out.Position = In.Position;
	
	Out.ScreenProj.xy = Out.Position.xy * float2(0.5f, -0.5f) + (Out.Position.ww * 0.5f);
	Out.ScreenProj.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProj.xy += (Out.ScreenProj.w * 0.5f) * ScreenTexelUV.xy;
#endif
	
	Out.EyeSpacePos = float4(In.Position.xy * float2(1.0f / ProjTM[0][0], 1.0f / ProjTM[1][1]), 1.0f, 1.0f);
	
	return Out;
//*/
}

float2 EvalDepthGradient(float4 shadowCoords)
{
	float3 projCoords = shadowCoords.xyz / shadowCoords.w;

	float2 ddist_duv;
	//Packing derivatives of u,v, and distance to light source w.r.t. screen space x, and y
	float3 duvdist_dx = ddx(projCoords);
	float3 duvdist_dy = ddy(projCoords);
	//Invert texture Jacobian and use chain rule to compute ddist/du and ddist/dv
	// |ddist/du| = |du/dx du/dy|-T * |ddist/dx|
	// |ddist/dv| |dv/dx dv/dy| |ddist/dy|
	//Multiply ddist/dx and ddist/dy by inverse transpose of Jacobian
	float invDet = 1 / ((duvdist_dx.x * duvdist_dy.y) - (duvdist_dx.y * duvdist_dy.x) );
	//Top row of 2x2
	ddist_duv.x = duvdist_dy.y * duvdist_dx.z ; // invJtrans[0][0] * ddist_dx
	ddist_duv.x -= duvdist_dx.y * duvdist_dy.z ; // invJtrans[0][1] * ddist_dy
	//Bottom row of 2x2
	ddist_duv.y = duvdist_dx.x * duvdist_dy.z ; // invJtrans[1][1] * ddist_dy
	ddist_duv.y -= duvdist_dy.x * duvdist_dx.z ; // invJtrans[1][0] * ddist_dx
	ddist_duv *= invDet;

	return ddist_duv;
}

float SampleShadowPCF3x3_NV(uniform bool bias, float4 shadowCoords, sampler sampShadowDepth, int2 ShadowSize, float4 ShadowTexelUV)
{
	float2 shadowUV = shadowCoords.xy / shadowCoords.w;

	float4 offset;
	offset.xy = frac(shadowUV * ShadowSize - 0.5f);
	offset.zw = 1 - offset.xy;
	
	float4 weight = 1 + offset * 2;
	
	float4 offsetUV = offset.zwxy + (offset / weight);
	offsetUV *= ShadowTexelUV;
	offsetUV *= shadowCoords.w;
	
	float2 ddist_duv = bias ? EvalDepthGradient(shadowCoords) : 0.0f;
	float4 shadowUVOffset = (offset.zwxy + 0.5f) * ShadowTexelUV;

	
	float4 shadowMapVals;
	shadowMapVals.x = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.zw, dot(ddist_duv, shadowUVOffset.zw)*shadowCoords.w, 0)).r;
	shadowMapVals.y = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.xw, dot(ddist_duv, shadowUVOffset.xw)*shadowCoords.w, 0)).r;
	shadowMapVals.z = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.zy, dot(ddist_duv, shadowUVOffset.zy)*shadowCoords.w, 0)).r;
	shadowMapVals.w = tex2Dproj(sampShadowDepth, shadowCoords + float4(offsetUV.xy, dot(ddist_duv, shadowUVOffset.xy)*shadowCoords.w, 0)).r;

	float4 shadowMapWeights = weight.zxzx * weight.wwyy;
	
	return dot(shadowMapVals, shadowMapWeights) * (1.0f / 16);
}

float SampleShadowPCF_NV(uniform bool bias, float4 shadowCoords, sampler sampShadowDepth, int2 ShadowSize, float4 ShadowTexelUV)
{
	return SampleShadowPCF3x3_NV(bias, shadowCoords, sampShadowDepth, ShadowSize, ShadowTexelUV);

	//NV - hardware PCF when using Linear texture filtering
	return tex2Dproj(sampShadowDepth, shadowCoords).r;
}

float SampleShadowPCF3x3_ATI(uniform bool bias, float4 shadowCoords, sampler sampShadowDepth, int2 ShadowSize, float4 ShadowTexelUV)
{
	float3 projCoords = shadowCoords.xyz / shadowCoords.w;
	float2 shadowUV = projCoords.xy;
	float shadowDepth = projCoords.z;

	float4 offset;
	offset.xy = frac(shadowUV * ShadowSize - 0.5f);
	offset.zw = 1 - offset.xy;
	
	shadowUV -= offset.xy * ShadowTexelUV.xy;
	
	float4 shadowUV4 = shadowUV.xyxy + ShadowTexelUV;
	
	float4 shadowMapVals[4];
	shadowMapVals[0] = tex2D(sampShadowDepth, shadowUV4.zw);
	shadowMapVals[1] = tex2D(sampShadowDepth, shadowUV4.xw);
	shadowMapVals[2] = tex2D(sampShadowDepth, shadowUV4.zy);
	shadowMapVals[3] = tex2D(sampShadowDepth, shadowUV4.xy);

	const float4 weightsAdd = { 0, 1, 1, 0 };
	float4 weightsX = offset.zzxx + weightsAdd;
	float4 weightsY = offset.wwyy + weightsAdd;
	
	float4 shadowMapWeights[4];
	shadowMapWeights[0] = weightsX.yxyx * weightsY.xyyx;
	shadowMapWeights[1] = weightsX.wzwz * weightsY.xyyx;
	shadowMapWeights[2] = weightsX.yxyx * weightsY.zwwz;
	shadowMapWeights[3] = weightsX.wzwz * weightsY.zwwz;

	float2 ddist_duv = bias ? EvalDepthGradient(shadowCoords) : 0.0f;
	float4 shadowUVOffset = (offset.zwxy + 0.5f) * ShadowTexelUV;
	
	float percentInLight;
	percentInLight  = dot(step(dot(ddist_duv, shadowUVOffset.zw) + shadowDepth, shadowMapVals[0]), shadowMapWeights[0]);
	percentInLight += dot(step(dot(ddist_duv, shadowUVOffset.xw) + shadowDepth, shadowMapVals[1]), shadowMapWeights[1]);
	percentInLight += dot(step(dot(ddist_duv, shadowUVOffset.zy) + shadowDepth, shadowMapVals[2]), shadowMapWeights[2]);
	percentInLight += dot(step(dot(ddist_duv, shadowUVOffset.xy) + shadowDepth, shadowMapVals[3]), shadowMapWeights[3]);
	
	return percentInLight * (1.0f / 16);
}

float SampleShadowPCF_ATI(uniform bool bias, float4 shadowCoords, sampler sampShadowDepth, int2 ShadowSize, float4 ShadowTexelUV)
{
	return SampleShadowPCF3x3_ATI(bias, shadowCoords, sampShadowDepth, ShadowSize, ShadowTexelUV);
	
	float2 shadowUV = shadowCoords.xy / shadowCoords.w;
	float shadowDepth = shadowCoords.z / shadowCoords.w;
	
	float4 lerpOffset;
	lerpOffset.xy = frac(shadowUV * ShadowSize - 0.5f);
	lerpOffset.zw = 1 - lerpOffset.xy;
	
	shadowUV -= lerpOffset.xy * ShadowTexelUV.xy;
	
	float4 texDepth = tex2D(sampShadowDepth, shadowUV);
	return dot(step(shadowDepth, texDepth), lerpOffset.xzxz * lerpOffset.wyyw);
}

float SampleStaticShadow(uniform bool hasATI, float4 viewPos, out float shadowConf)
{
	float4 shadowCoords = mul(viewPos, StaticShadowTexTM);

	float2 shadowUV = shadowCoords.xy / shadowCoords.w;
	float shadowDepth = shadowCoords.z / shadowCoords.w;
	
	float2 shadowOffset = frac(shadowUV * StaticShadowSize) - 0.5f;
	shadowConf = 1.0f - max(abs(shadowOffset.x), abs(shadowOffset.y)) * 2.0f;  

	if (hasATI)
	{	//ATI
		float texDepth = tex2D(sampStaticShadowDepth_Point, shadowUV).r;
		return step(shadowDepth, texDepth);
	}
	else
	{	//NV
		return tex2Dproj(sampStaticShadowDepth_Point, shadowCoords).r;
	}
}

float4 ShadowReceivePS(uniform bool hasATI, ShadowReceiveOutput In) : COLOR
{
	float4 viewPos = In.EyeSpacePos;
	viewPos.xyz /= viewPos.z;
	viewPos.xyz *= tex2Dproj(sampScreenDepth, In.ScreenProj).r;

	float4 shadowCoordsDyn = mul(viewPos, DynamicShadowTexTM);
	//if (!(all(shadowCoordsDyn.xy >= 0) && all(shadowCoordsDyn.xy <= shadowCoordsDyn.ww))) discard;
	
	float shadowConf;
	float lightAmount = SampleStaticShadow(hasATI, viewPos, shadowConf);
	//shadowConf = 0;

	float lightAmountDyn = hasATI
		? SampleShadowPCF_ATI(false, shadowCoordsDyn, sampDynamicShadowDepth_Point, DynamicShadowSize, DynamicShadowTexelUV)
		: SampleShadowPCF_NV(false, shadowCoordsDyn, sampDynamicShadowDepth_Linear, DynamicShadowSize, DynamicShadowTexelUV);
//*
	float4 lastViewProjCoords = mul(viewPos, ViewToLastViewProjTexTM);
	bool bInFrustrum = all(lastViewProjCoords.xyz >= 0) && all(lastViewProjCoords.xyz <= lastViewProjCoords.www);
	if (bInFrustrum)
	{
		float4 lastScreenProj = lastViewProjCoords;
#ifdef D3D9_RASTER_OFFSET
		lastScreenProj.xy += (lastScreenProj.w * 0.5f) * ScreenTexelUV.xy;
#endif

		float depth_Last = tex2Dproj(sampScreenLastDepth, lastScreenProj).r;
		if (lastViewProjCoords.w < depth_Last + 0.1f)
		{
			float4 shadowVal_Last = tex2Dproj(sampScreenLastShadow, lastScreenProj);
			float lightAmount_Last = shadowVal_Last.g;
			float shadowConf_Last = shadowVal_Last.b;
			
			float weight = pow(shadowConf, lerp(0.0f, 5.0f, shadowConf_Last));
		
			lightAmount = lerp(lightAmount_Last, lightAmount, weight);
			shadowConf = lerp(shadowConf_Last, shadowConf, weight);
		}
	}
//*/

	float4 outColor;
	outColor.r = min(lightAmount, lightAmountDyn);
	outColor.g = lightAmount;
	outColor.b = shadowConf;
	outColor.a = 0.0f;
	return outColor;
}


float4 ShadowReceiveDynPS(uniform bool hasATI, ShadowReceiveOutput In) : COLOR
{
	float4 viewPos = In.EyeSpacePos;
	viewPos.xyz /= viewPos.z;
	viewPos.xyz *= tex2Dproj(sampScreenDepth, In.ScreenProj).r;

	float4 shadowCoordsDyn = mul(viewPos, DynamicShadowTexTM);
	//if (!(all(shadowCoordsDyn.xy >= 0) && all(shadowCoordsDyn.xy <= shadowCoordsDyn.ww))) discard;
	
	float lightAmount = hasATI
		? SampleShadowPCF_ATI(false, mul(viewPos, StaticShadowTexTM), sampStaticShadowDepth_Point, StaticShadowSize, StaticShadowTexelUV)
		: SampleShadowPCF_NV(false, mul(viewPos, StaticShadowTexTM), sampStaticShadowDepth_Linear, StaticShadowSize, StaticShadowTexelUV);

	float lightAmountDyn = hasATI
		? SampleShadowPCF_ATI(true, shadowCoordsDyn, sampDynamicShadowDepth_Point, DynamicShadowSize, DynamicShadowTexelUV)
		: SampleShadowPCF_NV(true, shadowCoordsDyn, sampDynamicShadowDepth_Linear, DynamicShadowSize, DynamicShadowTexelUV);

	float4 outColor;
	outColor.r = min(lightAmount, lightAmountDyn);
	outColor.g = lightAmount;
	outColor.b = 0.0f;
	outColor.a = 0.0f;
	return outColor;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowFilterInput
{
	float4 Position : POSITION;
};

struct ShadowFilterOutput
{
	float4 Position    : POSITION;
	float2 ScreenUV0   : TEXCOORD0;
	float2 ScreenUV1   : TEXCOORD1;
	float2 ScreenUV2   : TEXCOORD2;
	float2 ScreenUV3   : TEXCOORD3;
};

ShadowFilterOutput ShadowFilterVS(ShadowFilterInput In)
{
	ShadowFilterOutput Out;
	Out.Position = In.Position;
	
	float2 screenUV = (In.Position.xy + 1.0f) * 0.5f;
	screenUV.y = 1.0f - screenUV.y;
#ifdef D3D9_RASTER_OFFSET
	screenUV += ScreenTexelUV * 0.5f;
#endif
	const float4 offsetUV = float4(ScreenTexelUV.xy, -ScreenTexelUV.xy) * 0.5f;
	Out.ScreenUV0 = screenUV + offsetUV.zw;
	Out.ScreenUV1 = screenUV + offsetUV.xw;
	Out.ScreenUV2 = screenUV + offsetUV.zy;
	Out.ScreenUV3 = screenUV + offsetUV.xy;
	return Out;
}

float4 ShadowFilterPS(ShadowFilterOutput In) : COLOR
{
	float4 res;
	res  = tex2D(sampScreenLastShadow, In.ScreenUV0);
	res += tex2D(sampScreenLastShadow, In.ScreenUV1);
	res += tex2D(sampScreenLastShadow, In.ScreenUV2);
	res += tex2D(sampScreenLastShadow, In.ScreenUV3);
	return res * (1.0f / 4);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

technique ShadowReceive_ATI
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowReceiveVS();
		PixelShader  = compile ps_3_0 ShadowReceivePS(true);
		
		ZWriteEnable     = false;
		ZEnable          = true;
		ZFunc            = Greater;
		
		StencilEnable    = true;
		StencilFunc      = Equal;
		StencilRef       = 1;
		StencilFail      = Keep;
		StencilZFail     = Keep;
		StencilPass      = Zero;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CW;
	}
}

technique ShadowReceiveDyn_ATI
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowReceiveVS();
		PixelShader  = compile ps_3_0 ShadowReceiveDynPS(true);
		
		ZWriteEnable     = false;
		ZEnable          = true;
		ZFunc            = Greater;
		
		StencilEnable    = true;
		StencilFunc      = Equal;
		StencilRef       = 2;
		StencilFail      = Keep;
		StencilZFail     = Keep;
		StencilPass      = Zero;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CW;
	}
}

technique ShadowReceive_NV
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowReceiveVS();
		PixelShader  = compile ps_3_0 ShadowReceivePS(false);
		
		ZWriteEnable     = false;
		ZEnable          = true;
		ZFunc            = Greater;
		
		StencilEnable    = true;
		StencilFunc      = Equal;
		StencilRef       = 1;
		StencilFail      = Keep;
		StencilZFail     = Keep;
		StencilPass      = Zero;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CW;
	}
}

technique ShadowReceiveDyn_NV
{
	pass p0
	{
		VertexShader = compile vs_3_0 ShadowReceiveVS();
		PixelShader  = compile ps_3_0 ShadowReceiveDynPS(false);
		
		ZWriteEnable     = false;
		ZEnable          = true;
		ZFunc            = Greater;
		
		StencilEnable    = true;
		StencilFunc      = Equal;
		StencilRef       = 2;
		StencilFail      = Keep;
		StencilZFail     = Keep;
		StencilPass      = Zero;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CW;
	}
}

technique ShadowFilter
{
	pass p0
	{
		VertexShader = compile vs_2_0 ShadowFilterVS();
		PixelShader  = compile ps_2_0 ShadowFilterPS();
		
		ZWriteEnable     = false;
		ZEnable          = false;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
	}
}