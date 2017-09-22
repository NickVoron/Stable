#include "../common/globalConstants.fx"
float4 lightDir;

struct DepthOutput
{
	float4 Pos					: POSITION0;
	float Depth					: TEXCOORD0;
};

DepthOutput DepthVS(	float3 Pos  : POSITION, float3 Norm : NORMAL)
{
	DepthOutput Out = (DepthOutput)0;

	float4x4 WorldView = mul(World, View);

	float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
	Out.Pos  = mul(float4(P, 1), Projection);
	Out.Depth.x = P.z;

	return Out;
}


void DepthPS(DepthOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.Depth.x, 0, 0, 0);
}

technique XMeshDepth
{
	pass P0
	{
		VertexShader = compile vs_2_0 DepthVS();
		PixelShader  = compile ps_2_0 DepthPS();

		ZWriteEnable     = true;
		ZEnable          = true;

		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;

		FillMode         = Solid;
		CullMode         = NONE;
	}
}














struct ShadowOutput
{
	float4 Pos					: POSITION0;
};

DepthOutput ShadowVS(	float3 Pos  : POSITION, float3 Norm : NORMAL)
{
	DepthOutput Out = (DepthOutput)0;

	float4x4 WorldView = mul(World, View);

	float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
	Out.Pos  = mul(float4(P, 1), Projection);

	Out.Pos.x -= ScreenTexelUV.x * Out.Pos.w;
	Out.Pos.y += ScreenTexelUV.y * Out.Pos.w;

	return Out;
}


void ShadowPS(ShadowOutput In, out float4 outColor : COLOR)
{
	outColor = 0;
}

technique XMeshShadow
{
	pass P0
	{
		VertexShader = compile vs_2_0 ShadowVS();
		PixelShader  = compile ps_2_0 ShadowPS();

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;

		ColorWriteEnable = 0;

		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.00001;
	}
}














struct MainOutput
{
	float4 Pos				: POSITION0;
	float4 Diff				: COLOR0;
	float2 ScreenProjCoords : TEXCOORD0;
	float depth : TEXCOORD1;
};

MainOutput MainVS(	float3 Pos  : POSITION, float3 Norm : NORMAL)
{
	MainOutput Out = (MainOutput)0;

	float4x4 WorldView = mul(World, View);

	float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
	Out.Pos  = mul(float4(P, 1), Projection);

	Out.ScreenProjCoords.xy = (Out.Pos.xy * float2(0.5f, -0.5f) + Out.Pos.ww * 0.5f + Out.Pos.zw * 0.5f * ScreenTexelUV)/Out.Pos.ww;

	//float k = 0.5f/Out.Pos.w;
	//Out.ScreenProjCoords.x =  Out.Pos.x * k + 0.5f;
	//Out.ScreenProjCoords.y = -Out.Pos.y * k + 0.5f;

	float3 N = normalize(mul(Norm, (float3x3)World));
	Out.Diff = dot(N, -normalize(lightDir.xyz) ) + 0.3f;

	Out.depth = P.z;

	return Out;
}


void MainPS(MainOutput In, out float4 outColor : COLOR)
{
	float shadow = lerp(1.0f, tex2D(sampScreenShadow, In.ScreenProjCoords).r, clamp(0.5f*(shadowMaxDepth-In.depth), 0.0f, 1.0f));
	outColor = In.Diff*shadow + 0.3f;
}



technique XMeshMain
{
	pass P0
	{
		VertexShader = compile vs_2_0 MainVS();
		PixelShader  = compile ps_2_0 MainPS();

		ZWriteEnable     = false;
		ZEnable          = true;
		//FillMode         = Solid;
		CullMode         = NONE;
	}
}









struct ReflectionOutput
{
	float4 Pos		: POSITION0;
	float4 Diff		: COLOR0;
	float  Depth	: TEXCOORD0;
};

ReflectionOutput ReflectionVS(	float3 Pos  : POSITION, float3 Norm : NORMAL)
{
	ReflectionOutput Out;

	float4x4 WorldView = mul(World, View);

	float3 P = mul(float4(Pos, 1), (float4x3)WorldView);
	Out.Pos  = mul(float4(P, 1), Projection);

	float3 N = normalize(mul(Norm, (float3x3)World));
	Out.Diff = dot(N, -normalize(lightDir.xyz) ) + 0.3f;

	Out.Depth = Out.Pos.w;
	//Out.Depth = P.z;

	return Out;
}


void ReflectionPS(ReflectionOutput In, out float4 outColor : COLOR, out float4 depth : COLOR1)
{
	outColor = In.Diff + 0.3f;
	depth = In.Depth;
}



technique XMeshReflection
{
	pass P0
	{
		VertexShader = compile vs_2_0 ReflectionVS();
		PixelShader  = compile ps_2_0 ReflectionPS();

		ZEnable          = true;
		FillMode         = Solid;
		CullMode         = CCW;
	}
}
