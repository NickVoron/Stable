cbuffer PerCamera : register(b0)
{
	float4x4 WorldTM;
	float4x4 ViewTM;
	float4x4 ProjTM;
}

Texture2D    tex2D			: register(t0);
SamplerState textureSampler	: register(s0);

struct TInputVertex
{
	float4 pos			: POSITION;
	float4 otherPos		: TEXCOORD0;	// Position of the other vertex at the other end of the line.
	float4 texOffset	: TEXCOORD1;	// Tex coord offset.
	float3 thickness	: TEXCOORD2;	// Thickness info.
};

struct TOutputVertex
{
	float4 hPos : SV_POSITION;
	float blend : COLOR0;														 
	float4 tex0 : TEXCOORD0;
	float4 tex1 : TEXCOORD1;
};

TOutputVertex VS(TInputVertex IN)
{
	TOutputVertex OUT;// = (TOutputVertex)0;

	float4x4 ModelView = mul(WorldTM, ViewTM);
	float4x4 ModelViewProj = mul(ModelView, ProjTM);

	// Transform 2 positions with modelview
	//
	float4 posstart = mul(IN.pos, ModelView);
	float4 posend = mul(IN.otherPos, ModelView);

	//unit vector between EYE and CENTER of the line (param4.x = 1/2)
	float3	middlepoint = normalize((posstart.xyz + posend.xyz) / 2.0);

	//unit vector of the line direction
	float3	lineoffset = posend.xyz - posstart.xyz;
	float3	linedir = normalize(lineoffset);
	float	sqlinelength = dot(lineoffset, lineoffset);

	//dot product in order to compute the texture coef
	float	texcoef = abs(dot(linedir, middlepoint));

	//change texture coef depending on line length: y=(Sz/(l^2))(x-1)+1
	texcoef = max(((texcoef - 1)*(sqlinelength / IN.thickness.z)) + 1, 0);

	//----------------------------------------------------------------------------

	//
	//modelview + projection on start and end points
	//
	posstart = mul(IN.pos, ModelViewProj);
	posend = mul(IN.otherPos, ModelViewProj);

	//perspective transform to get projected point
	float2 startpos2d = posstart.xy / posstart.w;
	float2 endpos2d = posend.xy / posend.w;

	//vector between these 2 points
	float2 linedir2d = normalize(startpos2d - endpos2d);

	//move corners with radius0 and radius1
	posstart.xy = ((texcoef * IN.texOffset.x) * linedir2d.xy) + posstart.xy; // horizontal

	linedir2d = IN.thickness.x * linedir2d;

	posstart.x = posstart.x + linedir2d.y; // vertical x
	posstart.y = posstart.y - linedir2d.x; // vertical y
	OUT.hPos = posstart;
	//--------------------------------------------------------------------------------
	//
	//compute tex coords depending on texcoef
	//
	float blend;
	float4 tex;
	tex.zw = float2(0, 1);
	tex.y = min(15.0 / 16.0, texcoef); // we don't want more than 15/16
	tex.x = modf(tex.y * 4.0, tex.y);
	blend = modf(tex.x * 4.0, tex.x); // integer part of tx * 4 and get the blend factor
	tex.xy = (tex.xy / 4.0) + (IN.texOffset).zw;
	OUT.tex0 = tex;
	//
	// now get the second texture coord : increment
	//
	tex.y = min(texcoef + (1.0 / 16.0), 15.0 / 16.0);
	tex.x = modf(tex.y * 4.0, tex.y);
	tex.x = floor(tex.x * 4.0);
	tex.xy = (tex.xy / 4) + (IN.texOffset).zw;
	OUT.tex1 = tex;

	OUT.blend = blend.xxxx;

	return OUT;
}

float4 PS( TOutputVertex IN ) : SV_TARGET0
{
	float4 blendFactor = IN.blend;
	float4 c0 = tex2D.Sample( textureSampler, IN.tex0 );
	float4 c1 = tex2D.Sample( textureSampler, IN.tex1 );
	float4 result = lerp( c0, c1, blendFactor );

	//result.a = length(result.xyz);

	//result.a = (result.a < 0.7) ? 0 : result.a;

	return result;
}

