#include "../common/globalConstants.fx"

float4 sunLightColor()
{
	return float4(0.7f, 0.7f, 0.7f, 1.0f);
}


float4 lightAmbient()
{
	return float4(0.3f, 0.3f, 0.3f, 1.0f);
}

#include "sails.fx"