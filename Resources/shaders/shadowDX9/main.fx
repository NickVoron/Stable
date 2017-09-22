#define D3D9_RASTER_OFFSET

struct PerCamera
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 DirLightDirectionEyeSpace;
	float4 ScreenTexelUV;	
};

PerCamera camera;


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


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct MainGeomInput
{
	float4 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct MainGeomOutput
{
	float4 Position         : POSITION;
	float2 TextureUV        : TEXCOORD0;
	float3 NormalEyeSpace   : TEXCOORD1;
	float4 ScreenProjCoords : TEXCOORD2;
};

MainGeomOutput VS(MainGeomInput In)
{
	MainGeomOutput Out;
	
	float4 posEyeSpace = mul(In.Position, camera.ViewTM);
		float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)camera.ViewTM));
	
	Out.Position       = mul(posEyeSpace, camera.ProjTM);
	Out.TextureUV      = In.TextureUV;
	Out.NormalEyeSpace = normEyeSpace;
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * camera.ScreenTexelUV.xy;
#endif
	
	return Out;
}

void PS(MainGeomOutput In, out float4 outColor : COLOR)
{
	float dotNL = saturate(dot(normalize(In.NormalEyeSpace), camera.DirLightDirectionEyeSpace.xyz));
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	outColor.rgb = float3(1.3, 0.7, 0.5) * (dotNL * shadow) + float3(0.3, 0.3, 0.4);
	outColor.a = 1;
}

technique MainGeom
{
	pass p0
	{
		VertexShader     = compile vs_2_0 VS();
		PixelShader      = compile ps_2_0 PS();
		
		ZWriteEnable     = false;
		ZEnable          = true;

		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode = NONE;
	}
}

