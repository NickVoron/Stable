cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 DirLightDirectionEyeSpace;
	float4 ScreenTexelUV;
	float4 cameraPos;
}

Texture2D    roadMap		: register(t3);
SamplerState roadSampler	: register(s3);

struct VS_INPUT
{
	float3 pos	: POSITION;
	float3 normal : NORMAL;
	float2 tex0 : TEXCOORD0;	
};

struct VS_OUTPUT
{
	float4 pos				: SV_POSITION;
	float2 tex0				: TEXCOORD0;
    float3 normal			: TEXCOORD1;
	float4 screenProjCoords : TEXCOORD2;
};

Texture2D    ScreenShadowMap	: register(t0);
SamplerState sampScreenShadow	: register(s0);

VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT output;

	float3 P = mul(float4(input.pos, 1), (float4x3)ViewTM);
	output.pos = mul(float4(P, 1), ProjTM);
	
	output.normal = input.normal;
	output.tex0 = float2(input.tex0.x, input.tex0.y*10);

	output.screenProjCoords.xy = output.pos.xy * float2(0.5f, -0.5f) + output.pos.ww * 0.5f;
	output.screenProjCoords.zw = output.pos.zw;


	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET0
{
	//return 1;
	//return float4(input.tex0.yyy, 1);

	float3 normal = float3(0,1,0);
	//normal = 2 * normal - 1.0f;

	float4x4 WorldViewTM =  ViewTM;
	float3 normEyeSpace = normalize(mul(normal, (float3x3)WorldViewTM));

	float dotNL = saturate(dot(normalize(normEyeSpace), DirLightDirectionEyeSpace.xyz));

	float shadow = ScreenShadowMap.Sample(sampScreenShadow, input.screenProjCoords.xy / input.screenProjCoords.w).r;
	float radiance = 1.5*float3(1.0, 1.0, 1.0) * (dotNL* shadow) + 3 * float3(0.3, 0.3, 0.3);

	float4 texColor = roadMap.Sample(roadSampler, input.tex0.yx);
	float3 resultColor = 0.6f*radiance*texColor.xyz;
	
	float4 outColor = float4(resultColor, texColor.w);


	return outColor;
}
