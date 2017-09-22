

struct SFrondOutput
{
    float4 vPosition            : POSITION;
    float2 vDiffuseTexCoords    : TEXCOORD0;
    float2 vSelfShadowTexCoords : TEXCOORD1;
    float4 vColor               : COLOR0;
    float4 ScreenProjCoords		: TEXCOORD2;
    float4 fog				: TEXCOORD3;
    float3 vViewWS          : TEXCOORD4;   // view vector in world space, denormalized


};

struct SShadowFrondOutput
{
    float4  vPosition          : POSITION0;
    float2 vDiffuseTexCoords   : TEXCOORD0;
};

struct SDepthFrondOutput
{
    float4  vPosition      : POSITION0;
    float2 vDiffuseTexCoords   : TEXCOORD0;
	float Norm_Depth       : TEXCOORD1;
};



///////////////////////////////////////////////////////////////////////  
//  FrondVS
//
//  Frond geometry vertex shader
    
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

	sOutput.vViewWS = (cameraPosition - vPosition).xyz;

    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
	
	// туман
	float sf = computeSolidFog(sOutput.vPosition.z, minMaxDist);
	sOutput.fog = float4(sf, vPosition.y, sOutput.vPosition.z, 1);

    
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;
    
    sOutput.ScreenProjCoords.xy = sOutput.vPosition.xy * float2(0.5f, -0.5f) + sOutput.vPosition.ww * 0.5f;
	sOutput.ScreenProjCoords.zw = sOutput.vPosition.zw;
#ifdef D3D9_RASTER_OFFSET
	sOutput.ScreenProjCoords.xy += sOutput.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif
    
    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  FrondPS

float4 MainFrondPS(SFrondOutput In, out float4 depth : COLOR1) : COLOR
{
    // look up the diffuse layer
    float4 texDiffuse = tex2D(samFrondsMap, In.vDiffuseTexCoords.xy);
	//return texDiffuse;
    
    // look up the self shadow texture layer (greyscale)
    //float4 texShadow = tex2D(samSelfShadowMap, In.vSelfShadowTexCoords.xy);
    
    // compute the ambient contribution (pulled from the diffuse map)
    float3 vAmbient = texDiffuse.xyz * g_vMaterialAmbient.xyz;

    // compute the full lighting equation, including diffuse and ambient values and
    // their respective scales.
    float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;

    float4 outColor = float4(saturate((In.vColor.rgb * shadow + lightAmbient())* texDiffuse.rgb * g_fDiffuseScale), In.vColor.a * texDiffuse.a);

	float4 fc = computeSkyColor( dot( normalize(In.vViewWS) , -sunLightDir) ) * 1500.0f * sunLightColor();
	
	outColor.rgb = texDiffuse.rgb * ( shadow*In.vColor.rgb*sunLightColor() + lightAmbient() );
	//outColor.rgb = texDiffuse.rgb * ( In.vColor.rgb*sunLightColor() + lightAmbient() );
	//outColor.rgb =  texDiffuse;

	//outColor.a = In.vColor.a * texDiffuse.a;
	outColor.a = texDiffuse.a;

	depth = In.ScreenProjCoords.w;
	
    return outColor;
}



///////////////////////////////////////////////////////////////////////  
//  FrondVS
//
//  Frond geometry vertex shader
    
SDepthFrondOutput DepthFrondVS(	float4     vPosition               : POSITION,
							 float4     vColor                  : COLOR0,

							 float3   treePos	                : TEXCOORD0,
							 float2   rotScale					: TEXCOORD1,

							 float2     vDiffuseTexCoords       : TEXCOORD2,
							 float4     vWindParams             : TEXCOORD3,
							 float2     vSelfShadowTexCoords    : TEXCOORD4,
							 float3     vNormal                 : NORMAL)
{
    // this will be fed to the frond pixel shader
    SDepthFrondOutput sOutput;

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

  
    float4 posEyeSpace = mul(vPosition, View);
    sOutput.vPosition = mul(posEyeSpace, Projection);
	sOutput.Norm_Depth     = posEyeSpace.z;
    
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;
    
    
    
    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  FrondPS

float4 DepthFrondPS(SDepthFrondOutput In) : COLOR
{
    float a = tex2D(samFrondsMap, In.vDiffuseTexCoords.xy).a;
    return float4(In.Norm_Depth, 0, 0, a);
}


///////////////////////////////////////////////////////////////////////  
//  FrondVS
//
//  Frond geometry vertex shader
    
SShadowFrondOutput ShadowFrondVS(float4     vPosition               : POSITION,
							 float4     vColor                  : COLOR0,

							 float3   treePos	                : TEXCOORD0,
							 float2   rotScale					: TEXCOORD1,

							 float2     vDiffuseTexCoords       : TEXCOORD2,
							 float4     vWindParams             : TEXCOORD3,
							 float2     vSelfShadowTexCoords    : TEXCOORD4,
							 float3     vNormal                 : NORMAL)
{
    // this will be fed to the frond pixel shader
    SShadowFrondOutput sOutput;

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

  // project position to the screen
	float4 eyeSpacePos = mul(vPosition, View);
	sOutput.vPosition = mul(eyeSpacePos, Projection);
    

#ifdef D3D9_RASTER_OFFSET
	sOutput.vPosition.x -= ScreenTexelUV.x * sOutput.vPosition.w;
	sOutput.vPosition.y += ScreenTexelUV.y * sOutput.vPosition.w;
#endif
    
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;

    
    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  FrondPS

float4 ShadowFrondPS(SShadowFrondOutput In) : COLOR
{
	float a = tex2D(samFrondsMap, In.vDiffuseTexCoords.xy).a;
    return float4(0,0,0, a);
}

//  Techniques
technique MainFrond
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainFrondVS( );
        PS_COMPILE_COMMAND MainFrondPS( );

        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 126;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
    }
}

technique OceanFrond
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainFrondVS( );
        PS_COMPILE_COMMAND MainFrondPS( );

        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 126;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
    }
}


//  Techniques
technique ShadowFrond
{
    pass P0
    {          
        VS_COMPILE_COMMAND ShadowFrondVS( );
        PS_COMPILE_COMMAND ShadowFrondPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		ZFunc			= Less;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		AlphaRef = 126;
		AlphaFunc = Greater;

		
		FillMode         = Solid;
		
		//ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;

		Cullmode = NONE;
    }
}

//  Techniques
technique DepthFrond
{
    pass P0
    {          
        VS_COMPILE_COMMAND DepthFrondVS( );
        PS_COMPILE_COMMAND DepthFrondPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		
		FillMode = Solid;
		Cullmode = NONE;
    }
}
