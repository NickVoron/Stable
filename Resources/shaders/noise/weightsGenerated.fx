#include "common.fx" 

float4 WeightsPS( float2 Tex  : TEXCOORD0  ) : COLOR
{
	float4 result = 0;
	float height = tex2D(heightsSam, Tex).r;
	float angle = getAngle(heightsSam, Tex, resultPixelSize);

	float value_height_0 = clamp( height, 0.0000, 0.3000 );
	float c_height_0 = ( value_height_0 - 0.0000 ) / ( 0.3000 - 0.0000 );
	float result_height_0 = lerp( 0.0000, 1.0000, c_height_0 );
	float value_angle_0 = clamp( angle, 0.0000, 3.1416 );
	float c_angle_0 = ( value_angle_0 - 0.0000 ) / ( 3.1416 - 0.0000 );
	float result_angle_0 = lerp( 1.0000, 0.0000, c_angle_0 );
	result.r = result_height_0 * result_angle_0;


	float value_height_1 = clamp( height, -1.0000, 0.0000 );
	float c_height_1 = ( value_height_1 - -1.0000 ) / ( 0.0000 - -1.0000 );
	float result_height_1 = lerp( 0.0000, 1.0000, c_height_1 );
	float value_angle_1 = clamp( angle, 0.0000, 3.1416 );
	float c_angle_1 = ( value_angle_1 - 0.0000 ) / ( 3.1416 - 0.0000 );
	float result_angle_1 = lerp( 1.0000, 0.0000, c_angle_1 );
	result.g = result_height_1 * result_angle_1;


	float value_height_2 = clamp( height, 0.3000, 1.0000 );
	float c_height_2 = ( value_height_2 - 0.3000 ) / ( 1.0000 - 0.3000 );
	float result_height_2 = lerp( 0.0000, 1.0000, c_height_2 );
	float value_angle_2 = clamp( angle, 0.0000, 3.1416 );
	float c_angle_2 = ( value_angle_2 - 0.0000 ) / ( 3.1416 - 0.0000 );
	float result_angle_2 = lerp( 1.0000, 0.0000, c_angle_2 );
	result.b = result_height_2 * result_angle_2;


	return result;
}

technique Weights
{
	pass P0
	{
		PixelShader  = compile ps_3_0 WeightsPS();
		ZEnable = false;
	}
}
