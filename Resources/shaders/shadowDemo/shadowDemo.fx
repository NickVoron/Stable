float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float2 ScreenTexelUV;
int MovementType;

float3 SunDir = float3(1.0f, 0.5f, 1.0f);

//textures
texture diffuseTex;
sampler2D diffuseSam = sampler_state 
{
	Texture = <diffuseTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

texture shadowTex;
sampler2D shadowSam = sampler_state 
{
	Texture = <shadowTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_INPUT
{
	float3 vPos : POSITION;
	float3 vNorm : NORMAL;
	float2 uvDiffuse : TEXCOORD0;
};



//
//DEPTH
//
struct VS_DEPTH
{
	float4 Position : POSITION;
	float  Depth : TEXCOORD0;
};

struct PS_DEPTH
{
	float4 color : COLOR0;
};

VS_DEPTH DepthVS( VS_INPUT In)
{
	VS_DEPTH Out;

	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(In.vPos, 1), worldView);
	Out.Depth = pos.z;
	Out.Position = mul(pos, Projection);

	return Out;
}

PS_DEPTH DepthPS( VS_DEPTH In ) 
{ 
	PS_DEPTH Out;
	Out.color = float4(In.Depth.x, 0, 0, 1.0);
	return Out;
}

technique Depth
{
	pass P0
	{          
		VertexShader = compile vs_3_0 DepthVS();
		PixelShader  = compile ps_3_0 DepthPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		StencilEnable    = true;
		StencilRef       = <MovementType>;
		StencilPass      = Replace;
		CullMode = NONE;
	}
}


//
//SHADOW
//
struct VS_SHADOW
{
	float4 Position : POSITION;
};

struct PS_SHADOW
{
	float4 color : COLOR0;
};

VS_SHADOW ShadowVS( VS_INPUT In)
{
	VS_SHADOW Out;

	float3 vPos = In.vPos;// - normalize(In.vNorm)*fHeightMapRange;
	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(vPos, 1), worldView);
	Out.Position = mul(pos, Projection);
	Out.Position.x -= ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += ScreenTexelUV.y * Out.Position.w;
	return Out;
}

PS_SHADOW ShadowPS( VS_SHADOW In ) 
{ 
	PS_SHADOW Out;
	Out.color = float4(0,0,0, 1);
	return Out;
}

technique Shadow
{
	pass P0
	{          
		VertexShader = compile vs_3_0 ShadowVS();
		PixelShader  = compile ps_3_0 ShadowPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = NONE;
		SlopeScaleDepthBias = 1.001;
		DepthBias           = 0.00001;
	}
}





//
//FINAL
//
struct VS_FINAL
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Diffuse : TEXCOORD0;
	float4 ShadowUV : TEXCOORD1;
};

struct PS_FINAL
{
	float4 color : COLOR0;
};

VS_FINAL FinalVS( VS_INPUT In )
{
	VS_FINAL Out;

	float4x4 worldView = mul(World, View);
	float4 pos = mul(float4(In.vPos, 1), worldView);
	Out.Position = mul(pos, Projection);

	Out.ShadowUV.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ShadowUV.zw = Out.Position.zw;
	Out.ShadowUV.xy += Out.ShadowUV.ww * 0.5f * ScreenTexelUV;

	Out.Diffuse = In.uvDiffuse;
	Out.Normal = In.vNorm;

	return Out;
}


PS_FINAL FinalPS( VS_FINAL In ) 
{ 
	PS_FINAL Out;
	
	float shadow = tex2Dproj(shadowSam, In.ShadowUV);
	float3 diffuseColor = tex2D(diffuseSam, In.Diffuse);
	float nDotL = dot(In.Normal, normalize(SunDir));
	Out.color.rgb = shadow;
	//Out.color.rg = In.ShadowUV;
	//Out.color.rgb *= diffuseColor;
	Out.color.a = 1;
	return Out;
}


technique Final
{
	pass P0
	{          
		VertexShader = compile vs_3_0 FinalVS();
		PixelShader  = compile ps_3_0 FinalPS(); 
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = NONE;
	}
}
