float4x4 world;
float4x4 view;
float4x4 proj;
float4 locCamPos;

texture Tex;
sampler densitySampler = sampler_state
{
	texture = <Tex>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	AddressW  = Clamp;
	MinFilter = linear;
	MagFilter = linear;
};
sampler shadowSampler = sampler_state
{
	texture = <Tex>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	AddressW  = Clamp;
	MinFilter = linear;
	MagFilter = linear;
};

texture permTexture;
sampler2D permTextureSam = sampler_state
{
	texture = <permTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
};


struct VS_INPUT
{
    float3 Pos  : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float3 wPos  : TEXCOORD0;
};

VS_OUTPUT MainVS( VS_INPUT In)
{
    VS_OUTPUT Out;
	float4 wp = mul(float4(In.Pos, 1), world);
	Out.Pos = mul(mul(wp, view), proj);
	Out.wPos = In.Pos;
    return Out;
}



#define ONE 0.00390625
#define ONEHALF 0.001953125
float fade(float t)
{
	//return t*t*(3.0-2.0*t); // Old fade
	return t*t*t*(t*(t*6.0-15.0)+10.0); // Improved fade
}

float noise(float3 P)
{
	float3 Pi = ONE*floor(P)+ONEHALF;

	float3 Pf = P-floor(P);

	// Noise contributions from (x=0, y=0), z=0 and z=1
	float perm00 = tex2D(permTextureSam, Pi.xy).a ;
	float3  grad000 = tex2D(permTextureSam, float2(perm00, Pi.z)).rgb * 4.0 - 1.0;
	float n000 = dot(grad000, Pf);
	float3  grad001 = tex2D(permTextureSam, float2(perm00, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n001 = dot(grad001, Pf - float3(0.0, 0.0, 1.0));

	// Noise contributions from (x=0, y=1), z=0 and z=1
	float perm01 = tex2D(permTextureSam, Pi.xy + float2(0.0, ONE)).a ;
	float3  grad010 = tex2D(permTextureSam, float2(perm01, Pi.z)).rgb * 4.0 - 1.0;
	float n010 = dot(grad010, Pf - float3(0.0, 1.0, 0.0));
	float3  grad011 = tex2D(permTextureSam, float2(perm01, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n011 = dot(grad011, Pf - float3(0.0, 1.0, 1.0));

	// Noise contributions from (x=1, y=0), z=0 and z=1
	float perm10 = tex2D(permTextureSam, Pi.xy + float2(ONE, 0.0)).a ;
	float3  grad100 = tex2D(permTextureSam, float2(perm10, Pi.z)).rgb * 4.0 - 1.0;
	float n100 = dot(grad100, Pf - float3(1.0, 0.0, 0.0));
	float3  grad101 = tex2D(permTextureSam, float2(perm10, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n101 = dot(grad101, Pf - float3(1.0, 0.0, 1.0));

	// Noise contributions from (x=1, y=1), z=0 and z=1
	float perm11 = tex2D(permTextureSam, Pi.xy + float2(ONE, ONE)).a ;
	float3  grad110 = tex2D(permTextureSam, float2(perm11, Pi.z)).rgb * 4.0 - 1.0;
	float n110 = dot(grad110, Pf - float3(1.0, 1.0, 0.0));
	float3  grad111 = tex2D(permTextureSam, float2(perm11, Pi.z + ONE)).rgb * 4.0 - 1.0;
	float n111 = dot(grad111, Pf - float3(1.0, 1.0, 1.0));

	// Blend contributions along x
	float4 n_x = lerp(float4(n000, n001, n010, n011), float4(n100, n101, n110, n111), fade(Pf.x));

	// Blend contributions along y
	float2 n_xy = lerp(n_x.xy, n_x.zw, fade(Pf.y));

	// Blend contributions along z
	float n_xyz = lerp(n_xy.x, n_xy.y, fade(Pf.z));

	return n_xyz;
}






float4 MainPS( VS_OUTPUT In) : COLOR
{
	float4 uv = float4(In.wPos, 0.0f);
	float3 dir = normalize(In.wPos - locCamPos.xyz);
	float mxd = max(abs(dir.z), max(abs(dir.x), abs(dir.y)));
	float scale = locCamPos.w*4;
	float4 dt = float4(dir/mxd/scale, 0.0f);

	float d = 1.0f;
	float3 c = float3(1,0,0);
	float3 n = float3(0.0f,0.0f,0.0f);
	float pw = pow(0.93f,256/scale);
	[loop]
	for(; d>0.01f; uv += dt)
	{
		float den = tex3Dlod( densitySampler, uv ).a;
		if(den>0.8f)
		{
			d *= pow(pw, den);
			c += tex3Dlod( shadowSampler, uv ).rgb*d;
			if(n.x==0.0f)	n = uv.xyz;
		}

		if(uv.x<0.0f || uv.x>1.0f || uv.y<0.0f || uv.y>1.0f || uv.z<0.0f || uv.z>1.0f)	break;
	}
	//if(d==1.0f)	d = 0.0f;
	c *= float3(0.0f,0.0f,1.0f) * (256/scale) * 0.1f/(1-d) * (1+0.03f*noise(n*20));
	float3 col = float3(1.0f, 0.9f, 0.8f)*c.b + float3(0.2f, 0.5f, 1.0f)*c.g + float3(1.0f, 0.4f, 0.2f)*c.r;
	return float4(col, 1-d);


}


technique Main
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = ccw;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaTestEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
    }
}
