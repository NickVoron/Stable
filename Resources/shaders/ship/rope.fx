#include "../material/common.fx"
#define D3D9_RASTER_OFFSET

int ObjType = 1;

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
float3 dirLightDirection;
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

float4 sunLightColor()
{
	return float4(0.7f, 0.7f, 0.7f, 1.0f);
}


float4 lightAmbient()
{
	return float4(0.3f, 0.3f, 0.3f, 1.0f);
}

float4 CamPos;
float width;

texture ropeTex;
sampler2D ropeTexSam = sampler_state 
{
	Texture = <ropeTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_ROPE_OUTPUT
{
	float4 Position : POSITION;
	float2 uv : TEXCOORD0;
};

VS_ROPE_OUTPUT ropeVS( float3 vPos : POSITION, float3 pos0 : TEXCOORD0, float3 pos1 : TEXCOORD1)
{
	VS_ROPE_OUTPUT Output;
	//float3 pos0 = tex2D(idxSam, float2(uv.x, 0.0f)).xyz;
	//float3 pos1 = tex2D(idxSam, float2(uv.y, 0.0f)).xyz;

	float3 vX = normalize(pos1.xyz-pos0.xyz);
	float3 vZ = normalize( CamPos.xyz - (dot( CamPos.xyz-pos0.xyz, vX) * vX + pos0.xyz) );
	float3 vY = cross( vX, vZ );
	float3x3 rot = float3x3(vX, vY, vZ);

	float l = length(pos1.xyz-pos0.xyz);
	float3 pp = float3(vPos.x*l, vPos.y, 0.0f);
	float3 p = mul( pp, rot );
	p += pos0.xyz;

	float4x4 viewProjection = mul(View, Proj);
	Output.Position = mul(float4(p, 1), viewProjection);

	Output.uv = float2(vPos.x * l * 5, vPos.y / (width * 2.0f) + 0.5f);

	return Output;    
}


struct PS_ROPE_OUTPUT
{
	float4 color : COLOR0;
};

PS_ROPE_OUTPUT ropePS( VS_ROPE_OUTPUT In ) 
{ 
	PS_ROPE_OUTPUT Output;
	Output.color = tex2D(ropeTexSam, In.uv) * (sunLightColor() + lightAmbient());
	return Output;
}

technique RenderRope
{
	pass P0
	{          
		VertexShader = compile vs_3_0 ropeVS();
		PixelShader  = compile ps_3_0 ropePS(); 
		//FillMode = Wireframe;
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaRef = 128;
		AlphaFunc = Greater;
		CullMode = None;        
	}
}


