struct VS_INPUT
{
    float3 Position  : POSITION;
};

struct VS_OUTPUT
{
    float4 Position  : SV_POSITION;
    float2 uv        : TEXCOORD0;
    float3 wp        : TEXCOORD1;
    float3 color     : TEXCOORD2;
};

cbuffer cbPerFrame : register( b0 )
{
	float4x4 viewProj;
	float4 lightDir;
};
cbuffer cbPerObj : register( b1 )
{
	float4x4 objMtx;
	float4 objColor;
};





//-----------------------------------------------------------------------------------
VS_OUTPUT mainVS( VS_INPUT input,  uint VertexID : SV_VertexID )
{
	VS_OUTPUT Output;
	float3 p = input.Position;
	if( objColor.w == 0.0f )
		if( VertexID > 3 )
			p = float3(0,100,0);
		else
			p /= 100.0f;
	Output.wp = mul( objMtx, float4(p, 1) ).xyz;
	Output.Position = mul( viewProj, float4(Output.wp, 1) );
	Output.color = 0;
	return Output;    
}








//-----------------------------------------------------------------------------------
[maxvertexcount(3)]
void mainGS(triangle VS_OUTPUT input[3], inout TriangleStream<VS_OUTPUT> triStream)
{
  VS_OUTPUT output;

  float3 flatNormal = normalize(cross(input[2].wp - input[0].wp, input[1].wp - input[0].wp));
  
  [unroll (3)]
  for( int v = 0; v < 3; v++ )
  { 
    output.Position = input[v].Position;
    output.wp = input[v].wp;
    output.uv = input[v].wp.xy;
    output.color = saturate( dot( flatNormal, lightDir.xyz ) );

    triStream.Append(output);
  }

  triStream.RestartStrip();
}




//-----------------------------------------------------------------------------------
void depthPS( VS_OUTPUT input )
{
}





//-----------------------------------------------------------------------------------
Texture2D diffuseTex : register(t0);
SamplerState pointSampler : register(s0);

float4 mainPS( VS_OUTPUT input ) : SV_TARGET
{
	return float4( diffuseTex.Sample( pointSampler, (input.uv+1)/2 ).rgb * input.color, 1);
}

