struct STD_VS_OUTPUT
{
    float4 Pos  : POSITION;
	float2 UV	: TEXCOORD0;
};

texture colorTex;
texture depthTex;
texture colorDownsampled4xTex;
texture depthDownsampled4xTex;

sampler colorSampler				= sampler_state { texture = <colorTex>;				 AddressU = Clamp; AddressV = Clamp; MinFilter = Point; MagFilter = Linear; MipFilter = Linear;};
sampler depthSampler				= sampler_state { texture = <depthTex>;				 AddressU = Clamp; AddressV = Clamp; MinFilter = Point; MagFilter = Linear; MipFilter = Linear;};
sampler colorDownsampled4xSampler	= sampler_state { texture = <colorDownsampled4xTex>; AddressU = Clamp; AddressV = Clamp; MinFilter = Point; MagFilter = Linear; MipFilter = Linear;};
sampler depthDownsampled4xSampler	= sampler_state { texture = <depthDownsampled4xTex>; AddressU = Clamp; AddressV = Clamp; MinFilter = Point; MagFilter = Linear; MipFilter = Linear;};

float4 color(float2 uv) { return tex2D(colorSampler, uv); }
float4 depth(float2 uv) { return tex2D(depthSampler, uv); }
float4 color4X(float2 uv) { return tex2D(colorDownsampled4xSampler, uv); }
float4 depth4X(float2 uv) { return tex2D(depthDownsampled4xSampler, uv); }

float2 texelSize;


float3 grayscale(float3 c)
{
	return dot(c, float3(0.3f, 0.59f, 0.11f)); 
}

float4 grayscale(float4 c)
{
	c.rgb = grayscale(c.rgb);
	return c;
}

float __min_channel(float3 v)
{
    return min(min(v.x, v.y), v.z);
}

float __max_channel(float3 v)
{
	return max(max(v.x, v.y), v.z);
}

float3 rgb_to_hsv(float3 RGB)
{
    float3 HSV = (0.0).xxx;
    float minVal = __min_channel(RGB);
    float maxVal = __max_channel(RGB);
    float delta = maxVal - minVal;             //Delta RGB value
    HSV.z = maxVal;
    if (delta != 0) {                    // If gray, leave H & S at zero
       HSV.y = delta / maxVal;
       float3 delRGB;
       delRGB = ( ( ( maxVal.xxx - RGB ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
       if      ( RGB.x == maxVal ) HSV.x = delRGB.z - delRGB.y;
       else if ( RGB.y == maxVal ) HSV.x = ( 1.0/3.0) + delRGB.x - delRGB.z;
       else if ( RGB.z == maxVal ) HSV.x = ( 2.0/3.0) + delRGB.y - delRGB.x;
       if ( HSV.x < 0.0 ) { HSV.x += 1.0; }
       if ( HSV.x > 1.0 ) { HSV.x -= 1.0; }
    }
    return (HSV);
}

float3 hsv_to_rgb(float3 HSV)
{
    float3 RGB = HSV.z;
    if ( HSV.y != 0 ) {
       float var_h = HSV.x * 6;
       float var_i = floor(var_h);   // Or ... var_i = floor( var_h )
       float var_1 = HSV.z * (1.0 - HSV.y);
       float var_2 = HSV.z * (1.0 - HSV.y * (var_h-var_i));
       float var_3 = HSV.z * (1.0 - HSV.y * (1-(var_h-var_i)));
       if      (var_i == 0) { RGB = float3(HSV.z, var_3, var_1); }
       else if (var_i == 1) { RGB = float3(var_2, HSV.z, var_1); }
       else if (var_i == 2) { RGB = float3(var_1, HSV.z, var_3); }
       else if (var_i == 3) { RGB = float3(var_1, var_2, HSV.z); }
       else if (var_i == 4) { RGB = float3(var_3, var_1, HSV.z); }
       else                 { RGB = float3(HSV.z, var_1, var_2); }
   }
   return (RGB);
}

float4 oat_tonemap(float2 tc, float4 color)
{
	float fExposureLevel = 2.0f;
	tc -= 0.5f; // Put coords in -1/2 to 1/2 range
	// Square of distance from origin (center of screen)
	float vignette = 1 - dot(tc, tc);
	// Multiply by vignette to the fourth
// 	color *= vignette * vignette * vignette * vignette; 
// 	color *= fExposureLevel; // Apply simple exposure level
	//return color; // Apply gamma and return
	//color.xyz = 0.5f;
	float3 HSV = rgb_to_hsv(saturate(color.xyz));
	//HSV.y = 0.2f;
	//HSV.z *=1.5;
	HSV.z = pow(HSV.z, 1.15f)*1.3;
	color.xyz = (hsv_to_rgb(HSV));
	//color.xyz = HSV;
	return color;
	//return pow(color, 0.55f); // Apply gamma and return
}
