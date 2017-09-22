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

texture sailDiffTex;
sampler2D sailDiffSam = sampler_state
{
	Texture = <sailDiffTex>;
	MaxAnisotropy = 16;
	MinFilter = ANISOTROPIC;
	MipFilter = LINEAR;
	MagFilter = LINEAR;
};

texture sailNormTex;
sampler2D sailNormSam = sampler_state
{
	Texture = <sailNormTex>;
	MaxAnisotropy = 16;
	MinFilter = ANISOTROPIC;
	MipFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_SAILS_DEPTH
{
	float4 Position : POSITION;
	float2 Tex		: TEXCOORD0;
	float Depth		: TEXCOORD4;
};

VS_SAILS_DEPTH VerletSailVSDepth(float4 vPos : POSITION,
								 float2 vTex : TEXCOORD0,
								 float3 vNorm : NORMAL,
								 float3 vBiNorm : BINORMAL,
								 float3 vTan : TANGENT  )
{
	VS_SAILS_DEPTH Output;
	float4x4 worldView = mul(World, View);
	float4 pos = mul(vPos, worldView);

	Output.Position = mul(pos, Proj);
	Output.Tex = vTex;
	Output.Depth = pos.z;
	return Output;    
}

void SailPSDepth(VS_SAILS_DEPTH In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.Depth, 0, 0, 0);
}


struct VS_SAILS_SHADOW
{
	float4 Position : POSITION;
};
VS_SAILS_SHADOW VerletSailVSShadow( float4 vPos : POSITION,
														 float2 vTex : TEXCOORD0,
														 float3 vNorm : NORMAL,
														 float3 vBiNorm : BINORMAL,
														 float3 vTan : TANGENT  )
{
	VS_SAILS_SHADOW Output;
	float4x4 worldView = mul(World, View);
	float4 pos = mul(vPos, worldView);

	Output.Position = mul(pos, Proj);
	Output.Position.x -= ScreenTexelUV.x * Output.Position.w;
	Output.Position.y += ScreenTexelUV.y * Output.Position.w;
	return Output;    
}

void SailPSShadow(VS_SAILS_SHADOW In, out float4 outColor : COLOR)
{
	outColor = 0;
}

struct VS_SAILS_OUTPUT
{
	float4 Position : POSITION;
	float2 Tex		: TEXCOORD0;
	float3 Norm		: TEXCOORD1;
	float3 BiNorm	: TEXCOORD2;
	float3 Tan		: TEXCOORD3;
	float Depth		: TEXCOORD4;
	float4 ScreenProjCoords : TEXCOORD5;
};

//for verlet sail
VS_SAILS_OUTPUT VerletSailVS( float4 vPos : POSITION,
							  float2 vTex : TEXCOORD0,
							  float3 vNorm : NORMAL,
							  float3 vBiNorm : BINORMAL,
							  float3 vTan : TANGENT  )
{
	VS_SAILS_OUTPUT Output;
	float4x4 worldView = mul(World, View);
	float4 pos = mul(vPos, worldView);

	Output.Position = mul(pos, Proj);
	Output.Tex = vTex;
	Output.Norm = vNorm;
	//if(dot(vNorm, float3(View[0][2], View[1][2], View[2][2])) > 0)
	//{
	//  Output.Norm = -Output.Norm;
	//}
	Output.BiNorm = vBiNorm;
	Output.Tan = vTan; 
	Output.Depth = pos.z;
	Output.ScreenProjCoords.xy = Output.Position.xy * float2(0.5f, -0.5f) + Output.Position.ww * 0.5f;
	Output.ScreenProjCoords.zw = Output.Position.zw;
	Output.ScreenProjCoords.xy += Output.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
	return Output;    
}


struct PS_SAIL_OUTPUT
{
	float4 color : COLOR0;
};

PS_SAIL_OUTPUT SailPS( VS_SAILS_OUTPUT In )
{ 
	PS_SAIL_OUTPUT Output;
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float nDotL = abs(dot( normalize(In.Norm), dirLightDirection ));
	float4 l = saturate( shadow * nDotL * sunLightColor() + lightAmbient());
	Output.color = l * tex2D(sailDiffSam, In.Tex);
	//Output.color.xyz = abs(In.BiNorm);

	return Output;
}

PS_SAIL_OUTPUT SailWOShadowPS( VS_SAILS_OUTPUT In )
{ 
	PS_SAIL_OUTPUT Output;
	float4 l = saturate( abs(dot( normalize(In.Norm), sunDir )) ) * 0.7f + float4(0.3f, 0.3f, 0.3f, 1.0f);
	Output.color = l * tex2D(sailDiffSam, In.Tex);
	//Output.color = l;
	return Output;
}

technique DepthGeom
{
	pass P0
	{          
		VertexShader = compile vs_3_0 VerletSailVSDepth();
		PixelShader  = compile ps_3_0 SailPSDepth(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;
	}
}


technique ShadowGeom
{
	pass P0
	{          
		VertexShader = compile vs_3_0 VerletSailVSShadow();
		PixelShader  = compile ps_3_0 SailPSShadow(); 
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.0; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.00001;
	}
}

technique MainGeom
{
	pass P0
	{          
		VertexShader = compile vs_3_0 VerletSailVSDepth();
		PixelShader  = compile ps_3_0 SailPS(); 
				
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;

		//ColorWriteEnable = 1;

		AlphaRef = 100;
		AlphaFunc = Greater;      
	}
}

technique MainGeomDepth
{
	pass P0
	{          
		VertexShader = compile vs_3_0 VerletSailVSDepth();
		PixelShader  = compile ps_3_0 SailPS(); 
				
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;

		//ColorWriteEnable = 1;

		AlphaRef = 100;
		AlphaFunc = Greater;      
	}
}

/*
technique RenderSails
{
	pass P0
	{          
		VertexShader = compile vs_3_0 VerletSailVS();
		PixelShader  = compile ps_3_0 SailWOShadowPS(); 
		ZWriteEnable     = true;
		CullMode = None;  
		//FillMode = wireframe;       
	}
}*/
