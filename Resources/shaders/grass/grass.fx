/********************************************************************
	Grass.fx
	(c) 2008 Artyshock. All Rights Reserved.
	(c) 2008 Exceed Entertainment. All Rights Reserved.

	author:  Gipsle Roman

	description:    

*********************************************************************/

texture diffuseTex;
texture noiseTex;
texture animTex;
texture windTex;

float4 fadePlane;
//float time;

bool isAnimate;

float invIndexPitch;
float oscFactor;
float invCellSize;
float sampleOffset;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler DiffuseTextureSampler = sampler_state
{
    Texture = <diffuseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler NoiseTextureSampler = sampler_state
{
    Texture = <noiseTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler AnimTextureSampler = sampler_state
{
    Texture = <animTex>;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

sampler WindTextureSampler = sampler_state
{
    Texture = <windTex>;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

struct VS_GRASS_INPUT
{
	float3 pos	 		: POSITION;
	float2 tex0  		: TEXCOORD0;
	float1 index  		: TEXCOORD1;
	float4 posScale		: TEXCOORD2;
	float4 angles		: TEXCOORD3;
	float4 weights		: TEXCOORD4;
};

struct VS_SHADOW_GRASS_OUT
{
	float4 pos 			: POSITION0;
	float2 tex0 		: TEXCOORD0;
};

struct VS_DEPTH_GRASS_OUT
{
	float4 pos 			: POSITION;
	float2 tex0 		: TEXCOORD0;
	float Norm_Depth       : TEXCOORD1;
};

struct VS_MAIN_GRASS_OUT
{
	float4 pos 			: POSITION;
	float2 tex0 		: TEXCOORD0;
	float3 normal 		: TEXCOORD1;
	float3 wPos 		: TEXCOORD2;
	float4 color 		: COLOR0;
	float4 ScreenProjCoords : TEXCOORD3;
};


VS_DEPTH_GRASS_OUT VS_DepthL1(VS_GRASS_INPUT input)
{
	VS_DEPTH_GRASS_OUT output = (VS_DEPTH_GRASS_OUT)0;

	float4 pos;
	pos.x = dot(input.pos, input.angles.xwy);
	pos.y = input.pos.y;
	pos.z = dot(input.pos, input.angles.ywz);
	
	/*	
	if (isAnimate)
	{
		float2 windCoord = floor(input.posScale.xz * invCellSize) * sampleOffset;
		float4 w1 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y, 0.0f, 0.0f));
		float4 w2 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y, 0.0f, 0.0f));
		float4 w3 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y + sampleOffset, 0.0f, 0.0f));
		float4 w4 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y + sampleOffset, 0.0f, 0.0f));
		
		float4 row1 = float4(w1.x, w2.x, w3.x, w4.x);
		float4 row2 = float4(w1.y, w2.y, w3.y, w4.y);
		float4 row3 = float4(w1.z, w2.z, w3.z, w4.z);
		float4 row4 = float4(w1.w, w2.w, w3.w, w4.w);
		float4x4 windMat = float4x4(row1, row2, row3, row4);
		
		float4 wind = mul(windMat, input.weights);
		float2 windDirLocal = float2(dot(wind.xxx, input.angles.xwy), dot(wind.yyy, input.angles.ywz));
		
		float2 animCoord = float2(input.index * invIndexPitch, wind.z);
		float2 dispA, dispB;
		float k;
		
		if (windDirLocal.x >= 0.0f)
		{		
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				k = windDirLocal.x;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				k = -windDirLocal.y;
			}
		}
		else
		{
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				k = windDirLocal.y;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				k = -windDirLocal.x;
			}
		}
		
		float2 displace = sin(1.0f - k) * dispA + sin(k) * dispB;
		float3 anim = float3(wind.xy * displace.xx * cos(oscFactor * wind.w), displace.y);
		pos.xz += anim.xy;
		pos.y += anim.z;
	}*/
	
	pos.xyz = pos.xyz * input.posScale.w + input.posScale.xyz;
	pos.w = 1.0f;
	
	float4 posEyeSpace = mul(pos, View);
	output.pos = mul(posEyeSpace, Projection);
	output.Norm_Depth = posEyeSpace.z;
	output.tex0 = input.tex0;
	
	return output;
}

float4 PS_DepthL1(VS_DEPTH_GRASS_OUT input) : COLOR
{
	float a = tex2D(DiffuseTextureSampler, float2(1, 1) - input.tex0).a;
	float a1 = a*255 - 1.0f;
	clip(a1);
	//return 1;
	return float4(input.Norm_Depth, 0, 0, a);
}

VS_SHADOW_GRASS_OUT VS_ShadowL1(VS_GRASS_INPUT input)
{
	VS_SHADOW_GRASS_OUT output = (VS_SHADOW_GRASS_OUT)0;

	float4 pos;
	pos.x = dot(input.pos, input.angles.xwy);
	pos.y = input.pos.y;
	pos.z = dot(input.pos, input.angles.ywz);
	
	/*	
	if (isAnimate)
	{
		float2 windCoord = floor(input.posScale.xz * invCellSize) * sampleOffset;
		float4 w1 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y, 0.0f, 0.0f));
		float4 w2 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y, 0.0f, 0.0f));
		float4 w3 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y + sampleOffset, 0.0f, 0.0f));
		float4 w4 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y + sampleOffset, 0.0f, 0.0f));
		
		float4 row1 = float4(w1.x, w2.x, w3.x, w4.x);
		float4 row2 = float4(w1.y, w2.y, w3.y, w4.y);
		float4 row3 = float4(w1.z, w2.z, w3.z, w4.z);
		float4 row4 = float4(w1.w, w2.w, w3.w, w4.w);
		float4x4 windMat = float4x4(row1, row2, row3, row4);
		
		float4 wind = mul(windMat, input.weights);
		float2 windDirLocal = float2(dot(wind.xxx, input.angles.xwy), dot(wind.yyy, input.angles.ywz));
		
		float2 animCoord = float2(input.index * invIndexPitch, wind.z);
		float2 dispA, dispB;
		float k;
		
		if (windDirLocal.x >= 0.0f)
		{		
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				k = windDirLocal.x;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				k = -windDirLocal.y;
			}
		}
		else
		{
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				k = windDirLocal.y;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				k = -windDirLocal.x;
			}
		}
		
		float2 displace = sin(1.0f - k) * dispA + sin(k) * dispB;
		float3 anim = float3(wind.xy * displace.xx * cos(oscFactor * wind.w), displace.y);
		pos.xz += anim.xy;
		pos.y += anim.z;
	}
	*/
	pos.xyz = pos.xyz * input.posScale.w + input.posScale.xyz;
	pos.w = 1.0f;
	
	float4 posEyeSpace = mul(pos, View);
	output.pos = mul(posEyeSpace, Projection);

#ifdef D3D9_RASTER_OFFSET
	output.pos.x -= ScreenTexelUV.x * output.pos.w;
	output.pos.y += ScreenTexelUV.y * output.pos.w;
#endif

	output.tex0 = input.tex0;
	
	return output;
}

float4 PS_ShadowL1(VS_SHADOW_GRASS_OUT input) : COLOR
{
	float a = tex2D(DiffuseTextureSampler, float2(1, 1) - input.tex0).a;
	//return 1;
	return float4(0, 0, 0, a);
}

VS_MAIN_GRASS_OUT VS_MainL1(VS_GRASS_INPUT input)
{
	VS_MAIN_GRASS_OUT output = (VS_MAIN_GRASS_OUT)0;

	float3 normal = float3(0, 1, 0);
	float4 pos;
	pos.x = dot(input.pos, input.angles.xwy);
	pos.y = input.pos.y;
	pos.z = dot(input.pos, input.angles.ywz);

/*
	
	if (isAnimate)
	{
		float2 windCoord = floor(input.posScale.xz * invCellSize) * sampleOffset;
		float4 w1 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y, 0.0f, 0.0f));
		float4 w2 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y, 0.0f, 0.0f));
		float4 w3 = tex2Dlod(WindTextureSampler, float4(windCoord.x, windCoord.y + sampleOffset, 0.0f, 0.0f));
		float4 w4 = tex2Dlod(WindTextureSampler, float4(windCoord.x + sampleOffset, windCoord.y + sampleOffset, 0.0f, 0.0f));
		
		float4 row1 = float4(w1.x, w2.x, w3.x, w4.x);
		float4 row2 = float4(w1.y, w2.y, w3.y, w4.y);
		float4 row3 = float4(w1.z, w2.z, w3.z, w4.z);
		float4 row4 = float4(w1.w, w2.w, w3.w, w4.w);
		float4x4 windMat = float4x4(row1, row2, row3, row4);
		
		float4 wind = mul(windMat, input.weights);
		float2 windDirLocal = float2(dot(wind.xxx, input.angles.xwy), dot(wind.yyy, input.angles.ywz));
		
		float2 animCoord = float2(input.index * invIndexPitch, wind.z);
		float2 dispA, dispB;
		float k;
		
		if (windDirLocal.x >= 0.0f)
		{		
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				k = windDirLocal.x;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).xy;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				k = -windDirLocal.y;
			}
		}
		else
		{
			if (windDirLocal.y >= 0.0f)
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).xy;
				k = windDirLocal.y;
			}
			else
			{
				dispA = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y + 0.5f, 0.0f, 0.0f)).zw;
				dispB = tex2Dlod(AnimTextureSampler, float4(animCoord.x, animCoord.y, 0.0f, 0.0f)).zw;
				k = -windDirLocal.x;
			}
		}
		
		float2 displace = sin(1.0f - k) * dispA + sin(k) * dispB;
		float3 anim = float3(wind.xy * displace.xx * cos(oscFactor * wind.w), displace.y);
		pos.xz += anim.xy;
		pos.y += anim.z;
		normal.xz += anim.xy;
		normal.y += anim.z;
	}
	
	*/
	
	pos.xyz = pos.xyz * input.posScale.w + input.posScale.xyz;
	pos.w = 1.0f;
	
	output.normal = normal;
	output.wPos = pos / 1024.0f;
	
	float4 posEyeSpace = mul(pos, View);
	output.pos = mul(posEyeSpace, Projection);
	
	output.tex0 = input.tex0;
	float gray_factor = lerp(0.7f, 1.0f, input.angles.x);
	//float gray_factor = lerp(0.1f, 1.0f, pos.y);
	output.color = float4(gray_factor, gray_factor, gray_factor, 1.0f);	
	
	output.ScreenProjCoords.xy = output.pos.xy * float2(0.5f, -0.5f) + output.pos.ww * 0.5f;
	output.ScreenProjCoords.zw = output.pos.zw;
#ifdef D3D9_RASTER_OFFSET
	output.ScreenProjCoords.xy += output.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif
	
	return output;
}

float4 PS_MainL1(VS_MAIN_GRASS_OUT input) : COLOR
{
	float4 tex = tex2D(DiffuseTextureSampler, float2(1, 1) - input.tex0);
	float3 n = normalize(tex2D( sampNormal, input.wPos.xz) * 2.0f - 1.0f).xzy;

	//float3 n = normalize(input.normal);
	float nDotL = saturate( dot( n, sunLightDir) );
	float shadow = tex2Dproj(sampScreenShadow, input.ScreenProjCoords).r;

	float4 color = (shadow * sunLightColor() + lightAmbient()) * input.color * tex;
	color.a = tex.a;
	//color = tex;
	//color.a = 1.0f;

	//return float4(input.tex0, 0, 1);
	//return 1;
	return color;
}

#define fm solid

technique DepthGrassLod1
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		FillMode = fm;
		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_DepthL1();
        PixelShader  = compile ps_3_0 PS_DepthL1();
    }
}

technique ShadowGrassLod1
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		FillMode     = fm;
		
		//ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;

		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_ShadowL1();
        PixelShader  = compile ps_3_0 PS_ShadowL1();
    }
}

technique MainGrassLod1
{
    pass Pass0
    {
        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		Cullmode = NONE;
		
		FillMode     = fm;
		
		VertexShader = compile vs_3_0 VS_MainL1();
        PixelShader  = compile ps_3_0 PS_MainL1();
    }
}

technique DepthGrassLod2
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		FillMode = fm;
		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_DepthL1();
        PixelShader  = compile ps_3_0 PS_DepthL1();
    }
}

technique ShadowGrassLod2
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;

		
		FillMode = fm;
		
		//ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;

		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_ShadowL1();
        PixelShader  = compile ps_3_0 PS_ShadowL1();
    }
}

technique MainGrassLod2
{
    pass Pass0
    {
        ZWriteEnable     = false;
		ZEnable          = true;
		
		AlphaRef = 126;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		Cullmode = NONE;
		FillMode = fm;
		
		VertexShader = compile vs_3_0 VS_MainL1();
        PixelShader  = compile ps_3_0 PS_MainL1();
    }
}

technique DepthGrassLod3
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		FillMode = fm;
		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_DepthL1();
        PixelShader  = compile ps_3_0 PS_DepthL1();
    }
}

technique ShadowGrassLod3
{
    pass Pass0
    {
		ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;

		
		FillMode = fm;
		
		//ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;

		Cullmode = NONE;
		
		VertexShader = compile vs_3_0 VS_ShadowL1();
        PixelShader  = compile ps_3_0 PS_ShadowL1();
    }
}

technique MainGrassLod3
{
    pass Pass0
    {
        ZWriteEnable     = false;
		ZEnable          = true;
		
		AlphaRef = 126;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		Cullmode = NONE;
		FillMode = fm;
		
		VertexShader = compile vs_3_0 VS_MainL1();
        PixelShader  = compile ps_3_0 PS_MainL1();
    }
}

/*
VS_OUTPUT VS_MainL2(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4 pos;
	pos.x = dot(input.pos, input.angles.xwy);
	pos.y = input.pos.y;
	pos.z = dot(input.pos, input.angles.ywz);
	pos.xyz = pos.xyz * input.posScale.w + input.posScale.xyz;
	pos.w = 1.0f;
	
	output.wPos = mul(pos, WorldView);
	output.pos = mul(pos, WorldViewProj);
	output.tex0 = input.tex0;
	output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	output.normal = float3(0, 1, 0);
	
	return output;
}

float4 PS_MainL2(VS_OUTPUT input) : COLOR
{
	return tex2D(DiffuseTextureSampler, input.tex0) * input.color;
}

VS_OUTPUT VS_MainL3(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float4 pos;
	pos.x = dot(input.pos, input.angles.xwy);
	pos.y = input.pos.y;
	pos.z = dot(input.pos, input.angles.ywz);
	pos.xyz = pos.xyz * input.posScale.w + input.posScale.xyz;
	pos.w = 1.0f;
	
	output.wPos = mul(pos, WorldView);
	output.pos = mul(pos, WorldViewProj);
	output.tex0 = input.tex0;
	output.color.xyz = float3(1.0f, 1.0f, 1.0f);
	output.color.w = clamp(dot(fadePlane, pos), 0.0f, 1.0f);
	output.normal = float3(0, 1, 0);

	
	return output;
}

float4 PS_MainL3(VS_OUTPUT input) : COLOR
{
	return tex2D(DiffuseTextureSampler, input.tex0) * input.color;
}

technique GrassLod2
{
    pass Pass0
    {
		CullMode = None;
		AlphaBlendEnable = False;
		AlphaTestEnable = True;
		AlphaRef = 0x000000CF;
		AlphaFunc = GreaterEqual;
		ZFunc = Less;
		ZWriteEnable = True;
		
		VertexShader = compile vs_3_0 VS_MainL2();
        PixelShader  = compile ps_3_0 PS_MainL2();
    }
	
	pass Pass1
    {
		CullMode = None;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		AlphaTestEnable = False;
		ZFunc = Less;
		ZWriteEnable = False;
		
		VertexShader = compile vs_3_0 VS_MainL2();
        PixelShader  = compile ps_3_0 PS_MainL2();
    }
}

technique GrassLod3
{
    pass Pass0
    {
		CullMode = None;
		AlphaBlendEnable = False;
		AlphaTestEnable = True;
		AlphaRef = 0x0000002F;
		AlphaFunc = GreaterEqual;
		ZFunc = Less;
		ZWriteEnable = True;
		
		VertexShader = compile vs_3_0 VS_MainL3();
        PixelShader  = compile ps_3_0 PS_MainL3();
    }
	
	pass Pass1
    {
		CullMode = None;
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		AlphaTestEnable = False;
		ZFunc = Less;
		ZWriteEnable = False;
		
		VertexShader = compile vs_3_0 VS_MainL3();
        PixelShader  = compile ps_3_0 PS_MainL3();
    }
}*/