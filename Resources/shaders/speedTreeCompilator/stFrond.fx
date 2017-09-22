

struct SFrondOutput
{
    float4 vPosition            : POSITION;
    float2 vDiffuseTexCoords    : TEXCOORD0;
    float2 vSelfShadowTexCoords : TEXCOORD1;
    float4 vColor               : COLOR0;
    float3 vNormal              : TEXCOORD2;

};

SFrondOutput MainFrondVS(float4     vPosition               : POSITION,
						 float4     vColor                  : COLOR0,

						 float3   treePos	                : TEXCOORD0,
						 float2   rotScale					: TEXCOORD1,

						 float2     vDiffuseTexCoords       : TEXCOORD2,
						 float4     vWindParams             : TEXCOORD3,
						 float2     vSelfShadowTexCoords    : TEXCOORD4,
						 float3     vNormal                 : NORMAL)
{
    // this will be fed to the frond pixel shader
    SFrondOutput sOutput;

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotate = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotate = RotationMatrix_zAxis(rotScale.x);
#endif

    // tree instance rotation is handled by rotating the position, normal,
    // binormal, and tangent before the tree is translated into position
    vPosition.xyz = mul(matRotate, vPosition.xyz);
    vNormal.xyz = mul(matRotate, vNormal.xyz);
    
    // compute new position from wind effect
    // note: improved accuracy may be achieved by also transforming the normal
    //       by the wind matrix, but this was judged too expensive for the 
    //       minimal effect it achieved.
    vPosition = WindEffect_TwoWeights(vPosition, vWindParams);
    
    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;

    // compute the frond lighting (not using normal mapping, but per-vertex
    // lighting)
   // sOutput.vColor.rgb = LightDiffse(vPosition.xyz, vNormal.xyz, sunLightDir, sunLightColor, g_vMaterialDiffuse);
    sOutput.vColor.rgb = saturate(dot(vNormal.xyz, sunLightDir));
    sOutput.vColor.a = 1.0f;
    
    // move self-shadow texture coordinates based on wind strength
    sOutput.vSelfShadowTexCoords = vSelfShadowTexCoords + g_vSelfShadowMotion;;

    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
	
   
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;
    sOutput.vNormal = vNormal;

    
   
    return sOutput;
}

float4 MainFrondPS( float4 vPosition            : POSITION,
					float2 vDiffuseTexCoords    : TEXCOORD0,
					float2 vSelfShadowTexCoords : TEXCOORD1,
					float4 vColor               : COLOR0,
					float3 vNormal              : TEXCOORD2,
					float vFace : VFACE
					) : COLOR
{
	//return 1;
    float4 texDiffuse = tex2D(samFrondsMap, vDiffuseTexCoords.xy);
    float3 normal = vNormal;
    
    if(renderBillboard)
    {
		if(colorPass) return texDiffuse;
		return float4((normal + 1.0f) / 2.0f, texDiffuse.a);
    }
    
    float nDotL = saturate( dot(normal.xyz, normalize(sunLightDir)) );
    if(vFace < 0) nDotL *= 0.5f;
    float4 resColor;
    resColor.rgb = (nDotL + lightAmbient.rgb) * texDiffuse.rgb;
    resColor.a = texDiffuse.a;
    return resColor;
}

technique MainFrond
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainFrondVS( );
        PS_COMPILE_COMMAND MainFrondPS( );

        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
		//FillMode = Wireframe;
    }
}