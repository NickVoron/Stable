float4x4 WorldView;
float4x4 Projection;


texture Tex;
sampler samp = sampler_state
{
	texture = <Tex>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	MipFilter = linear;
	MinFilter = linear;
	MagFilter = linear;
};

struct VS_INPUT
{
    float3 Pos  : POSITION;
    float3 WPos : TEXCOORD0;
	float2 TexCoord0 : TEXCOORD1;
	float4 SizeRotationIdxOpacity : TEXCOORD2;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
 	float2 TexCoord0 : TEXCOORD0;
	float Opacity : TEXCOORD1;
};

VS_OUTPUT MainVS( VS_INPUT In)
{
    VS_OUTPUT Out;

	// Rotation around Z-Axis
	float Sin = sin( In.SizeRotationIdxOpacity.y);
	float Cos = cos( In.SizeRotationIdxOpacity.y);
	float3 Pos;
	Pos.x = In.Pos.x * Cos - In.Pos.y * Sin;
	Pos.y = In.Pos.x * Sin + In.Pos.y * Cos;
	Pos.z = In.Pos.z;

	float3 P = Pos*In.SizeRotationIdxOpacity.x + mul( float4( In.WPos, 1 ), (float4x3)WorldView );
   	Out.Pos  = mul( float4( P, 1 ), Projection );


	float horizontalIdx = frac( In.SizeRotationIdxOpacity.z * 0.25f ) * 4;
	Out.TexCoord0.x = ( In.TexCoord0.x + horizontalIdx ) * 0.25f;

	float verticalIdx = In.SizeRotationIdxOpacity.z * 0.25f;
	Out.TexCoord0.y = ( In.TexCoord0.y + (verticalIdx - frac( verticalIdx )) ) * 0.25f;

	
	Out.Opacity = In.SizeRotationIdxOpacity.w;
   	
    return Out;
}

float4 MainPS( VS_OUTPUT In) : COLOR
{
	return float4( tex2D(samp, In.TexCoord0).rgb*0.4f, tex2D(samp, In.TexCoord0).a*In.Opacity);
}


technique Main
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = none;
		ZEnable = true;
		ZWriteEnable = false;
		AlphaTestEnable = true;
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = INVSRCALPHA;
    }
}
