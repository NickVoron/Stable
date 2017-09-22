//  Modulate_Float
float Modulate_Float(float x, float y)
{
    return x - (int(x / y) * y);
}


//  New with 4.0 is a two-weight wind system that allows the tree model
//  to bend at more than one branch level.
float4 WindEffect_TwoWeights(float4 vPosition, float4 vWindInfo)
{
	return vPosition;
    // use the matrix offset to help keep indentical trees looking distinct - and
    // modulate back down to the range [0, NUM_WIND_MATRICES].
    //
    // note: the modulus operator has proven considerably difficult to use
    // on multiple platforms.  use care when modifying these two lines.
    int nWindIndex1 = int(vWindInfo.x + g_fWindMatrixOffset) % (NUM_WIND_MATRICES - 1);
    int nWindIndex2 = int(vWindInfo.z + g_fWindMatrixOffset) % (NUM_WIND_MATRICES - 1);
    
    // extract the wind weights
    float fWindWeight1 = vWindInfo.y;
    float fWindWeight2 = vWindInfo.w;
    
    // interpolate between still and full wind effect #1 by fWindWeight1
    float4 vWindEffect = lerp(vPosition, mul(vPosition, g_amWindMatrices[nWindIndex1]), fWindWeight1);
    
    // interpolate again between previous position and full wind effect #2 by fWindWeight2
    vWindEffect = lerp(vWindEffect, mul(vWindEffect, g_amWindMatrices[nWindIndex2]), fWindWeight2);
    
    return vWindEffect;
}


//  RotationMatrix_zAxis
//  Constructs a Z-axis rotation matrix
 float3x3 RotationMatrix_zAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(vSinCos.y, -vSinCos.x, 0.0f, 
                    vSinCos.x, vSinCos.y, 0.0f, 
                    0.0f, 0.0f, 1.0f);
}


//  RotationMatrix_yAxis
//  Constructs a Y-axis rotation matrix
float3x3 RotationMatrix_yAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(vSinCos.y, 0.0f, vSinCos.x,
                    0.0f, 1.0f, 0.0f,
                    -vSinCos.x, 0.0f, vSinCos.y);
}


//  RotationMatrix_xAxis
//  Constructs a X-axis rotation matrix
float3x3 RotationMatrix_xAxis(float fAngle)
{
    // compute sin/cos of fAngle
    float2 vSinCos;
    sincos(fAngle, vSinCos.x, vSinCos.y);
    
    return float3x3(1.0f, 0.0f, 0.0f,
                    0.0f, vSinCos.y, -vSinCos.x,
                    0.0f, vSinCos.x, vSinCos.y);
}


