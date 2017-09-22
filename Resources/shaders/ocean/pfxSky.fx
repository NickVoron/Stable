//#include "../mtl/inc/global.fxh"
#define COMPILE_VS compile vs_3_0
#define COMPILE_PS compile ps_3_0

#define D3D9_RASTER_OFFSET

float2 ViewportSize : VIEWPORTPIXELSIZE < string UIWidget = "None"; >;
static float2 ViewportInvSize = (float2(1.0f, 1.0f) / ViewportSize);

float4x4 WorldViewProjXf : WORLDVIEWPROJECTION;
float4x4 ProjXf : PROJECTION;
float4x4 ViewInvXf : VIEWINVERSE;

float3 LightDirection : DIRECTION <
	string UIName = "Light Direction";
	string Space = "World";
	string Object = "DirectionalLight";
> = { -0.5, -0.5, -0.5 };

///
static float LumScale = 0.01f;

static float3 SunDir = normalize(-LightDirection);

float Turbidity <
	string UIName = "Turbidity";
	string UIWidget = "Slider";
	float UIMin = 2.0;
	float UIMax = 6.0;
	float UIStep = 0.1;
> = 2.0f;

float3 GroundColor <
	string UIName = "Ground Color";
	string UIWidget = "Color";
> = float3(0.5, 0.5, 0.5);

//Zenith color

static float3x4 ZenithMtx_x = {
	{  0.00166, -0.00375,  0.00209,  0       },
	{ -0.02903,  0.06377, -0.03202,  0.00394 },
	{  0.11693, -0.21196,  0.06052,  0.25886 }
};
static float3x4 ZenithMtx_y = {
	{  0.00275, -0.00610,  0.00317,  0       },
	{ -0.04214,  0.08970, -0.04153,  0.00516 },
	{  0.15346, -0.26756,  0.06670,  0.26688 }
};

static float3 TurbidityVec = { Turbidity*Turbidity, Turbidity, 1.0f };

static float SunTheta = acos(SunDir.y);
static float4 SunThetaVec = { SunTheta*SunTheta*SunTheta, SunTheta*SunTheta, SunTheta, 1.0f };
static float Pi = acos(-1);

static float3 Zenith_xyY = {
	dot(TurbidityVec, mul(ZenithMtx_x, SunThetaVec)),
	dot(TurbidityVec, mul(ZenithMtx_y, SunThetaVec)),
	1000 * ((4.0453 * Turbidity - 4.9710) * tan((4.0 / 9 - Turbidity / 120)*(Pi - 2*SunTheta)) - 0.2155 * Turbidity + 2.4192)
};

//Perez function
static float3 A_xyY = {
	-0.0193 * Turbidity + -0.2592,
	-0.0167 * Turbidity + -0.2608,
	 0.1787 * Turbidity + -1.4630
};
static float3 B_xyY = {
	-0.0665 * Turbidity +  0.0008,
	-0.0950 * Turbidity +  0.0092,
	-0.3554 * Turbidity +  0.4275
};
static float3 C_xyY = {
	-0.0004 * Turbidity +  0.2125,
	-0.0079 * Turbidity +  0.2102,
	-0.0227 * Turbidity +  5.3251
};
static float3 D_xyY = {
	-0.0641 * Turbidity + -0.8989,
	-0.0441 * Turbidity + -1.6537,
	 0.1206 * Turbidity + -2.5771
};
static float3 E_xyY = {
	-0.0033 * Turbidity +  0.0452,
	-0.0109 * Turbidity +  0.0529,
	-0.0670 * Turbidity +  0.3703
};

float3 Perez_xyY(float cosTheta, float cosGamma)
{
	static float3 I_xyY = float3(1.0f, 1.0f, 1.0f);
	return (A_xyY * exp(B_xyY / cosTheta) + I_xyY) * 
		(C_xyY * exp(D_xyY * acos(cosGamma)) + 
			E_xyY * (cosGamma*cosGamma) + I_xyY);
}

static float3 Zenith_Perez_xyY = Perez_xyY(1.0f, SunDir.y);

//convert CIE xyY to sRGB
float3 xyY_to_sRGB(float3 xyY)
{
	float3 XYZ;
	XYZ.x = xyY.x / xyY.y;
	XYZ.y = 1.0f;
	XYZ.z = (1.0f - xyY.x - xyY.y) / xyY.y;
	XYZ *= xyY.z;

	float3 RGB;
	RGB.r = dot(float3( 3.2410, -1.5374, -0.4986), XYZ);
	RGB.g = dot(float3(-0.9692,  1.8760,  0.0416), XYZ);
	RGB.b = dot(float3( 0.0556, -0.2040,  1.0570), XYZ);
	return max(RGB, 0.0f);
}

float3 EvalSkyColor(float3 dir)
{
	float cosTheta = dir.y;
	float cosGamma = dot(dir, SunDir);
	
	float3 sky_xyY = Zenith_xyY * Perez_xyY(cosTheta, cosGamma) / Zenith_Perez_xyY;
	
	float3 skyColor = xyY_to_sRGB(sky_xyY) * LumScale;
	return (dir.y < 0.0f) ? GroundColor : skyColor;
}


///

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
	MagFilter = Point;
	MinFilter = Point;
	MipFilter = None;
};

texture	DepthRT : RENDERCOLORTARGET
<
	float2 ViewportRatio = {1, 1};
	int MipLevels = 1;
	string Format = "R32F";
	string UIWidget = "None";
>;
sampler2D sampDepthRT = sampler_state
{
	Texture = <DepthRT>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	MagFilter = Point;
	MinFilter = Point;
	MipFilter = None;
};

//--------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


struct	VS_INPUT_DEPTH
{
	float4 Position  : POSITION;
};

struct VS_OUTPUT_DEPTH
{
	float4 Position  : POSITION0;
	float Depth  : TEXCOORD0;
};

VS_OUTPUT_DEPTH VS_Depth(VS_INPUT_DEPTH In)
{
	VS_OUTPUT_DEPTH Out = (VS_OUTPUT_DEPTH)0;
	Out.Position  = mul(In.Position, WorldViewProjXf);
	Out.Depth = Out.Position.w;
	
	
	return Out;
}

float4 PS_Depth(VS_OUTPUT_DEPTH In) : COLOR
{
	return In.Depth;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

struct VS_OUTPUT_SKY
{
	float4 Position  : POSITION0;
	float2 TextureUV : TEXCOORD0;
	float3 RayVec    : TEXCOORD1;
};


VS_OUTPUT_SKY VS_DrawSky(VS_INPUT In)
{
	VS_OUTPUT_SKY Out = (VS_OUTPUT_SKY)0;
	Out.Position = In.Position;
	Out.Position.z = Out.Position.w; //set far plane
	Out.TextureUV = In.TextureUV;

	static float2 TanFOV_2 = float2(1.0f / ProjXf[0][0], 1.0f / ProjXf[1][1]);
	static float UnitZ = ProjXf[2][3];
	
	Out.RayVec = mul(float3(Out.Position.xy * TanFOV_2 / Out.Position.w, UnitZ), (float3x3)ViewInvXf);
	return Out;
}

float4 PS_DrawSky(VS_OUTPUT_SKY In) : COLOR
{
	float4 res;
	res.rgb = EvalSkyColor(normalize(In.RayVec));
	res.a = 1.0f;
	return res;
}

float4 PS_DrawAtt(VS_OUTPUT In) : COLOR
{
	float depth = tex2D(sampDepthRT, In.TextureUV).r;
	clip(depth - 1e-6);

	float4 res;
	res.rgb = tex2D(sampMainRT, In.TextureUV);
	res.a = 1.0f;
	return res;
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
			"Pass=Depth;"
			"RenderColorTarget0=MainRT;"
	        "RenderDepthStencilTarget=;"
			"ClearSetColor=ClearColorBG;"
			"Clear=Color0;"
			"ScriptExternal=color;"
        	"Pass=DrawSky;"
        	"Pass=DrawAtt;";
>
{
	pass DrawSky
	<
		string Script =
			"RenderColorTarget0=;"
	        "RenderDepthStencilTarget=;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_DrawSky();
		PixelShader      = COMPILE_PS PS_DrawSky();
		ZWriteEnable     = false;
		ZEnable          = true;
		CullMode         = None;
	}
	pass DrawAtt
	<
		string Script =
			"RenderColorTarget0=;"
	        "RenderDepthStencilTarget=;"
			"Draw=Buffer;";
	>
	{
		VertexShader     = COMPILE_VS VS_Main();
		PixelShader      = COMPILE_PS PS_DrawAtt();
		ZWriteEnable     = false;
		ZEnable          = false;
		CullMode         = None;
	}

	pass Depth
	<
		string Script =
			"RenderColorTarget0=DepthRT;"
	        "RenderDepthStencilTarget=;"
			"ClearSetColor=ClearColor0;"
			"ClearSetDepth=ClearDepth;"
			"Clear=Color0;"
			"Clear=Depth0;"
			"Draw=Geometry;";
	>
	{
		VertexShader     = COMPILE_VS VS_Depth();
		PixelShader      = COMPILE_PS PS_Depth();
		ZWriteEnable     = true;
		ZEnable          = true;
	}
}
