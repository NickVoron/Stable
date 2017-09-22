struct PerCamera
{
	float4x4 ProjTM;
	float4x4 ViewTM;
};

PerCamera camera;

struct DepthGeomInput
{
	float3 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct DepthGeomOutput
{
	float4 Position         : POSITION0;
	float2 TextureUV        : TEXCOORD0;						  
	float4 Norm_Depth       : TEXCOORD1;
};

DepthGeomOutput VS(DepthGeomInput In)
{
	DepthGeomOutput Out;
	
	float4 posEyeSpace = mul(float4(In.Position, 1), camera.ViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)camera.ViewTM));
	
	Out.Position         = mul(posEyeSpace, camera.ProjTM);
	Out.TextureUV        = In.TextureUV;
	Out.Norm_Depth.xyz   = normEyeSpace;
	Out.Norm_Depth.w     = posEyeSpace.z;
	
	return Out;
}

void PS(DepthGeomOutput In, out float4 outDepth : COLOR)
{
	outDepth = float4(In.Norm_Depth.w, 0, 0, 0);
}

technique DepthGeom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 VS();
		PixelShader      = compile ps_2_0 PS();
		
		ZWriteEnable     = true;
		ZEnable          = true;

		StencilEnable    = true;
		StencilRef       = 1;
		StencilFunc      = Always;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;
	}
}