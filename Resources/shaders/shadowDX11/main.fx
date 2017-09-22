#define D3D9_RASTER_OFFSET

cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 DirLightDirectionEyeSpace;
	float4 ScreenTexelUV;
	float4 cameraPos;
}

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 colorOBJ;
};

cbuffer PerMaterial : register(b2)
{
	int4 colorFromVertex;
}

Texture2D    ScreenShadowMap	: register(t0);	 
Texture2D    depthMap			: register(t2);

SamplerState sampScreenShadow	: register(s0);

Texture2D    textureMap			: register(t5);
SamplerState textureSampler		: register(s5);



//--------------------------------------------------------------------------------------

struct MainGeomInput
{
	float3 Position    : POSITION;
	float3 Normal      : NORMAL;
	float2 TextureUV   : TEXCOORD0;
};

struct MainGeomOutput
{
	float4 Position         : SV_POSITION;
	float2 TextureUV        : TEXCOORD0;
	float3 NormalEyeSpace   : TEXCOORD1;
	float4 ScreenProjCoords : TEXCOORD2;
	float4 Flags			: TEXCOORD3;
};

MainGeomOutput VS(MainGeomInput In)
{
	MainGeomOutput Out;
	
	float4x4 WorldViewTM = mul(WorldTM, ViewTM);
	float4 posEyeSpace = mul(float4(In.Position, 1), WorldViewTM);
	float3 normEyeSpace = normalize(mul(In.Normal, (float3x3)WorldViewTM));
	
	Out.Position = mul(posEyeSpace, ProjTM);
	Out.TextureUV      = In.TextureUV;
	Out.NormalEyeSpace = normEyeSpace;
	Out.Flags			= colorFromVertex[0] ? 0 : 1;
	
	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV.xy;
#endif

	return Out;
}

void PS(MainGeomOutput In, out float4 outColor : SV_TARGET0)
{			
	

	float2 uv = In.TextureUV;// float2(fmod(In.UV.x, 1.0f), fmod(In.UV.x, 1.0f));
	float4 color = textureMap.Sample(textureSampler, uv);
	float3 lerpColor = lerp(float3(1, 1, 1), color.xyz, In.Flags.x);
	color.xyz = lerpColor;
	
	float dotNL = saturate(dot(normalize(In.NormalEyeSpace), DirLightDirectionEyeSpace.xyz));
	//float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float shadow = ScreenShadowMap.Sample(sampScreenShadow, In.ScreenProjCoords.xy / In.ScreenProjCoords.w).r;
	//outColor.rgb = 2 * float3(1.3, 0.7, 0.5) * (dotNL * shadow) + float3(0.3, 0.3, 0.4);
	
	float radiance = float3(1.0, 1.0, 1.0) * (dotNL * shadow) + float3(0.3, 0.3, 0.3);
	//outColor.rgb = 1.0f * float3(1.0, 1.0, 1.0) * (shadow) + float3(0.3, 0.3, 0.3);
	
	
	//outColor.rgb = 2*float3(1.3, 0.7, 0.5) * (dotNL) + float3(0.3, 0.3, 0.4);

	//my
	outColor.rgb = 1.0f*radiance*color.xyz+ 0.1f*color.xyz;
	
	outColor.a = color.w;

	clip( color.a < 0.1f ? -1:1 );

	//float4 newColor = lerp(outColor, fcolor, l);
	//outColor = newColor;

	//outColor.rgb = depthVal;
	//outColor.a = 1.0f;

	
	//outColor = color;
	//outColor.rgb *= colorOBJ.rgb;

	//outColor.rgb = dotNL;
	
	//outColor.rgb = 1;
	//outColor.a = 1;
}

//*/
/*
void PS(MainGeomOutput In, out float4 outColor : SV_TARGET0)
{				
	float2 uv = In.TextureUV;// float2(fmod(In.UV.x, 1.0f), fmod(In.UV.x, 1.0f));
	float3 color = lerp(float3(1, 1, 1), textureMap.Sample(textureSampler, uv).xyz, In.Flags.x);
	//float3 color = textureMap.Sample(textureSampler, uv).xyz;
	float dotNL = saturate(dot(normalize(In.NormalEyeSpace), DirLightDirectionEyeSpace.xyz));
	//float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
	float shadow = ScreenShadowMap.Sample(sampScreenShadow, In.ScreenProjCoords.xy / In.ScreenProjCoords.w).r;
	outColor.rgb = 2 * float3(1.3, 0.7, 0.5) * (dotNL * shadow) + float3(0.3, 0.3, 0.4);
	//outColor.rgb = 2*float3(1.0, 1.0, 1.0) * (dotNL * shadow) + float3(0.3, 0.3, 0.3);
	//outColor.rgb = 2 * float3(1.0, 1.0, 1.0) * (dotNL) + float3(0.3, 0.3, 0.3);
	//outColor.rgb = 2*float3(1.3, 0.7, 0.5) * (dotNL) + float3(0.3, 0.3, 0.4);
	//outColor.rgb = colorOBJ;
	outColor.a = 1;
}
//*/