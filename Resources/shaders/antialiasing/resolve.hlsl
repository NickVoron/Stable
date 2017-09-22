//-----------------------------------------------------------------------------------
float4 mainVS( uint VertexID : SV_VertexID) : SV_POSITION
{
    float4 output = 0.0f;

    if(VertexID == 0)
        output = float4(-1.0f, 1.0f, 1.0f, 1.0f);
    else if(VertexID == 1)
        output = float4(3.0f, 1.0f, 1.0f, 1.0f);
    else
        output = float4(-1.0f, -3.0f, 1.0f, 1.0f);

    return output;
}


//-----------------------------------------------------------------------------------
Texture2D resolvedTex : register(t1);
Texture2D depthTex : register(t2);
Texture2DMS<float4> msaaTex : register(t0);
SamplerState pointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};


float Filter(in float x)
{
    // Rescale from [-2, 2] range to [-FilterWidth, FilterWidth]
    x *= 2.0f;

    float y = 0.0f;
    float x2 = x * x;
    float x3 = x * x * x;
    if(x < 1)
        y = (12 - 9) * x3 + (-18 + 12) * x2 + (6 - 2);
    else if (x <= 2)
        y = -x3 + 6 * x2 - 12 * x + 8;

    return y / 6.0f;
}

float4 mainPS1(float4 Position : SV_Position) : SV_TARGET
{
    //return msaaTex.Load(uint2(Position.xy), 0);
	const int MSAASamples_ = 8;
	float4 sample = 0.0f;
    for(uint subSampleIdx = 0; subSampleIdx < MSAASamples_; ++subSampleIdx)
		sample += msaaTex.Load(uint2(Position.xy), subSampleIdx);
	return sample/MSAASamples_;
}

float4 mainPS(float4 Position : SV_Position) : SV_TARGET
{
	float FilterSize = 2.5f;
	const int MSAASamples_ = 8;
	float4 SampleOffsets[ MSAASamples_ ];
	if(MSAASamples_ == 8)
	{
		SampleOffsets[0] = float4(0.580f, 0.298f, 0.0f, 0.0f);
		SampleOffsets[1] = float4(0.419f, 0.698f, 0.0f, 0.0f);
		SampleOffsets[2] = float4(0.819f, 0.580f, 0.0f, 0.0f);
		SampleOffsets[3] = float4(0.298f, 0.180f, 0.0f, 0.0f);
		SampleOffsets[4] = float4(0.180f, 0.819f, 0.0f, 0.0f);
		SampleOffsets[5] = float4(0.058f, 0.419f, 0.0f, 0.0f);
		SampleOffsets[6] = float4(0.698f, 0.941f, 0.0f, 0.0f);
		SampleOffsets[7] = float4(0.941f, 0.058f, 0.0f, 0.0f);
	}
	else if(MSAASamples_ == 4)
	{
		SampleOffsets[0] = float4(0.380f, 0.141f, 0.0f, 0.0f);
		SampleOffsets[1] = float4(0.859f, 0.380f, 0.0f, 0.0f);
		SampleOffsets[2] = float4(0.141f, 0.620f, 0.0f, 0.0f);
		SampleOffsets[3] = float4(0.619f, 0.859f, 0.0f, 0.0f);
	}
	else if(MSAASamples_ == 2)
	{
		SampleOffsets[0] = float4(0.741f, 0.741f, 0.0f, 0.0f);
		SampleOffsets[1] = float4(0.258f, 0.258f, 0.0f, 0.0f);
	}
	else if(MSAASamples_ == 1)
		return msaaTex.Load(uint2(Position.xy), 0);

    float2 pixelPos = Position.xy;

    float3 sum = 0.0f;
    float totalWeight = 0.0f;

    int SampleRadius = (int)((FilterSize / 2.0f) + 0.499f);
    for(int y = -SampleRadius; y <= SampleRadius; ++y)
    {
        for(int x = -SampleRadius; x <= SampleRadius; ++x)
        {
            float2 samplePos = pixelPos + float2(x, y);
            samplePos = clamp(samplePos, 0, float2(1280,720) - 1.0f);

            [unroll]
            for(uint subSampleIdx = 0; subSampleIdx < MSAASamples_; ++subSampleIdx)
            {
                float2 sampleOffset = float2(x, y) + SampleOffsets[ subSampleIdx ].xy-0.5f;
                float sampleDist = length(sampleOffset) / (FilterSize / 2.0f);

                [branch]
                if(sampleDist <= 1.0f)
                {
                    float weight = Filter(sampleDist);
                    totalWeight += weight;
                    sum += weight * msaaTex.Load( uint2(samplePos), subSampleIdx).xyz;
                }
            }
        }
    }

    return float4( max( sum / max(totalWeight, 0.00001f), 0.0f), 1.0f);
}







struct PS_OUT
{
    float4 color : SV_TARGET;
    float depth : SV_DEPTH;
};

PS_OUT upsamplePS(float4 Position : SV_Position)
{
    PS_OUT o;
    o.color = resolvedTex.Load(uint3(Position.xy,0));
    o.depth = depthTex.Load(uint3(Position.xy,0)).r;
	return o;
}



RWTexture2D<float4> csOutTex : register(u0);

[numthreads(16, 16, 1)]
void resolveCS( uint3 DTid : SV_DispatchThreadID )
{
    csOutTex[ DTid.xy ] = msaaTex.Load(uint2(DTid.xy),0);
}
