float4x4 WorldView;
float4x4 Projection;

float2 worldShift;
float2 randSin;
float3 lightDir;


struct VS_INPUT
{
    float3 Pos  : POSITION;
    float4 PosRot : TEXCOORD0;
	float2 UV : TEXCOORD1;
	float3 N : NORMAL;
};

struct VS_MAIN_OUTPUT
{
    float4 Pos  : POSITION;
	float2 UV : TEXCOORD0;
	float3 N : TEXCOORD1;
};

texture grassTex;

sampler grassSampl = sampler_state
{
	texture = <grassTex>;

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
};

VS_MAIN_OUTPUT MainVS( VS_INPUT In)
{
    VS_MAIN_OUTPUT Out;
 	float Sin = In.PosRot.z;
 	float Cos = In.PosRot.w;

	// Rotation around Y-Axis
  	float3 Pos = In.Pos;
  	In.Pos.x = Pos.x * Cos - Pos.z * Sin;
  	In.Pos.z = Pos.x * Sin + Pos.z * Cos;
	float3 scale = float3(0.01f, 0.01f, 0.01f)*(4 + In.PosRot.z);
	In.Pos.xyz *= scale;
 	In.Pos.xz += In.PosRot.xy + worldShift;

	//In.WPos.xyz += particleSystemPivot.xyz;	
	float3 P = mul( float4( In.Pos, 1 ), (float4x3)WorldView );  // position (view space)
	//P.xz += worldShift;
   	Out.Pos  = mul( float4( P, 1 ), Projection );
	Out.UV = In.UV;
	Out.N = In.N;
	return Out;
}


float4 MainPS( VS_MAIN_OUTPUT In, in float vFace : VFACE ) : COLOR
{
	float3 n = normalize(In.N);
	float l = saturate( dot(n, lightDir) );
	if(vFace < 0) l *= 0.6f;
	float4 tex = 1;// tex2D(grassSampl, In.UV);
	tex.rgb = tex.rgb * l + 0.3f;
	return  tex;
}


technique Main
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = None;
		ZEnable = true; // если включено партикли глючат!
		ZWriteEnable = true;
		//AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 150;
    }
}