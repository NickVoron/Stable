cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4x4 InvertProjTM;
	float4 CameraPos;
	float4 fogColor;
	float fogIntensity;
}

struct STD_VS_INPUT
{
	float3 Pos  : POSITION;
	float2 UV	: TEXCOORD0;
};

struct STD_VS_OUTPUT
{
	float4 Pos  : SV_POSITION;
	float2 UV	: TEXCOORD0;
};

SamplerState smpl			: register(s0);
Texture2D    colorTex				: register(t0);
Texture2D    depthTex				: register(t1);


STD_VS_OUTPUT VS(STD_VS_INPUT input)
{
	STD_VS_OUTPUT output;
	output.Pos = float4(input.Pos, 1.0f);
	output.UV = input.UV;
	return output;
}

float4 PS(STD_VS_OUTPUT In ) : SV_TARGET0
{
	float4 color = colorTex.SampleLevel(smpl, In.UV, 0);
	float depthVal = depthTex.SampleLevel(smpl, In.UV, 0).x;


	float l = exp(-depthVal * fogIntensity);
	l = saturate(1 - l);

	//float4 fogColor = float4(0.5, 0.5, 0.5, 1);

	float4 output = lerp(color, fogColor, l);
	
	//output.xyz = l;
	//output.xyz = depthVal;
	//return color ; 
	return output ; 
	//return 1; 
}



////
////Texture2D    colorMap			: register(t2);
////Texture2D    depthMap			: register(t3);	 
////
////SamplerState textureSampler		: register(s0);
////
//////--------------------------------------------------------------------------------------
////
////struct MainGeomInput
////{
////	float3 Pos  : POSITION;
////	float2 UV	: TEXCOORD0;
////};
////
////struct MainGeomOutput
////{
////	float4 Position         : SV_POSITION;
////	float2 UV				: TEXCOORD0;
////};
////
////MainGeomOutput VS(MainGeomInput input)
////{
////	MainGeomOutput output;
////	output.Position = float4(input.Pos, 1.0f);
////	output.UV = input.UV;
////	
////	//Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
////	//Out.ScreenProjCoords.zw = Out.Position.zw;
////
////	return output;
////}
////
////void PS(MainGeomOutput In, out float4 outColor : SV_TARGET0)
////{				
////	float2 uv = In.UV;
////	float4 color = colorMap.Sample(textureSampler, uv);
////	float depthVal = depthMap.Sample(textureSampler, uv).x;
////
////    //compute screen-space position
////    //float4 position;
////    //position.x = In.TextureUV.x * 2.0f - 1.0f;
////    //position.y = -(In.TextureUV.y * 2.0f - 1.0f);
////    //position.z = depthVal;
////    //position.w = 1.0f;
////    //transform to world space
////    //position = mul(position, InvertViewProjection);
////	//position = mul(position, InvertProjTM);
////	//position /= position.w;
////	//float cameraDistance = length(CameraPos - position);
////
////	//float dz = 0.1f;
////	//float l = exp(-cameraDistance * dz);
////	//l = saturate(1 - l);
////
////	//float4 fcolor = float4(0.5, 0.5, 0.5, 1);
////
////	//float4 newColor = lerp(outColor, fcolor, l);
////	//outColor = newColor;
////
////	outColor.rgb = color.rgb;
////	outColor.a = 1.0f;
////
////	
////	//outColor = color;
////	//outColor.rgb *= colorOBJ.rgb;
////
////	//outColor.rgb = dotNL;
////	//outColor.a = 1;
////}
////
//////*/
/////*
