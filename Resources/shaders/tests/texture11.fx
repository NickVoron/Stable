// Constant Buffers
cbuffer cbPerObject : register( b0 )
{
	matrix  g_mWorldViewProjection  : packoffset( c0 );
}


//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D    g_txDiffuse : register( t0 );
SamplerState g_samLinear : register( s0 );

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position     : POSITION; // vertex position 
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position     : SV_POSITION; // vertex position 
    float2 TextureUV    : TEXCOORD0;   // vertex texture coords 
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT Output;
 
    // Transform the position from object space to homogeneous projection space
    Output.Position = mul( float4(input.Position, 1), transpose(g_mWorldViewProjection) );
    
    // Just copy the texture coordinate through
    Output.TextureUV = input.TextureUV; 
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
// color with diffuse material color
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
	//return float4(1, 1, 0, 1);
    // Lookup mesh texture and modulate it with diffuse
    return g_txDiffuse.Sample( g_samLinear, In.TextureUV );
}
