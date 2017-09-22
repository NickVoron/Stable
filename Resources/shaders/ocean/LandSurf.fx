
float GeomStep;
float UVStep;

float Level = 0;

float4 DebugColor;

float4 PosXVec = {1, 0, 0, 0};
float4 PosYVec = {0, 1, 0, 0};

float2 BlockOrg = {0,  0};

float4x4 ViewProjTM : WorldViewProjection;

float4 WorldEyePos;

float AlphaHeight = 1.0f;

float3 DirLightDirection;
float3 DirLightColor;
float3 AmbientColor;

texture TexHeight
<
	string Format = "R32F";
>;
texture TexSlope
<
	string Format = "G16R16F";
>;
texture TexColor;

sampler sampHeight = sampler_state
{
	texture = <TexHeight>;
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

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
};
sampler sampColor = sampler_state
{
	texture = <TexColor>;
	AddressU  = Wrap;
	AddressV  = Wrap;

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;

//	SRGBTexture = True;
};

texture TexClipDepth;

sampler sampClipDepth = sampler_state
{
	texture = <TexClipDepth>;
	AddressU  = Clamp;
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
    float4 SourceUV : TEXCOORD0;
    float3 WorldEyeVec : TEXCOORD1;
    float3 Normal : TEXCOORD2;
	float4 ScreenProjCoords : TEXCOORD3;
};

SOutput Draw_VS(SInput In)
{
    SOutput Out;

    float2 pos;
    pos.x = dot(In.Position, PosXVec);
    pos.y = dot(In.Position, PosYVec);
    
    float4 uv4;
    uv4.xy = (pos + BlockOrg) * UVStep;
    uv4.z = 0.0f;
    uv4.w = Level;
    
    float4 worldPos;
    worldPos.xz = (pos + BlockOrg) * GeomStep;
    
    const float w = 32;
    float AlphaOffset = (254/2 - 1 - w) * GeomStep;
    float OneOverWidth = 1.0f / (w * GeomStep);
    
    float2 alpha2 = saturate((abs(worldPos.xz - WorldEyePos.xz) - AlphaOffset)*OneOverWidth);
    float alpha = AlphaHeight * (1 - max(alpha2.x, alpha2.y));


    float h = tex2Dlod(sampHeight, uv4).x;
    
    
    worldPos.y = h;
    worldPos.w = 1.0f;

    Out.Position = mul(worldPos, ViewProjTM);
    //Out.Position = mul(ViewProjTM, worldPos);

    Out.Normal = float3(0, 1, 0);
    //Out.Alpha = alpha;
    Out.SourceUV = uv4;
    Out.SourceUV.w += (1.0f - alpha); //LOD bias
    
    Out.WorldEyeVec = worldPos - WorldEyePos.xyz;

	Out.ScreenProjCoords.xy = Out.Position.xy * float2(0.5f, -0.5f) + Out.Position.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Position.zw;
#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * InvViewportSize;
#endif
    return Out;
}


void Draw_PS(uniform bool doClip, SOutput In, out float4 color : COLOR0, out float4 depth : COLOR1)
{
	depth = In.ScreenProjCoords.w;
	if (doClip)
	{
		float clipDepth = tex2Dproj(sampClipDepth, In.ScreenProjCoords);
		clip(depth - clipDepth);
	}
	
	float2 slope = tex2Dlod(sampSlope, In.SourceUV).rg;

	float3 N;
	N.xz = slope.xy;
	N.y = 1.0f;
	
	N = normalize(N);
	
	float3 L = normalize(DirLightDirection);
	
	float diff = saturate(dot(L, N));
	
	float3 diffColor = tex2D(sampColor, In.SourceUV.xy).rgb;

    color.rgb = diffColor * 0.3f*(diff*DirLightColor + AmbientColor);
    color.a = 1.0;
	
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

technique Draw
{
    pass p0
    {
        VertexShader = compile vs_3_0 Draw_VS();
        PixelShader  = compile ps_3_0 Draw_PS(false);
   }
}

technique DrawClipped
{
    pass p0
    {
        VertexShader = compile vs_3_0 Draw_VS();
        PixelShader  = compile ps_3_0 Draw_PS(true);
   }
}
