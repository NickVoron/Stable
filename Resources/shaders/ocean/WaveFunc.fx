
float4 SourceTexelUV;

float Time = 0.0f;
float2 UVtoK;
float Step;

//------------------------------------------------------------------------------
// Phillips spectrum
//------------------------------------------------------------------------------

float   Ph_Amplitude;
float2  Ph_Length;
float   Ph_Cutoff;
float   Ph_Gravity;
float   Ph_WindVel;
float2  Ph_WindDir;

float   Ph_kMaxSqr;

float Ph(float2 k)
{	
      float kSqr = dot(k, k);
      float x = (kSqr == 0.0f) ? 0.0f : (Ph_kMaxSqr / kSqr);
      return Ph_Amplitude * exp(1.0f - x) * x;
 	  

     float k2 = dot(k, k);
     
     float L2 = (Ph_WindVel * Ph_WindVel) / Ph_Gravity; L2 *= L2;
     
     float l2 = (Ph_Cutoff * Ph_Cutoff);
 
     float c = dot(k, Ph_WindDir); //c = max(c, 0.0f);
     float c2 = (c * c) / k2;
     
     float s = exp(-1.0f / (k2 * L2)) / (k2 * k2);
     return k2; //Ph_Amplitude *  c2 * exp(-k2 * l2);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

texture texSource;

sampler sampSource = sampler_state
{
	texture = <texSource>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

texture texTangling;

sampler sampTangling = sampler_state
{
	texture = <texTangling>;
	AddressU  = Wrap;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

struct SInput
{
    float4 Position : POSITION;
};

struct SOutput
{
    float4 Position : POSITION;
    float2 SourceUV : TEXCOORD0;
};

SOutput Init_VS(SInput In)
{
    SOutput Out;
    Out.Position = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.SourceUV = uv;
    return Out;
}

float4 Init_PS(SOutput In) : COLOR
{
	float2 idx;
    idx.x = tex2D(sampTangling, float2(In.SourceUV.x, 0.5f)).r;
    idx.y = tex2D(sampTangling, float2(In.SourceUV.y, 0.5f)).r;
	idx *= 65535.0f; // from int16
	
    float2 k = idx * SourceTexelUV.xy;
	//float2 k = In.SourceUV;
    k = k - step(0.5f, k);
    k *= UVtoK;
    
	float4 n = tex2D(sampSource, In.SourceUV);
    return float4(n.xy * Ph(+k), n.zw * Ph(-k));
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

SOutput Update_VS(SInput In)
{
    SOutput Out;
    Out.Position = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.SourceUV = uv;
    return Out;
}


float2 cmul(float ar, float ai, float br, float bi)
{
    return float2(ar*br - ai*bi, ar*bi + ai*br);
}

void Update_PS(SOutput In, out float4 height : COLOR0, out float4 displace : COLOR1)
{
	float2 idx;
    idx.x = tex2D(sampTangling, float2(In.SourceUV.x, 0.5f)).r;
    idx.y = tex2D(sampTangling, float2(In.SourceUV.y, 0.5f)).r;
	idx *= 65535.0f; // from int16
	
    float2 k = idx * SourceTexelUV.xy;
	//float2 k = In.SourceUV;
    k = k - step(0.5f, k);
    k *= UVtoK;

    float2 hp = tex2D(sampSource, In.SourceUV).xy;
    float2 hm = tex2D(sampSource, In.SourceUV).zw;
    
    float w = sqrt(length(k) * Ph_Gravity);
    
    float2 ce;
    sincos(w * Time, ce.y, ce.x);
    
    float2 h = cmul(hp.x, hp.y, ce.x, ce.y) + cmul(hm.x, -hm.y, ce.x, -ce.y);

	float klen = length(k);
	float klenI = (klen > 0.0f) ? (1.0f / klen) : 0.0f;
    float2 kn = k * klenI;

    displace = float4(cmul(-kn.y, +kn.x, h.x, h.y), 0, 0);
    //slope = float4(cmul(+k.y, -k.x, h.x, h.y), 0, 0);

    float2 ih = float2(-h.y, +h.x);
    
    height = float4(h + ih * klen, 0, 0);
    
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

SOutput Slope_VS(SInput In)
{
    SOutput Out;
    Out.Position = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.SourceUV = uv;
    return Out;
}

float4 Slope_PS(SOutput In) : COLOR
{
    float vSS = tex2D(sampSource, SourceTexelUV.zw + In.SourceUV).x;
    float vAS = tex2D(sampSource, SourceTexelUV.xw + In.SourceUV).x;
    float vSA = tex2D(sampSource, SourceTexelUV.zy + In.SourceUV).x;
    float vAA = tex2D(sampSource, SourceTexelUV.xy + In.SourceUV).x;

    float v0S = 2 * tex2D(sampSource, float2(0, SourceTexelUV.w) + In.SourceUV).x;
    float vS0 = 2 * tex2D(sampSource, float2(SourceTexelUV.z, 0) + In.SourceUV).x;
    float vA0 = 2 * tex2D(sampSource, float2(SourceTexelUV.x, 0) + In.SourceUV).x;
    float v0A = 2 * tex2D(sampSource, float2(0, SourceTexelUV.y) + In.SourceUV).x;
    
    float2 slope;
    slope.x = (vAS + vA0 + vAA) - (vSS + vS0 + vSA);
    slope.y = (vSA + v0A + vAA) - (vSS + v0S + vAS);
    slope /= -8 * Step;
//    n.x = vA0 - vS0;
//    n.y = v0A - v0S;
//    n /= -2 * Step;
    
    return float4(slope, 0, 0);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

technique Init
{
    pass p0
    {
        VertexShader = compile vs_3_0 Init_VS();
        PixelShader  = compile ps_3_0 Init_PS();

        CullMode = None;
   }
}

technique Update
{
    pass p0
    {
        VertexShader = compile vs_3_0 Update_VS();
        PixelShader  = compile ps_3_0 Update_PS();

        CullMode = None;
   }
}

technique Slope
{
    pass p0
    {
        VertexShader = compile vs_3_0 Slope_VS();
        PixelShader  = compile ps_3_0 Slope_PS();

        CullMode = None;
   }
}
