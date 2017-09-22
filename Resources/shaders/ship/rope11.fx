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
	float4 color;
	//int4 colorFromVertex;
};

cbuffer PerObject : register(b2)
{
	float width;
};

//structs
struct VS_INPUT
{
	float3 vPos : POSITION;
	float3 pos0	: TEXCOORD0;
	float3 pos1	: TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

//textures
Texture2D    ropeTex		: register(t3);
SamplerState ropeSampler	: register(s3);
//Texture2D ScreenShadowMap: register(t1);

float4 sunLightColor()
{
	return float4(0.7f, 0.7f, 0.7f, 1.0f);
}


float4 lightAmbient()
{
	return float4(0.3f, 0.3f, 0.3f, 1.0f);
}

VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT output;
	//float3 pos0 = tex2D(idxSam, float2(uv.x, 0.0f)).xyz;
	//float3 pos1 = tex2D(idxSam, float2(uv.y, 0.0f)).xyz;

	float3 vX = normalize(input.pos1.xyz- input.pos0.xyz);
	float3 vZ = normalize(cameraPos.xyz - (dot(cameraPos.xyz- input.pos0.xyz, vX) * vX + input.pos0.xyz) );
	float3 vY = cross( vX, vZ );
	float3x3 rot = float3x3(vX, vY, vZ);

	float l = length(input.pos1.xyz- input.pos0.xyz);
	float3 pp = float3(input.vPos.x*l, input.vPos.y, 0.0f);
	float3 p = mul( pp, rot );
	p += input.pos0.xyz;

	float4x4 viewProjection = mul(ViewTM, ProjTM);
	output.position = mul(float4(p, 1), viewProjection);

	output.uv = float2(input.vPos.x * l * 5, input.vPos.y / (width * 2.0f) + 0.5f);

	return output;
}



void PS(VS_OUTPUT input, out float4 outColor : SV_TARGET0)
{ 
	outColor = ropeTex.Sample(ropeSampler, input.uv) * (sunLightColor() + lightAmbient());
}

