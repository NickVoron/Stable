#include "../sky/common.fx"

float4x4	mC2W;    // inverse matrix of the transform world to projection 
float		invFadeHeight;
float		mieRadius;

struct SVSInput 
{
	float4 vPosC : POSITION;
	float2 vTex  : TEXCOORD0;
};

struct SVSOutput 
{
	float4 vPos		: POSITION0;
	float4 vWorldPos : TEXCOORD0;
	float2 vTex      : TEXCOORD1;
};


// vertex shader
void mainVS( out SVSOutput _Output, in SVSInput _Input )
{
	// Output projection position
	_Output.vPos = _Input.vPosC;
	
	// world positon
	_Output.vWorldPos = mul( _Input.vPosC, mC2W );

	// uv 
	_Output.vTex = _Input.vTex;
}

// pixel shader parameters
sampler sDensity : register(s0);  // density map
sampler sLit : register(s1);      // blurred densitymap
sampler sSky : register(s2);      // sky

float4 mainPS(in SVSOutput _Input) : COLOR0
{
	// compute ray direction	
	float3 _vWorldPos = _Input.vWorldPos.xyz/_Input.vWorldPos.w;

	float4 _clDensity = tex2D( sDensity, _Input.vTex.xy );
	float4 _clLit     = 1.0f - tex2D( sLit, _Input.vTex.xy );

	// light cloud
	float3 cc = getCloudsColor(sSky, _vWorldPos, mieRadius);
	float3 _clCloud = vAmbColor+cc*0.5f + _clLit.r * vSunColor;

	// apply scattering 
	float4 _color;
	_color.rgb = lerp(_clCloud, getAtmosphereColor(sSky, _vWorldPos), 1 -_clDensity.a);
	_color.a = lerp(0.0f, _clDensity.a, _vWorldPos.y * invFadeHeight);
	return _color;
}
														
technique RenderTech
{
	pass Pass0
	{
    	VertexShader = compile vs_3_0 mainVS();
    	PixelShader = compile ps_3_0 mainPS();
	}
}


