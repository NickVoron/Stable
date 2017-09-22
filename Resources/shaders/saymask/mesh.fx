cbuffer cbPerObject : register( b0 )
{
	matrix proj;
	float4 cameraPosition;
	float4 cameraDirection;
	float4 lightDirection;
} 					  

cbuffer cbPerObject : register( b1 )
{
	matrix node;
	float4 mat_specular;
} 				

Texture2D    textureMap		: register( t0 );
SamplerState textureSampler	: register( s0 );

struct TEX_RECT_VS_INPUT
{
    float3 pos		: POSITION;
    float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float2 tex0		: TEXCOORD0;
	float2 tex1		: TEXCOORD1;
	float4 color	: COLOR0;
};

struct TEX_RECT_VS_OUTPUT
{
    float4 pos  : SV_POSITION;
	float2 tex	: TEXCOORD0;
	float4 color : TEXCOORD1;
	float3 normal : TEXCOORD2;
	float3 worldPos : TEXCOORD3;
};

TEX_RECT_VS_OUTPUT VS(TEX_RECT_VS_INPUT input)
{
    TEX_RECT_VS_OUTPUT output;
		
	float4 pos = mul(float4(input.pos, 1.0f), node);
	output.worldPos = pos;
	output.pos = mul(pos, proj);
	output.tex = input.tex0;
	output.color = input.color;
	output.normal = mul(input.normal, (float3x3)node) ;
	
    return output;
}

float4 PS(TEX_RECT_VS_OUTPUT input) : SV_TARGET0
{
	float3 normal = normalize(input.normal);
	float3 V = normalize( cameraPosition.xyz - input.worldPos );
	float3 R = reflect( normalize( lightDirection ), normalize( normal ) );
	float specular = mat_specular.x * pow( saturate( dot( R, V ) ), mat_specular.y );
	
	float diffuse = (dot(lightDirection, normal));

	float3 base = 1.0f;

	//return float4(normal, 1);
	return float4(base * (saturate(specular) + diffuse + 0.3f), 1.0f);
}
