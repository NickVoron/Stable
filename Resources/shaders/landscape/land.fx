float4 sunLightColor() { return float4(0.7f, 0.7f, 0.7f, 1.0f); }
float4 lightAmbient() { return float4(0.3f, 0.3f, 0.3f, 1.0f); }
float3 getAtmosphereColor(float3 pos){ return float4(0.5f, 0.5f, 0.5f, 1.0f); }

#define USE_VERTEX_TEXTURE

const float fog_density = 0.8f;
const float2 minMaxDist = float2(0, 1024);
const float2 lfStartEnd = float2(-200, 100);

float3 dirLightDirection;

int ObjType = 1;
float2 ScreenTexelUV;

texture ScreenShadowMap;

sampler sampScreenShadow = sampler_state
{
	texture = <ScreenShadowMap>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
//float shadowMaxDepth;

#include "../fog/fog.fx"
#include "../material/common.fx"
#include "../landscape/landscapeNew.fx"