
float PassStep;
float PassFactor;

float2 SourceTexelUV;

#ifndef TexCount
#define TexCount 2
#endif

texture texSource0;
texture texSource1;
texture texSource2;
texture texSource3;

sampler sampSource[4] = {
    sampler_state
    {
        texture = <texSource0>;
        AddressU  = Clamp;
        AddressV  = Clamp;

        MipFilter = None;
        MinFilter = Point;
        MagFilter = Point;
    },
    sampler_state
    {
        texture = <texSource1>;
        AddressU  = Clamp;
        AddressV  = Clamp;

        MipFilter = None;
        MinFilter = Point;
        MagFilter = Point;
    },
    sampler_state
    {
        texture = <texSource2>;
        AddressU  = Clamp;
        AddressV  = Clamp;

        MipFilter = None;
        MinFilter = Point;
        MagFilter = Point;
    },
    sampler_state
    {
        texture = <texSource3>;
        AddressU  = Clamp;
        AddressV  = Clamp;

        MipFilter = None;
        MinFilter = Point;
        MagFilter = Point;
    }
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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

struct TanglingInput
{
    float4 Position : POSITION;
};

struct TanglingOutput
{
    float4 Position : POSITION;
    float2 SourceUV : TEXCOORD0;
};

TanglingOutput TanglingVS(TanglingInput In)
{
    TanglingOutput Out;
    Out.Position = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.SourceUV = uv + SourceTexelUV.xy * 0.5f;
    return Out;
}

void TanglingPS(TanglingOutput In, out float4 res[TexCount] : COLOR)
{
    float2 uv = In.SourceUV.xy;
	float2 idx;
    idx.x = tex2D(sampTangling, float2(uv.x, 0.5f)).r;
    idx.y = tex2D(sampTangling, float2(uv.y, 0.5f)).r;
	idx *= 65535.0f; // from int16
    uv = (idx + 0.5f) * SourceTexelUV.xy;
    
    for (int i = 0; i < TexCount; ++i)
    {
        res[i] = tex2D(sampSource[i], uv);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
struct MainInput
{
    float2 srcUV : TEXCOORD0;
	float2 addUV : TEXCOORD1;
};

struct MainOutput
{
    float4 Position : POSITION;
    float2 SourceUV00 : TEXCOORD0;
    float2 SourceUV10 : TEXCOORD1;
    float2 SourceUV01 : TEXCOORD2;
    float2 SourceUV11 : TEXCOORD3;
    float2 TanglingUVx : TEXCOORD4;
    float2 TanglingUVy : TEXCOORD5;
	float2 Select : TEXCOORD6;
};

MainOutput MainVS(MainInput In)
{
    MainOutput Out;
	
    Out.Position.xy = (In.srcUV + In.addUV) * float2(+1.0, -1.0f) + float2(-1.0f, +1.0f);
	Out.Position.zw = float2(0.0f, 1.0f);

	float2 uv = In.srcUV;
    Out.SourceUV00 = uv;
    Out.SourceUV01 = uv + SourceTexelUV.xy * float2(1, 0);
    Out.SourceUV10 = uv + SourceTexelUV.xy * float2(0, 1);
    Out.SourceUV11 = uv + SourceTexelUV.xy;
    Out.TanglingUVx = float2(uv.x * PassStep, 0.5f);
    Out.TanglingUVy = float2(uv.y * PassStep, 0.5f);
	Out.Select = In.addUV;
    return Out;
}


void MainPS(MainOutput In, out float4 res[TexCount] : COLOR)
{
	float2 ang;
    ang.x = (tex2D(sampTangling, In.TanglingUVx).r * 65535.0f); // from L16
    ang.y = (tex2D(sampTangling, In.TanglingUVy).r * 65535.0f); // from L16
    ang *= PassFactor;
    
    float3 expX, expY;
    sincos(ang.x, expX.x, expX.y); expX.z = -expX.x;
    sincos(ang.y, expY.x, expY.y); expY.z = -expY.x;
	
    for (int i = 0; i < TexCount; ++i)
    {
		float2 c00 = tex2D(sampSource[i], In.SourceUV00.xy).rg;
		float2 c01 = tex2D(sampSource[i], In.SourceUV01.xy).rg;
		float2 c10 = tex2D(sampSource[i], In.SourceUV10.xy).rg;
		float2 c11 = tex2D(sampSource[i], In.SourceUV11.xy).rg;

		float2 c0, c1;
		if (In.Select.x == 0)
		{
			c0 = c00 + c01;
			c1 = c10 + c11;
		}
		else
		{
			c0 = c00 - c01;
			c1 = c10 - c11;

			c0 = float2(dot(c0, expX.yz), dot(c0, expX.xy));
			c1 = float2(dot(c1, expX.yz), dot(c1, expX.xy));
		}

		float2 c;
		if (In.Select.y == 0)
		{
			c = c0 + c1;
		}
		else
		{
			c = c0 - c1;
			c = float2(dot(c, expY.yz), dot(c, expY.xy));
		}

		res[i] = float4(c, 0, 0);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

technique Tangling
{
    pass p0
    {
        VertexShader = compile vs_3_0 TanglingVS();
        PixelShader  = compile ps_3_0 TanglingPS();

        CullMode = None;
   }
}

technique Main
{
    pass p0
    {
        VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();

        CullMode = None;
    }
}
