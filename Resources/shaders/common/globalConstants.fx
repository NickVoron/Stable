#define VS_COMPILE_COMMAND VertexShader = compile vs_3_0
#define PS_COMPILE_COMMAND PixelShader = compile ps_3_0

#define D3D9_RASTER_OFFSET
#define USE_VERTEX_TEXTURE

int ObjType = 1;

float3 sunLightDir;

const float fog_density = 0.8f;

const float2 minMaxDist = float2(0, 1024);
const float2 lfStartEnd = float2(-200, 100);

// common inp:
float3 cameraPosition;
float3 cameraDirection;

// landscape constants lod:
//float4 lodCornerGamma = float4(0,0, 0,0); // lod-switch (gamma) effect at points: (0,0), (1,0), (0,1), (1,1)
//float4 composedOriginAndRenderSize = float4(0, 0, 0.001, 0.001); // (x,y) -> minCornerFlat, (z,w) -> sizeFlat INVERTED;

float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
//float4x4 WorldViewProj;
float4x4 WorldIT;
float4x4 WorldView;

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
float shadowMaxDepth;