#define D3D9_RASTER_OFFSET

struct PerCamera
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 ScreenTexelUV;
};

PerCamera camera;


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct ShadowGeomInput
{
	float3 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct ShadowGeomOutput
{
	float4 Position    : POSITION0;
	float2 TextureUV   : TEXCOORD0;
	float Norm_Depth   : TEXCOORD1;
};

ShadowGeomOutput VS(ShadowGeomInput In)
{
	ShadowGeomOutput Out;
	
	float4 eyeSpacePos = mul(float4(In.Position, 1), camera.ViewTM);
	
	Out.Position = mul(eyeSpacePos, camera.ProjTM);
#ifdef D3D9_RASTER_OFFSET
	Out.Position.x -= camera.ScreenTexelUV.x * Out.Position.w;
	Out.Position.y += camera.ScreenTexelUV.y * Out.Position.w;
#endif
	Out.TextureUV = In.TextureUV;
	
	Out.Norm_Depth     = eyeSpacePos.z;
	return Out;
}

void PS(ShadowGeomOutput In, out float4 outColor : COLOR) 
{
	outColor = float4(In.Norm_Depth, 0, 0, 0);
}


technique ShadowGeom
{
	pass p0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader  = compile ps_2_0 PS();
		
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode = NONE;
		
		//ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.0;
		DepthBias           = 0.00001;
	}
}