float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float useLightmap;

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

texture lightmapTex;
sampler2D lightmapSam = sampler_state 
{
	Texture = <lightmapTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 DiffuseUV : TEXCOORD0;
	float2 LightmapUV : TEXCOORD1;
};

//
//
struct VS_FINAL
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 DiffuseUV : TEXCOORD0;
	float2 LightmapUV : TEXCOORD1;
};
VS_FINAL FinalVS( VS_INPUT In )
{
	VS_FINAL Out;
	Out.Position = mul(float4(In.Position, 1), mul( mul(World, View), Projection) );
	Out.Normal = In.Normal;
	Out.DiffuseUV = In.DiffuseUV;
	Out.LightmapUV = In.LightmapUV;
	return Out;
}




struct PS_FINAL
{
	float4 color : COLOR0;
};
PS_FINAL FinalPS( VS_FINAL In ) 
{ 
	PS_FINAL Out;
	Out.color = 0.9f + 0.1f*tex2D(diffuseSam, In.DiffuseUV);
	if(useLightmap>0.0f)	Out.color *= tex2D(lightmapSam, In.LightmapUV);
	return Out;
}
technique Final
{
	pass P0
	{          
		VertexShader = compile vs_3_0 FinalVS();
		PixelShader  = compile ps_3_0 FinalPS(); 
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		CullMode = CCW;
	}
}
