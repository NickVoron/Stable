

texture     g_tBranchDiffuseMap;            // bark image, .rgb = diffuse color, .a = alpha noise map
texture     g_tBranchNormalMap;             // bark normal map, .rgb = xyz normals
texture     g_tBranchDetailMap;             // bark detail image, .rgb = color, .a = amount of detail to use

sampler2D samBranchDiffuseMap = sampler_state {	Texture = <g_tBranchDiffuseMap>;MaxAnisotropy = 16;	MinFilter = ANISOTROPIC; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP;};
sampler2D samBranchNormalMap = sampler_state  { Texture = <g_tBranchNormalMap>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP;};
sampler2D samBranchDetailMap = sampler_state  { Texture = <g_tBranchDetailMap>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC; MagFilter = LINEAR; MipFilter = LINEAR; AddressU = WRAP; AddressV = WRAP;};


//  Branch VS Ouput, PS Input
struct SShadowBranchOutput
{
    float4  vPosition               : POSITION0;
};


//  BranchVS
//  Branch geometry vertex shader
SShadowBranchOutput ShadowBranchVS(float3   vPosition               : POSITION,

					   float3   treePos	                : TEXCOORD0,
                       float2   rotScale                : TEXCOORD1,

                       float2   vDiffuseTexCoords       : TEXCOORD2,
                       float4   vWindParams             : TEXCOORD3,
                       float2   vSelfShadowTexCoords    : TEXCOORD4,
                       float2   vNormalTexCoords        : TEXCOORD5,
                       float2   vDetailTexCoords        : TEXCOORD6,
					   float3   vNormal                 : NORMAL
					   )
{
    // this will be fed to the branch pixel shader
    SShadowBranchOutput sOutput;

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;
    
    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotate = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotate = RotationMatrix_zAxis(rotScale.x);
#endif


    vPosition.xyz = mul(matRotate, vPosition.xyz);
	vPosition.xyz += treePos;

    // project position to the screen
	float4 eyeSpacePos = mul(float4(vPosition, 1), View);
	sOutput.vPosition = mul(eyeSpacePos, Projection);
    

#ifdef D3D9_RASTER_OFFSET
	sOutput.vPosition.x -= ScreenTexelUV.x * sOutput.vPosition.w;
	sOutput.vPosition.y += ScreenTexelUV.y * sOutput.vPosition.w;
#endif

    return sOutput;
}


//  BranchPS
//
//  Branch geometry pixel shader.  This shader essentially processes
//  four texture layers:
//      - diffuse bark map
//      - normal map
//      - detail map (lerped against the diffuse map using detail alpha layer)
//      - shadow map (composite map generated via SpeedTreeCAD)
float4 ShadowBranchPS(SShadowBranchOutput In) : COLOR
{
    return 0;
}

//  Branch VS Ouput, PS Input
struct SDepthBranchOutput
{
    float4  vPosition               : POSITION0;
	float Norm_Depth       : TEXCOORD0;
};
//  BranchVS
//  Branch geometry vertex shader
SDepthBranchOutput DepthBranchVS(float3   vPosition               : POSITION,

						   float3   treePos	                : TEXCOORD0,
						   float2   rotScale                : TEXCOORD1,

						   float2   vDiffuseTexCoords       : TEXCOORD2,
						   float4   vWindParams             : TEXCOORD3,
						   float2   vSelfShadowTexCoords    : TEXCOORD4,
						   float2   vNormalTexCoords        : TEXCOORD5,
						   float2   vDetailTexCoords        : TEXCOORD6,
						   float3   vNormal                 : NORMAL
					   )
{
    // this will be fed to the branch pixel shader
    SDepthBranchOutput sOutput;

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;
    
    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotate = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotate = RotationMatrix_zAxis(rotScale.x);
#endif

    vPosition.xyz = mul(matRotate, vPosition.xyz);
	vPosition.xyz += treePos;
    
	float4 posEyeSpace = mul(float4(vPosition, 1), View);
    sOutput.vPosition = mul(posEyeSpace, Projection);
	sOutput.Norm_Depth     = posEyeSpace.z;

    return sOutput;
}


//  BranchPS
//
//  Branch geometry pixel shader.  This shader essentially processes
//  four texture layers:
//      - diffuse bark map
//      - normal map
//      - detail map (lerped against the diffuse map using detail alpha layer)
//      - shadow map (composite map generated via SpeedTreeCAD)
float4 DepthBranchPS(SDepthBranchOutput In) : COLOR
{
    return float4(In.Norm_Depth, 0, 0, 0);
}


//  BranchVS
//  Branch geometry vertex shader
//  Branch VS Ouput, PS Input
struct SMainBranchOutput
{
    float4  vPosition               : POSITION;
    float2  vDiffuseTexCoords       : TEXCOORD0;
    float2  vSelfShadowTexCoords    : TEXCOORD1;
    float2  vNormalTexCoords        : TEXCOORD2;
    float2  vDetailTexCoords        : TEXCOORD3;
    float3  vNormal                 : TEXCOORD4;
	float4 ScreenProjCoords			: TEXCOORD5;
	float4 fog				: TEXCOORD6;
    float3 vViewWS          : TEXCOORD7;   // view vector in world space, denormalized

};
SMainBranchOutput MainBranchVS(float3   vPosition               : POSITION,

					   float3   treePos	                : TEXCOORD0,
                       float2   rotScale                : TEXCOORD1,

                       float2   vDiffuseTexCoords       : TEXCOORD2,
                       float4   vWindParams             : TEXCOORD3,
                       float2   vSelfShadowTexCoords    : TEXCOORD4,
                       float2   vNormalTexCoords        : TEXCOORD5,
                       float2   vDetailTexCoords        : TEXCOORD6,
					   float3   vNormal                 : NORMAL
					   )
{
    // this will be fed to the branch pixel shader
    SMainBranchOutput sOutput;

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;
    
    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotate = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotate = RotationMatrix_zAxis(rotScale.x);
#endif

	float3 vBinormal = 1;
	float3 vTangent = 1;


    // tree instance rotation is handled by rotating the position, normal,
    // binormal, and tangent before the tree is translated into position
    vPosition.xyz = mul(matRotate, vPosition.xyz);
    vNormal.xyz = mul(matRotate, vNormal.xyz);
    vBinormal.xyz = mul(matRotate, vBinormal.xyz);
    vTangent.xyz = mul(matRotate, vTangent.xyz);
    
    // compute new position from wind effect
    // note: improved accuracy may be achieved by also transforming the normal,
    //       binary, and tangents by the wind matrix, but this was judged too 
    //       expensive for the minimal effect it achieved.
    //vPosition = WindEffect_TwoWeights(vPosition, vWindParams);
    
    // translate tree into position (must be done after the rotation)
	vPosition.xyz += treePos;

    // setup per-pixel normal mapping  (assumes normalized light direction)
    sOutput.vNormal.x = dot(sunLightDir.xyz, vTangent.xyz);
    sOutput.vNormal.y = dot(sunLightDir.xyz, vBinormal.xyz);
    sOutput.vNormal.z = dot(sunLightDir.xyz, vNormal.xyz);
	
    // move self-shadow texture coordinates based on wind strength
    sOutput.vSelfShadowTexCoords = vSelfShadowTexCoords + g_vSelfShadowMotion;
    
    sOutput.vViewWS = (cameraPosition - float4(vPosition, 1)).xyz;
	
    // project position to the screen
    sOutput.vPosition = mul(float4(vPosition, 1), View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
	
	// туман
	float sf = computeSolidFog(sOutput.vPosition.z, minMaxDist);
	sOutput.fog = float4(sf, vPosition.y, sOutput.vPosition.z, 1);
    
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;
    sOutput.vNormalTexCoords = vNormalTexCoords;
    sOutput.vDetailTexCoords = vDetailTexCoords;

	sOutput.ScreenProjCoords.xy = sOutput.vPosition.xy * float2(0.5f, -0.5f) + sOutput.vPosition.ww * 0.5f;
	sOutput.ScreenProjCoords.zw = sOutput.vPosition.zw;
#ifdef D3D9_RASTER_OFFSET
	sOutput.ScreenProjCoords.xy += sOutput.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

    return sOutput;
}


//  BranchPS
//
//  Branch geometry pixel shader.  This shader essentially processes
//  four texture layers:
//      - diffuse bark map
//      - normal map
//      - detail map (lerped against the diffuse map using detail alpha layer)
//      - shadow map (composite map generated via SpeedTreeCAD)
float4 MainBranchPS(SMainBranchOutput In, out float4 depth : COLOR1) : COLOR
{
	//return 1;
    // look up the diffuse and normal-map layers
    float4 texDiffuse = tex2D(samBranchDiffuseMap, In.vDiffuseTexCoords);

    //return texDiffuse;

	float4 texNormal = tex2D(samBranchNormalMap, In.vNormalTexCoords);
      
    // look up the self shadow texture layer (greyscale)
    //float4 texShadow = tex2D(samSelfShadowMap, In.vSelfShadowTexCoords);
    
    // if branch detail layer is active, look it up and lerp between it
    // and the base layer (alllows for a smooth transition between diffuse
    // and detail layers if the detail layer's alpha map is done correctly -
    // the diffuse map's alpha layer doesn't figure into it)
    float4 texDetail = tex2D(samBranchDetailMap, In.vDetailTexCoords);
    texDiffuse.rgb = lerp(texDiffuse.rgb, texDetail.rgb, texDetail.a);
    
    // since the normal map normal values (normally ranged [-1,1]) are store
    // as a color value (ranged [0,1]), they must be uncompressed.  a dot product 
    // is used to compute the diffuse lighting contribution.
    const float3 vHalves = { 0.5f, 0.5f, 0.5f };
    float fDot = saturate(dot(2.0f * (texNormal.rgb - vHalves), In.vNormal.rgb));

    // compute the ambient contribution (pulled from the diffuse map)
    float3 vAmbient = texDiffuse.xyz * g_vMaterialAmbient.xyz;
    
    // compute the full lighting equation, including diffuse and ambient values and
    // their respective scales.
    //vColor = float4(saturate(texDiffuse.rgb * fDot * g_fDiffuseScale * texShadow.xyz * sunLightColor.rgb + lightAmbient), texDiffuse.a);
	float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
    //float4 outColor =  float4((shadow * fDot + lightAmbient()) * texDiffuse.rgb * sunLightColor().rgb, 1);
    float4 outColor;
	outColor.rgb =  texDiffuse*(fDot*shadow*sunLightColor().rgb + lightAmbient());
    
    //float4 fc = computeSkyColor( dot( normalize(In.vViewWS) , -sunLightDir) ) * 1500.0f * sunLightColor();
	//outColor = lerp(outColor, fc, In.fog.x);
	outColor.a = 1;

//	outColor.rgb = (shadow * fDot + lightAmbient());
//	outColor.rgb = texDiffuse.rgb;
	// * sunLightColor().rgb;

	depth = In.ScreenProjCoords.w;
	
    return outColor;
}



//  Techniques
technique MainBranches
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainBranchVS( );
        PS_COMPILE_COMMAND MainBranchPS( );

		ZWriteEnable     = false;
		ZEnable          = true;
		Cullmode = CCW;
    }
}

technique OceanBranches
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainBranchVS( );
        PS_COMPILE_COMMAND MainBranchPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		Cullmode = CCW;
    }
}

technique ShadowBranches
{
    pass P0
    {          
        VS_COMPILE_COMMAND ShadowBranchVS( );
        PS_COMPILE_COMMAND ShadowBranchPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 2.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;
    }
}

technique DepthBranches
{
    pass P0
    {          
        VS_COMPILE_COMMAND DepthBranchVS( );
        PS_COMPILE_COMMAND DepthBranchPS( );
		
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
    }
}