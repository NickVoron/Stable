cbuffer PerCamera : register(b0)
{
	float4x4 ProjTM;
	float4x4 ViewTM;
	float4 cameraPos;
};

cbuffer PerObject : register(b1)
{
	float4x4 WorldTM;
	float4 color;
};


Texture2D    tex2D			: register(t3);
SamplerState textureSampler	: register(s3);

struct TInputVertex
{
	float4 pos			: POSITION;
	float4 otherPos		: TEXCOORD0;	// Position of the other vertex at the other end of the line.
	float4 texOffset	: TEXCOORD1;	// Tex coord offset.
	float3 thickness	: TEXCOORD2;	// Thickness info.
};

struct TOutputVertex
{
	float4 hPos			: SV_POSITION;
	float blend			: COLOR0;														 
	float4 Norm_Depth	: TEXCOORD1;
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

	OUT.Norm_Depth.w = posstart.z;

	return OUT;
}

float4 PS( TOutputVertex IN ) : SV_TARGET0
{

	return float4(IN.Norm_Depth.w, 0, 0, 1);
}

