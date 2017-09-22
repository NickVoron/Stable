#define COMPILE_VS compile vs_3_0
#define COMPILE_PS compile ps_3_0

#define D3D9_RASTER_OFFSET

float RelFrameTime : ELAPSEDTIME;

float2 ViewportSize : VIEWPORTPIXELSIZE;
static float2 ViewportInvSize = (float2(1.0f, 1.0f) / ViewportSize);


///

float hdrLevel = 0.2f;

static float3 hdrRGBtoY = float3(0.299f, 0.587f, 0.114f);
static float3 hdrRGBtoS = float3(-0.060205f, 0.543640f, 0.359775f);

static float3 hdrBlueShift = float3(1.05f, 0.97f, 1.27f);

static float hdrLumMin = 0.0001f;
static float hdrLumMax = 1000000;
static float hdrPreScale = 100.0f;

static float hdrTimeRod = 0.4f; //sec
static float hdrTimeCones = 0.1f; //sec

static float hdrLdmax = 100.0f; // cd/m^2

static float hdrExposeRadius = 0.5f; //from 0.0f to 0.5f;


#define USE_LOG_LUM_AVERAGE 1

float getRGBLum(float3 clr)
{
	return dot(clr, hdrRGBtoY);
}

float getRGB_S(float3 clr)
{
	return max(dot(clr, hdrRGBtoS), 0.0f);
}

texture	MainRT : RENDERCOLORTARGET
<
	float2 ViewportRatio = {1, 1};
	int MipLevels = 1;
	string Format = "A16B16G16R16F";
	string UIWidget = "None";
>;

sampler2D sampMainRT = sampler_state
{
	Texture = <MainRT>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = None;
};

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

#define DECLARE_RT(TexName, Size, TexFormat, Filter) \
texture TexName : RENDERCOLORTARGET \
< \
	float2 Dimensions = {Size, Size}; \
	int MipLevels = 1; \
	string Format = TexFormat; \
	string UIWidget = "None"; \
>; \
sampler2D samp##TexName = sampler_state \
{ \
	Texture = <TexName>; \
    AddressU = Clamp; \
    AddressV = Clamp; \
	MagFilter = Filter; \
	MinFilter = Filter; \
	MipFilter = None; \
};

#define DECLARE_AVG_LUM(TexName, Size) DECLARE_RT(TexName, Size, "A16B16G16R16F", Linear)

DECLARE_AVG_LUM(AvgLum_512, 512)
DECLARE_AVG_LUM(AvgLum_64,  64)
DECLARE_AVG_LUM(AvgLum_8,   8)
DECLARE_AVG_LUM(AvgLum_1,   1)

#ifndef MODE_SOLAR
texture AvgLumDS : RENDERDEPTHSTENCILTARGET
<
	float2 Dimensions = {512, 512};
	string Format = "D24X8";
	string UIWidget = "None";
>;
#endif

#define DECLARE_LUM_SCALE(TexName) DECLARE_RT(TexName, 1, "A32B32G32R32F", Point)


DECLARE_LUM_SCALE(LumScale)
DECLARE_LUM_SCALE(LumScalePrev)


//--------------------------------------------------------------------------------------
// Vertex Shader input/output structures
//--------------------------------------------------------------------------------------
struct	VS_INPUT
{
	float4 Position  : POSITION;
	float2 TextureUV : TEXCOORD0;	
};

struct VS_OUTPUT
{
	float4 Position  : POSITION0;
	float2 TextureUV : TEXCOORD0;
};

VS_OUTPUT VS_Main(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.Position  = In.Position;
	Out.TextureUV = In.TextureUV;
#ifdef D3D9_RASTER_OFFSET
	Out.TextureUV += 0.5f * ViewportInvSize;
#endif
	return Out;
}

float4 PS_Draw(uniform sampler2D samp, VS_OUTPUT In) : COLOR
{
	return tex2D(samp, In.TextureUV);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

float4 PS_EvalAvgLum(VS_OUTPUT In) : COLOR
{
	float4 texColor = tex2D(sampMainRT, In.TextureUV);

	float r = min(length(In.TextureUV - 0.5f) / hdrExposeRadius, 1.0f);
	float r2 = r*r;
	float r3 = r2*r;
	float weight = 1.0f - 3.0f*r2 + 2.0f*r3;

	float Lc = getRGBLum(texColor.rgb);
	float Lr = getRGB_S(texColor.rgb);
#if USE_LOG_LUM_AVERAGE	
	Lc = log(clamp(Lc * hdrPreScale, hdrLumMin, hdrLumMax));
	Lr = log(clamp(Lr * hdrPreScale, hdrLumMin, hdrLumMax));
	
	Lc = isfinite(Lc) ? Lc : 0.0f;
	Lr = isfinite(Lr) ? Lr : 0.0f;
#endif
	return float4(Lc, Lr, 0.0f, 0.0f) * weight;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

float EpsC(float logLaC)
{
	float logEpsC =
		(logLaC <= -2.6) ? -0.72 :
			(logLaC >= 1.9) ? (logLaC - 1.255) :
				(pow(0.249*logLaC + 0.65, 2.7) - 0.72);

	return pow(10.0, logEpsC);
}

float EpsR(float logLaR)
{
	float logEpsR =
		(logLaR <= -3.94) ? -2.86 :
			(logLaR >= -1.44) ? (logLaR - 0.395) :
				(pow(0.405*logLaR + 1.6, 2.18) - 2.86);

	return pow(10.0, logEpsR);
}

float4 PS_EvalLumScale(VS_OUTPUT In) : COLOR
{
	const float2 uv = float2(0.5f, 0.5f);

	float hdrScaleC_prev = tex2D(sampLumScalePrev, uv).b;
	float hdrScaleR_prev = tex2D(sampLumScalePrev, uv).a;

	const float weightScale = 1.0f / (3.1415967f * hdrExposeRadius * hdrExposeRadius * 0.3f);
	
	float LwaC = tex2D(sampAvgLum_1, uv).r * weightScale;
	float LwaR = tex2D(sampAvgLum_1, uv).g * weightScale;
#if USE_LOG_LUM_AVERAGE	
	LwaC = exp(LwaC);
	LwaR = exp(LwaR);
#else
	LwaC *= hdrPreScale;
	LwaR *= hdrPreScale;
#endif

    float EpsLdaC = EpsC( log10(hdrLdmax * hdrLevel) );
    float EpsLwaC = EpsC( log10(LwaC) );
    float EpsLwaR = EpsR( log10(LwaR) );

    float hdrScaleC_inst = EpsLdaC / (EpsLwaC * hdrLdmax);
    float hdrScaleR_inst = EpsLdaC / (EpsLwaR * hdrLdmax);

	float hdrScaleC = lerp(hdrScaleC_inst, hdrScaleC_prev, exp(-RelFrameTime / hdrTimeCones));
	float hdrScaleR = lerp(hdrScaleR_inst, hdrScaleR_prev, exp(-RelFrameTime / hdrTimeRod));

    float k = max((hdrLdmax - 0.25f*LwaR) / (hdrLdmax + LwaR), 0.0f);
	return float4(hdrScaleC * hdrPreScale, k * hdrScaleR * hdrPreScale, hdrScaleC, hdrScaleR);
}


float4 PS_CopyLumScale(VS_OUTPUT In) : COLOR
{
	const float2 uv = float2(0.5f, 0.5f);
	return tex2D(sampLumScale, uv);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

float4 PS_DrawFinal(VS_OUTPUT In) : COLOR
{
        //return tex2D(sampMainRT, In.TextureUV);

	float2 hdrScale = tex2D(sampLumScale, float2(0.5f, 0.5f)).rg;
	float hdrScaleC = hdrScale.r;
	float hdrScaleR = hdrScale.g;
	
	float4 color = tex2D(sampMainRT, In.TextureUV);
	color.a = getRGB_S(color.rgb);

	float4 res;
	res.rgb = (color.rgb * hdrScaleC) + (color.a * hdrScaleR) * hdrBlueShift;;
	res.a = 1.0f;
	return res;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct VS_OUTPUT_DS8
{
	float4 pos : POSITION;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
	float4 uv2 : TEXCOORD2;
	float4 uv3 : TEXCOORD3;
	float4 uv4 : TEXCOORD4;
	float4 uv5 : TEXCOORD5;
	float4 uv6 : TEXCOORD6;
	float4 uv7 : TEXCOORD7;
};

VS_OUTPUT_DS8 VS_Downsample8(uniform float2 texelSize, VS_INPUT In)
{
	VS_OUTPUT_DS8 Out;
	Out.pos = In.Position;

	float2 uv = In.TextureUV;
#ifdef D3D9_RASTER_OFFSET
	uv += 0.5f * texelSize;
#endif
	float4 texOrg = float4(uv, uv);
	float4 texAdd = float4(texelSize.xy, texelSize.xy) / 8.0f;

	Out.uv0.xywz = texOrg + texAdd * float4(-3, -3, -1, -3);
	Out.uv1.xywz = texOrg + texAdd * float4(-3, -1, -1, -1);
	Out.uv2.xywz = texOrg + texAdd * float4(+1, -3, +3, -3);
	Out.uv3.xywz = texOrg + texAdd * float4(+1, -1, +3, -1);
	Out.uv4.xywz = texOrg + texAdd * float4(-3, +1, -1, +1);
	Out.uv5.xywz = texOrg + texAdd * float4(-3, +3, -1, +3);
	Out.uv6.xywz = texOrg + texAdd * float4(+1, +1, +3, +1);
	Out.uv7.xywz = texOrg + texAdd * float4(+1, +3, +3, +3);
	return Out;
}

float4 PS_Downsample8(uniform sampler2D samp, VS_OUTPUT_DS8 In) : COLOR
{
	float4 res;
	res  = tex2D(samp, In.uv0.xy) + tex2D(samp, In.uv0.wz);
	res += tex2D(samp, In.uv1.xy) + tex2D(samp, In.uv1.wz);
	res += tex2D(samp, In.uv2.xy) + tex2D(samp, In.uv2.wz);
	res += tex2D(samp, In.uv3.xy) + tex2D(samp, In.uv3.wz);
	res += tex2D(samp, In.uv4.xy) + tex2D(samp, In.uv4.wz);
	res += tex2D(samp, In.uv5.xy) + tex2D(samp, In.uv5.wz);
	res += tex2D(samp, In.uv6.xy) + tex2D(samp, In.uv6.wz);
	res += tex2D(samp, In.uv7.xy) + tex2D(samp, In.uv7.wz);
	return res / 16;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct VS_OUTPUT_DS4
{
	float4 pos : POSITION;
	float4 uv0 : TEXCOORD0;
	float4 uv1 : TEXCOORD1;
};

VS_OUTPUT_DS4 VS_Downsample4(uniform float2 texelSize, VS_INPUT In)
{
	VS_OUTPUT_DS4 Out;
	Out.pos = In.Position;

	float2 uv = In.TextureUV;
#ifdef D3D9_RASTER_OFFSET
	uv += 0.5f * texelSize;
#endif
	float4 texOrg = float4(uv, uv);
	float4 texAdd = float4(texelSize.xy, texelSize.xy) / 4.0f;

	Out.uv0.xywz = texOrg + texAdd * float4(-1, -1, +1, -1);
	Out.uv1.xywz = texOrg + texAdd * float4(-1, +1, +1, +1);
	return Out;
}

float4 PS_Downsample4(uniform sampler2D samp, VS_OUTPUT_DS4 In) : COLOR
{
	float4 res;
	res  = tex2D(samp, In.uv0.xy) + tex2D(samp, In.uv0.wz);
	res += tex2D(samp, In.uv1.xy) + tex2D(samp, In.uv1.wz);
	return res / 4;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

float Script : STANDARDSGLOBAL <
	string UIWidget = "none";
	string ScriptClass = "scene";
	string ScriptOrder = "postprocess";
	string ScriptOutput = "color";
	string Script = "Technique=Main;";
> = 0.8;

bool bReset : FXCOMPOSER_RESETPULSE <
	string UIName = "Reset";
	string UIWidget = "none";
>;

// Standard full-screen imaging value
float4 ClearColor0 < string UIWidget = "None"; > = 0.0f;
float4 ClearColorBG < string UIWidget = "None"; > = {0.5,0.5,0.5,1.0};
float ClearDepth < string UIWidget = "None"; > = 1.0;


technique Main
<
	string Script =
			"LoopByCount=bReset;"
				"RenderColorTarget0=LumScalePrev;"
				"RenderDepthStencilTarget=AvgLumDS;"
				"ClearSetColor=ClearColor0;"
	        	"ClearSetDepth=ClearDepth;"
				"Clear=Color0;"
				"Clear=Depth;"
			"LoopEnd=;"
			"RenderColorTarget0=MainRT;"
	        "RenderDepthStencilTarget=;"
			"ClearSetColor=ClearColorBG;"
			"ClearSetDepth=ClearDepth;"
			"Clear=Color0;"
			"Clear=Depth0;"
			"ScriptExternal=color;"
        	"Pass=EvalAvgLum;"
			"Pass=Downsample8_64;"
			"Pass=Downsample8_8;"
			"Pass=Downsample8_1;"
        	"Pass=EvalLumScale;"
        	"Pass=DrawFinal;"
        	"Pass=CopyLumScale;";
>
{
	pass EvalAvgLum
	<
		string Script =
			"RenderColorTarget0=AvgLum_512;"
	        "RenderDepthStencilTarget=AvgLumDS;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_Main();
		PixelShader      = COMPILE_PS PS_EvalAvgLum();
		ZWriteEnable     = false;
		ZEnable          = false;
		CullMode         = None;
	}
	
#define PASS_DOWNSAMPLE8(size, samp) \
	pass Downsample8_##size \
	< \
		string Script = \
			"RenderColorTarget0=AvgLum_"#size";" \
	        "RenderDepthStencilTarget=AvgLumDS;" \
			"Draw=Buffer;"; \
	> \
	{ \
		VertexShader     = COMPILE_VS VS_Downsample8(1.0f.xx / size); \
		PixelShader      = COMPILE_PS PS_Downsample8(samp); \
		ZWriteEnable     = false; \
		ZEnable          = false; \
		CullMode         = None; \
	}

	PASS_DOWNSAMPLE8(64, sampAvgLum_512)
	PASS_DOWNSAMPLE8( 8, sampAvgLum_64)
	PASS_DOWNSAMPLE8( 1, sampAvgLum_8)
	
	pass EvalLumScale
	<
		string Script =
			"RenderColorTarget0=LumScale;"
	        "RenderDepthStencilTarget=AvgLumDS;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_Main();
		PixelShader      = COMPILE_PS PS_EvalLumScale();
		ZWriteEnable     = false;
		ZEnable          = false;
		CullMode         = None;
	}

	pass DrawFinal
	<
		string Script =
			"RenderColorTarget0=;"
	        "RenderDepthStencilTarget=;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_Main();
		PixelShader      = COMPILE_PS PS_DrawFinal();
		ZWriteEnable     = false;
		ZEnable          = false;
		CullMode         = None;
		
		SRGBWriteEnable  = true;
	}
	
	pass CopyLumScale
	<
		string Script =
			"RenderColorTarget0=LumScalePrev;"
	        "RenderDepthStencilTarget=AvgLumDS;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_Main();
		PixelShader      = COMPILE_PS PS_CopyLumScale();
		ZWriteEnable     = false;
		ZEnable          = false;
		CullMode         = None;
	}
}
