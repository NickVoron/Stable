#include "../material/common.fx"

// struct VS_INPUT
// {
//     float3 Pos		: POSITION;
// 	float3 Norm		: NORMAL;
// 	float4 SHDir1	: TEXCOORD0;
// 	float4 SHDir2	: TEXCOORD1;
// 	float4 SHAmb1	: TEXCOORD2;
// 	float4 SHAmb2	: TEXCOORD3;
// 	float2 SHDirAmb : TEXCOORD4;
// };

struct VS_INPUT
{
	float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float2 UV1		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;
	float4 SHDir1	: TEXCOORD2;
	float4 SHDir2	: TEXCOORD3;
	float4 SHAmb1	: TEXCOORD4;
	float4 SHAmb2	: TEXCOORD5;
	float2 SHDirAmb : TEXCOORD6;
};

struct ANIMATED_VS_INPUT
{
	float3 Pos			: POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 UV1			: TEXCOORD0;
	float2 UV2			: TEXCOORD1;
	float4 blendWeight	: TEXCOORD2;
	float4 blendIndices	: TEXCOORD3;
};

struct VS_OUTPUT
{
    float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float3 Binormal	: BINORMAL;

	float2 UV1		: TEXCOORD0;
	float2 UV2		: TEXCOORD1;

	float3 viewWS	: TEXCOORD2;
};


// VS_OUTPUT VS( VS_INPUT In )
// {
//     VS_OUTPUT Out;
// 	float4x4 WorldView = mul(World, View);
//     float4 P = mul(float4(In.Pos, 1), WorldView);
//     Out.Pos  = mul(P, Projection);
// 	float3 N = mul(In.Norm, (float3x3)World);
// 
// 	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;
// 	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
// 	float amb1 = dot(In.SHDir1, SHAmb1) + dot(In.SHDir1, SHAmb2) + In.SHDirAmb.x*SHDirAmb.y;
// 	Out.Color.r = dir;
// 	Out.Color.g = amb1;
// 	Out.Color.b = amb;
// 	Out.Norm = N;
// 
//     return Out;
// }

VS_OUTPUT VS( VS_INPUT In )
{
	VS_OUTPUT Out;
	float4x4 WorldView = mul(World, View);
	float4 P = mul(float4(In.Pos, 1), WorldView);
	Out.Pos  = mul(P, Proj);
	//float3 N = mul(In.Norm, (float3x3)World);
	Out.UV1  = float2(In.UV1.x, -In.UV1.y);
	Out.UV2  = float2(In.UV2.x, -In.UV2.y);
	Out.Normal = mul(In.Normal,(float3x3)World);
	Out.Tangent = mul(In.Tangent,(float3x3)World);
	Out.Binormal = normalize( cross(In.Tangent, In.Normal) );
	Out.viewWS = cameraPosition - In.Pos.xyz;


// 	float dir = dot(In.SHDir1, SHDir1) + dot(In.SHDir2, SHDir2) + In.SHDirAmb.x*SHDirAmb.x;
// 	float amb = dot(In.SHAmb1, SHAmb1) + dot(In.SHAmb2, SHAmb2) + In.SHDirAmb.y*SHDirAmb.y;
// 	float amb1 = dot(In.SHDir1, SHAmb1) + dot(In.SHDir1, SHAmb2) + In.SHDirAmb.x*SHDirAmb.y;
// 	Out.Color.r = dir;
// 	Out.Color.g = amb1;
// 	Out.Color.b = amb;


	return Out;
}

float4 PS(VS_OUTPUT In ) : COLOR
{
	float3x3 bnmat = float3x3(In.Tangent, In.Binormal, In.Normal);
	float3 N = normal(In.UV1, In.Tangent, In.Binormal, In.Normal);
	//N = normalize(In.Normal);
	float light = saturate( dot(normalize(N), normalize(-sunDir) ) );
	//return light;
	//return 1;
	//return float4(N, 1);

	float3 viewTS  = (In.viewWS.x*In.Tangent + In.viewWS.y*In.Normal + In.viewWS.z*In.Binormal);
	float specular;
	float3 lightTS  = mul( bnmat, sunDir );
	float3 lightTSAdj = float3( lightTS.x, -lightTS.y, lightTS.z );
	float3 reflectionTS = normalize( 2 * dot( viewTS, N ) * N - viewTS );
	float fRdotL = saturate( dot( reflectionTS, lightTSAdj ));

	float specCosPow = 50.0;
	float specScaleFactor = 1;
	specular = saturate( pow( fRdotL, specCosPow )) * specScaleFactor;

	//return normal(In.UV1, In.Tangent, In.Binormal, In.Normal)
	//return float4(N, 1);
	//return specularColor(In.UV1);
	//return specular + light;
	return light * diffColor(In.UV1, In.UV2);
}

//#define fm wireframe
#define fm solid

technique RenderScene
{
    pass P0
    {
		VertexShader = compile vs_3_0 VS();
        PixelShader  = compile ps_3_0 PS();
		CullMode = none;
		FillMode = fm;
		ZEnable = true;
    }
}
