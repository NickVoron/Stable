float4x4 mC2W;  // inverse matrix of world to projection matrix
float2   vPix;  // 0.5f/texture size
float4   vOff;  // parameter to compute blur direction


struct SVSInput 
{
	float4 vPosC : POSITION0;
	float2 vTex : TEXCOORD0;
};

struct SVSOutput 
{
	float4 vPos : POSITION0;
	float4 vWorldPos : TEXCOORD0;
	float4 vTex : TEXCOORD1;	
};


void mainVS( out SVSOutput Out, in SVSInput In )
{
	Out.vPos = float4( In.vPosC.xy, 0.5f, 1 );   // projection position
	Out.vWorldPos = mul( In.vPosC, mC2W );// transform projection space to world space 
	Out.vTex.xy = In.vTex + vPix;  // uv 
	Out.vTex.zw = In.vTex * vOff.xy + vOff.zw;// compute blur direction 
}

#define FILTER_WIDTH 16

sampler sDensity : register(s0);  // density map

float  vFallOff; // fall off parameters
float2  invMax;   // inverse of the maximum length of the blur vector

float4 mainPS(SVSOutput In) : COLOR0
{
	// Compute UV offset.
	float2 UVOffset = In.vTex.zw / FILTER_WIDTH;

	// limit blur vector
	float2 len = abs( UVOffset * invMax );
	float over = max( len.x, len.y );
	float scale = over > 1.0f ? 1.0f/over : 1.0f;
	UVOffset.xy *= scale;

	// blur 
	float2 uv = In.vTex.xy;
	float4 res = tex2D( sDensity, uv );
	float4 weightSum = 1.0f;
	// scale parameter of exponential weight
	float dist = dot( UVOffset, UVOffset );

	float falloff = vFallOff;
	for ( int i = 1; i < FILTER_WIDTH; ++i ) 
	{
		uv += UVOffset * falloff;
		//falloff *= 0.5f; 
		float4 weight = 1.0f;// exp(dist * i );
		res += weight * tex2D( sDensity, uv);
		weightSum += weight;
	}
	res /= weightSum;

	return res;
}

technique RenderTech
{
	pass Pass0
	{
    	VertexShader = compile vs_3_0 mainVS();
    	PixelShader = compile ps_3_0 mainPS();
	}
}


