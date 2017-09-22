float reflectivity
<
    string UIName = "Reflectivity";
	string UIWidget = "Slider";
	float UIMin = 0.0f;
	float UIStep = 0.05f;
	float UIMax = 1.0f;
> = 0.02f;


float3 deepColor
<
    string UIName = "Deep Water Color";
	string UIWidget = "Color";
> = { 0.01, 0.3, 0.4 };

float3 shallowColor 
<
    string UIName = "Shallow Water Color";
	string UIWidget = "Color";
> = { 0.01, 0.1, 0.2 };

float shallowDepth 
<
    string UIName = "Shallow Water Depth";
	string UIWidget = "Slider";
> = 20.0f;

float3 attenColor
<
    string UIName = "Attenuation Color";
	string UIWidget = "Color";
> = { 0.5f, 0.5f, 0.5f };
float attenDist
<
    string UIName = "Attenuation Distance";
	string UIWidget = "Slider";
> = 50.0f;

static float3 beamAttColor < string UIWidget = "None"; >
= -log( pow(attenColor.rgb, 1.0f / attenDist) );

static float3 diffAttColor < string UIWidget = "Color"; >
= -log( saturate((1.0f - saturate(shallowColor.rgb / deepColor.rgb)) / exp(-shallowDepth * beamAttColor.rgb)) ) / shallowDepth;


float4x4 ReflectViewProjTexXf;
float4x4 RefractViewProjTexXf;

texture texReflectColor;
texture texReflectDepth;

texture texRefractColor;
texture texRefractDepth;

sampler sampReflectColor = sampler_state
{
	texture = <texReflectColor>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampReflectDepth = sampler_state
{
	texture = <texReflectDepth>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

sampler sampRefractColor = sampler_state
{
	texture = <texRefractColor>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampRefractDepth = sampler_state
{
	texture = <texRefractDepth>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};


float2 ViewportSize : VIEWPORTPIXELSIZE;
static float2 InvViewportSize = float2(1.0f / ViewportSize.x, 1.0f / ViewportSize.y);

float2 BaseUVStep;
float2 AddUVStep;


float GridSize;
float GeomStep;

float4 DebugColor;

float4 PosXVec;
float4 PosYVec;

float2 BlockOrg;

float4x4 ViewProjTM;

float4 WorldEyePos;

float AlphaHeight = 1.0f;

texture TexHeight;
texture TexDisplace;
texture TexSlope;


texture TexSlopeX1;
texture TexSlopeX2;
texture TexSlopeX4;

sampler sampHeight = sampler_state
{
	texture = <TexHeight>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampDisplace = sampler_state
{
	texture = <TexDisplace>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampSlope = sampler_state
{
	texture = <TexSlope>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

sampler sampSlopeX1 = sampler_state
{
	texture = <TexSlopeX1>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSlopeX2 = sampler_state
{
	texture = <TexSlopeX2>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSlopeX4 = sampler_state
{
	texture = <TexSlopeX4>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

textureCUBE texEnvCube;

samplerCUBE sampEnvCube = sampler_state
{
	texture = <texEnvCube>;
	AddressU  = Wrap;
	AddressV  = Wrap;
	AddressW  = Wrap;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

struct SInput
{
    float4 Position : POSITION;
};

struct SOutput
{
    float4 Position : POSITION;
    float4 BaseUV : TEXCOORD0;
    float2 AddUV1 : TEXCOORD1;
    float2 AddUV2 : TEXCOORD2;
    float2 AddUV4 : TEXCOORD3;
    float4 WorldEyeVec : TEXCOORD4;
	float3 WorldEyeVec_Reflect : TEXCOORD5;
    float SlopeDiv : TEXCOORD6;
    float Alpha : TEXCOORD7;
};

SOutput Draw_VS(SInput In)
{
    SOutput Out;

    float2 pos;
    pos.x = dot(In.Position, PosXVec);
    pos.y = dot(In.Position, PosYVec);
    
    float4 baseUV;
    baseUV.xy = (pos + 0.5f) * BaseUVStep;
    baseUV.zw = 0.0f;
    Out.BaseUV = baseUV;
	
	pos += BlockOrg;
	Out.AddUV1 = (pos*1 + 0.5f) * AddUVStep;
	Out.AddUV2 = (pos*2 + 0.5f) * AddUVStep;
	Out.AddUV4 = (pos*4 + 0.5f) * AddUVStep;
    
    float4 worldPos;
    worldPos.xz = pos * GeomStep;
    
    static float w = GridSize / 8;
    static float AlphaOffset = ((GridSize-2)/2 - 1 - w) * GeomStep;
    static float OneOverWidth = 1.0f / (w * GeomStep);
    
    float2 alpha2 = saturate((abs(worldPos.xz - WorldEyePos.xz) - AlphaOffset)*OneOverWidth);
    float alpha = AlphaHeight * (1 - max(alpha2.x, alpha2.y));


    float4 h4 = tex2Dlod(sampHeight, baseUV);
    float2 h2 = lerp(h4.zw, h4.xy, alpha);
    
    const float Q = 0.7f;
    
    float4 d4 = tex2Dlod(sampDisplace, baseUV);
    float2 d2 = lerp(d4.zw, d4.xy, alpha);

    worldPos.xz += Q * d2;
    
    worldPos.y = h2.x;
    worldPos.w = 1.0f;
    
    Out.SlopeDiv = 1.0f - Q * h2.y;

    Out.Position = mul(worldPos, ViewProjTM);
    
    Out.Alpha = alpha;
    Out.WorldEyeVec.xyz = worldPos - WorldEyePos.xyz;
	Out.WorldEyeVec.w = Out.Position.w;
	
	Out.WorldEyeVec_Reflect = worldPos - WorldEyePos.xyz * float3(1, -1, 1);
    return Out;
}

float getFresnel(float ref, float dotIN)
{
	float fresnel = ref + (1.0 - ref) * pow(1.0 - dotIN, 5.0);
	return fresnel;
}

float3 getReflectVec(float3 V, float3 Vr, float3 Nn)
{
	float4 texCoordV = mul(float4(Vr, 1.0f), ReflectViewProjTexXf);
	float depthV = tex2Dproj(sampReflectDepth, texCoordV).r;
	float radius = length(Vr) * max(depthV / texCoordV.w - 1, 0);
	
	float3 R = Vr + radius * reflect(normalize(V), Nn);
	R.y = max(R.y, 0.0f);
	return R;
}

float3 getRefractVec(float3 V, float3 Vr, float3 Nn, out float dist, out float distY)
{
	float4 texCoordV = mul(float4(V, 1.0f), RefractViewProjTexXf);
	float depthV = tex2Dproj(sampRefractDepth, texCoordV).r;
	float radius = length(V) * max(depthV / texCoordV.w - 1, 0);

	float3 R = V + radius * reflect(normalize(Vr), -Nn);
	R.y = min(R.y, 0.0f);
	
	float4 texCoordR = mul(float4(R, 1.0f), RefractViewProjTexXf);
	float depthR = tex2Dproj(sampRefractDepth, texCoordR).r;

	float3 D = R * (depthR / texCoordR.w) - V;
	dist = length(D);
	distY = abs(D.y);
	return R;
}

float4 DrawDepth_PS(SOutput In) : COLOR
{
	return In.WorldEyeVec.w;
}

float4 Draw_PS(SOutput In) : COLOR
{
	float3 n;
	n.y = 1.0f;
	
	float4 slope4 = tex2Dlod(sampSlope, In.BaseUV);
	n.xz = lerp(slope4.zw, slope4.xy, In.Alpha) / In.SlopeDiv;
	
    n.xz += tex2D(sampSlopeX1, In.AddUV1).xy * (1.0f - In.Alpha);
    n.xz += tex2D(sampSlopeX2, In.AddUV2).xy;
    n.xz += tex2D(sampSlopeX4, In.AddUV4).xy * In.Alpha;
	
    float3 Nn = normalize(n);
    float3 Vn = normalize(In.WorldEyeVec.xyz);

	//fresnel
    float dotIN = saturate(-dot(Vn, Nn));
    float fresnel = getFresnel(reflectivity, dotIN);
    
    //reflect
    float3 worldReflect = getReflectVec(In.WorldEyeVec.xyz, In.WorldEyeVec_Reflect.xyz, Nn);
	float4 reflectTexCoord = mul(float4(worldReflect, 1.0f), ReflectViewProjTexXf);
	float3 reflectColor = tex2Dproj(sampReflectColor, reflectTexCoord);
	
    //refract
	float refractDist, refractDistY;
	float3 worldRefract = getRefractVec(In.WorldEyeVec.xyz, In.WorldEyeVec_Reflect.xyz, Nn, refractDist, refractDistY);
	float4 refractTexCoord = mul(float4(worldRefract, 1.0f), RefractViewProjTexXf);
	float3 refractColor = tex2Dproj(sampRefractColor, refractTexCoord);
	
	float3 beamAttFactor = exp(-refractDist * beamAttColor);
	float3 diffAttFactor = exp(-refractDistY * diffAttColor);
	
	refractColor *= beamAttFactor;
	refractColor += deepColor * saturate(1.0f - beamAttFactor * diffAttFactor);

	//output
    float4 finalColor;
    finalColor.rgb = lerp(refractColor, reflectColor, fresnel);
    finalColor.a = 1;
    return finalColor;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/*
texture TexHeightBase;
texture TexDisplaceBase;
*/

texture TexCombineBase;
texture TexSlopeBase;

texture TexHeightAdd;
texture TexDisplaceAdd;
texture TexSlopeAdd;

/*
sampler sampHeightBase = sampler_state
{
	texture = <TexHeightBase>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampDisplaceBase = sampler_state
{
	texture = <TexDisplaceBase>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
*/
sampler sampCombineBase = sampler_state
{
	texture = <TexCombineBase>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampSlopeBase = sampler_state
{
	texture = <TexSlopeBase>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Linear;
	MagFilter = Linear;
};

sampler sampHeightAdd = sampler_state
{
	texture = <TexHeightAdd>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampDisplaceAdd = sampler_state
{
	texture = <TexDisplaceAdd>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};
sampler sampSlopeAdd = sampler_state
{
	texture = <TexSlopeAdd>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

float4 ComposeBase;
float4 ComposeAdd;

struct ComposeInput
{
    float4 Position : POSITION;
};

struct ComposeOutput
{
    float4 Position : POSITION;
    float2 BaseUV : TEXCOORD0;
    float2 AddUV : TEXCOORD1;
};

ComposeOutput Compose_VS(ComposeInput In)
{
    ComposeOutput Out;
    Out.Position = In.Position;

    float2 uv = (In.Position.xy + 1.0f) * 0.5f;
    uv.y = 1.0f - uv.y;

    Out.BaseUV = uv * ComposeBase.xy + ComposeBase.zw;
    Out.AddUV = uv * ComposeAdd.xy + ComposeAdd.zw;
    return Out;
}

void Compose_PS(uniform float base, ComposeOutput In, out float4 combine : COLOR0, out float4 slope : COLOR1)
{
	combine = tex2D(sampCombineBase, In.BaseUV) * base;
    combine.xy += tex2D(sampHeightAdd, In.AddUV).xy;
    combine.zw += tex2D(sampDisplaceAdd, In.AddUV).xy;

    slope.zw = tex2D(sampSlopeBase, In.BaseUV).xy * base;
    slope.xy = tex2D(sampSlopeAdd, In.AddUV).xy + slope.zw;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
texture TexCombine;

sampler sampCombine = sampler_state
{
	texture = <TexCombine>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Point;
	MagFilter = Point;
};

void Copy_PS(uniform float base, ComposeOutput In, out float4 height : COLOR0, out float4 displace : COLOR1)
{
	float4 combine = tex2D(sampCombineBase, In.BaseUV) * base;
	height.zw = combine.xy;
	displace.zw = combine.zw;
	
	combine = tex2D(sampCombine, In.AddUV);
	height.xy = combine.xy;
	displace.xy = combine.zw;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

technique Draw
{
    pass p0
    {
        VertexShader = compile vs_3_0 Draw_VS();
        PixelShader  = compile ps_3_0 Draw_PS();

        //CullMode = CCW;
        //FillMode = WireFrame;
        
        AlphaBlendEnable = false;
        BlendOp = Add;
        SrcBlend = One;
        DestBlend = InvSrcAlpha;
   }
}

technique DrawDepth
{
    pass p0
    {
        VertexShader = compile vs_3_0 Draw_VS();
        PixelShader  = compile ps_3_0 DrawDepth_PS();

        CullMode = None;
        AlphaBlendEnable = false;
   }
}

technique Compose
{
    pass p0
    {
        VertexShader = compile vs_3_0 Compose_VS();
        PixelShader  = compile ps_3_0 Compose_PS(1.0f);

        CullMode = None;
   }
}
technique Compose0
{
    pass p0
    {
        VertexShader = compile vs_3_0 Compose_VS();
        PixelShader  = compile ps_3_0 Compose_PS(0.0f);

        CullMode = None;
   }
}
technique Copy
{
    pass p0
    {
        VertexShader = compile vs_3_0 Compose_VS();
        PixelShader  = compile ps_3_0 Copy_PS(1.0f);

        CullMode = None;
   }
}
technique Copy0
{
    pass p0
    {
        VertexShader = compile vs_3_0 Compose_VS();
        PixelShader  = compile ps_3_0 Copy_PS(0.0f);

        CullMode = None;
   }
}

///
struct TestInput
{
    float4 Position : POSITION;
	float2 UV : TEXCOORD0;
};

struct TestOutput
{
    float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
};

float4x4 MVP : WorldViewProjection;

TestOutput TestVS(TestInput In)
{
    TestOutput Out;
    Out.HPos = mul(In.Position, MVP);
	Out.UV = In.UV;
    return Out;
}

float4 TestPS(TestOutput In) : COLOR
{
	float dist = In.UV.x*50.0f;

	float3 beamAttFactor = exp(-dist * beamAttColor);
	float3 diffAttFactor = exp(dist * -1 * diffAttColor);
	
	float3 color = deepColor * saturate(1.0f - beamAttFactor * diffAttFactor);
	return float4(color, 1);
}

technique Test
{
    pass p0
    {
        VertexShader = compile vs_2_0 TestVS();
        PixelShader  = compile ps_2_0 TestPS();
        
        CullMode = None;
    }
}