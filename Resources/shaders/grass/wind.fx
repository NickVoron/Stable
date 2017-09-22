/********************************************************************
	Wind.fx
	(c) 2008 Artyshock. All Rights Reserved.
	(c) 2008 Exceed Entertainment. All Rights Reserved.

	author:  Gipsle Roman

	description:    

*********************************************************************/

texture windTex;
texture ppTex;
float time;
float invMaxForce;
float windForceThreshold;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler WindTextureSampler = sampler_state
{
    Texture = <windTex>;
    MipFilter = POINT;
    MinFilter = POINT;
    MagFilter = POINT;
};

sampler PPTextureSampler = sampler_state
{
    Texture = <ppTex>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PS_UpdateFluid(in float2 texCoord : TEXCOORD0) : COLOR
{
	float4 retVal;
	float2 windDir = tex2D(WindTextureSampler, texCoord);
	float4 request = tex2D(PPTextureSampler, texCoord);
	
	float windForce = saturate((windDir.x * windDir.x + windDir.y * windDir.y) * invMaxForce);

	if(windForce > windForceThreshold)
	{
		retVal.w = 0.0f;
	}
	else
	{
		windForce = windForceThreshold;
		retVal.w = request.w + time;
	}
	
	retVal.xy = normalize(windDir.xy);
	retVal.z = windForce * 0.49f;	// константу не менять!!!

	return retVal;
}

float4 PS_WindChecker(in float2 texCoord : TEXCOORD0) : COLOR
{
	return tex2D(PPTextureSampler, texCoord);
}

technique UpdateFluid
{
    pass Pass0
    {
		PixelShader  = compile ps_3_0 PS_UpdateFluid();
    }
}

technique WindChecker
{
    pass Pass0
    {
		PixelShader  = compile ps_3_0 PS_WindChecker();
    }
}