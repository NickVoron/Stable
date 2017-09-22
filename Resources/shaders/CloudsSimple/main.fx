float4x4 View;
float4x4 Projection;
float4 camPos;
float4 lightDir;
float4 lightSH;
float4 lightColor;
float4 tileSize;


texture cloudRefTex;
sampler cloudRef = sampler_state
{
	texture = <cloudRefTex>;
	MipFilter = point;
	MinFilter = point;
	MagFilter = point;
};
texture cloudPosTex;
sampler cloudPos = sampler_state
{
	texture = <cloudPosTex>;
	MipFilter = point;
	MinFilter = point;
	MagFilter = point;
};



texture Tex;
sampler samp = sampler_state
{
	texture = <Tex>;
	AddressU  = Clamp;
	AddressV  = Clamp;
	MipFilter = linear;
	MinFilter = linear;
	MagFilter = linear;
};

struct VS_INPUT
{
    float3 Pos  : POSITION;
    float4 partInst_RotFade : TEXCOORD0;
	float2 TexCoord0 : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
 	float2 TexCoord0 : TEXCOORD0;
	float fade : TEXCOORD1;
	float specular : TEXCOORD2;
	float4 SH : TEXCOORD3;
};

float2 unpack2(float v)
{
	float i1 = fmod( v, 4096.0f );
	float i0 = (v-i1) / 4096.0f;
	return float2( i0, i1 ) / 4096.0f;
}

VS_OUTPUT MainVS( VS_INPUT In)
{
	VS_OUTPUT Out;
	
	//float2 partInst = unpack2(In.partInst_RotFade.x);
	float particleIdx = In.partInst_RotFade.x;
	float instanceIdx = In.partInst_RotFade.z;
	
	float ppc_u;
	float ppc_v = modf(instanceIdx*tileSize.w, ppc_u);
	float4 cloudInst = tex2Dlod(cloudPos, float4(ppc_v+0.5f/tileSize.w, (ppc_u+0.5f)/tileSize.w, 0,0));

	float4 f0 = tex2Dlod(cloudRef, float4(particleIdx.x, cloudInst.r, 0,0));
	float3 WPos;
	WPos.xy = unpack2(f0.a) * 16.0f;
	
	float2 zSI = unpack2(f0.r);
	WPos.z = zSI.x * 16.0f;
	float tidx;
	float size = modf(zSI.y*16.0f, tidx)*4.0f;
	
	float3 wp;
	wp.xz = cloudInst.ab;
	wp.y = 10.0f;

	float2 rotFade = In.partInst_RotFade.yw;//unpack2(In.partInst_RotFade.y);


	wp.x += camPos.x - frac(camPos.x / tileSize.x)* tileSize.x;
	wp.z += camPos.z - frac(camPos.z / tileSize.z)* tileSize.z;
	//бокс облака не центрирован, поэтому добавим половину его размера - истинная координата центра облака
	//можно этого и не делать: отсечение по расстоянию будет чуть смещено, но не надо для каждого облака просовывать размер бокса
	float3 dx = camPos.xyz - wp;// - float3(4.0f,0.0f,4.0f);
	//дистанция видимости не более половины размера тайла!!!
	if(abs(dx.x)>tileSize.y)	wp.x += sign(dx.x)*tileSize.x;
	if(abs(dx.z)>tileSize.y)	wp.z += sign(dx.z)*tileSize.z;
	
	//if( abs(camPos.x - wp.x) > tileSize.y || abs(camPos.z - wp.z) > tileSize.y )	rotFade.y *= 0.1f;
	
	
	/*float earthRadius = 1000.0f;
	float3 earthCenter = float3(camPos.x, -earthRadius, camPos.z);
	wp += normalize(earthCenter - wp )*(distance(wp, earthCenter) - earthRadius);
	wp.y += 30.0f;*/
	
	WPos += wp;
	
	
	float Sin, Cos;
	sincos( rotFade.x*2.0f*3.1415926f, Sin, Cos );
	float3 Pos;
	Pos.x = In.Pos.x * Cos - In.Pos.y * Sin;
	Pos.y = In.Pos.x * Sin + In.Pos.y * Cos;
	Pos.z = In.Pos.z;

	float3 zdir = normalize(WPos - camPos.xyz);
	float3 ydir = float3(0,1,0);
	float3 xdir = normalize( cross(zdir, ydir) );
	ydir = cross(xdir, zdir);
	float3 D = size * mul( float3(-Pos.x, Pos.y, 0), float3x3(xdir, ydir, zdir) );
	float3 P = mul( float4( D + WPos, 1 ), View ).xyz;

	float3 PP = size *Pos + mul( float4( WPos, 1 ), (float4x3)View );
	
	float d2c = distance(WPos, camPos.xyz);
	if(d2c>2)	Out.fade = 1;
	else	Out.fade = d2c*0.33333 + 0.3333f;
	Out.fade *= rotFade.y;
	if(Out.fade>1.0f)	Out.fade = 1.0f;
	
	float k = 1 - d2c*2.0f;
	if(k<0)	k = 0;
	float3 pp = lerp(P, PP, k);
   	Out.Pos  = mul( float4( pp, 1 ), Projection );

	float2 texIdx;
	texIdx.x = modf(tidx*0.25f, texIdx.y)*4.0f;
	Out.TexCoord0 = ( In.TexCoord0 + texIdx ) * 0.25f;

	float ld = clamp( dot( normalize(WPos - camPos.xyz), lightDir.xyz ), 0, 1 );
	Out.specular = pow( ld, 100);
	
	Out.SH = float4( unpack2(f0.g), unpack2(f0.b) ) * 8.0f - 4.0f;
   	
    return Out;
}

float4 ambientCubeSH_0;
float4 ambientCubeSH_1;
float4 ambientCubeSH_2;

float4 MainPS( VS_OUTPUT In) : COLOR
{
	float3 ltAmb;
	ltAmb.b = dot(In.SH, ambientCubeSH_0);
	ltAmb.g = dot(In.SH, ambientCubeSH_1);
	ltAmb.r = dot(In.SH, ambientCubeSH_2);

	float shl = dot(In.SH, lightSH);
	float3 ltDir = shl * lightColor.xyz;

	float3 c = ltAmb+ 0.8f * ltDir * tex2D(samp, In.TexCoord0).rgb;


	float a = tex2D(samp, In.TexCoord0).a*In.fade;
	float spc = In.specular * pow(abs(shl),10);
	if(spc>1)
	{
		c += spc-1;
		//c = 0;	c.r = 1;
		if(spc>4)	spc = 4;
		a *= spc;
	}

	return float4( c , a );
}

technique Main
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = none;
		ZEnable = true;
		ZWriteEnable = false;
 		AlphaTestEnable = true;
 		AlphaBlendEnable = true;
 		SrcBlend = SRCALPHA;
 		DestBlend = INVSRCALPHA;
    }
}
