float4      g_avLeafAngles[MAX_NUM_LEAF_ANGLES]; // each element: .x = rock angle, .y = rustle angle
                                                 // each element is a float4, even though only a float2 is needed, to facilitate
                                                 // fast uploads on all platforms (one call to upload whole array)
float4      g_vLeafAngleScalars;                 // each tree model has unique scalar values: .x = rock scalar, .y = rustle scalar
float4x3    g_mLeafUnitSquare =                  // unit leaf card that's turned towards the camera and wind-rocked/rustled by the
            {                                    // vertex shader.  card is aligned on YZ plane and centered at (0.0f, 0.0f, 0.0f)
                float3( 0.5f,  0.5f, 0.0f), 
                float3(-0.5f,  0.5f, 0.0f), 
                float3(-0.5f, -0.5f, 0.0f), 
                float3( 0.5f, -0.5f, 0.0f)
            };

//  Leaf VS Ouput, PS Input

struct SLeafOutput
{
    float4 vPosition        : POSITION;
    float2 vBaseTexCoords   : TEXCOORD0;
    float4 vColor           : COLOR0;
    float4 vNormal          : TEXCOORD1;
};


SLeafOutput MainLeafCardVS(float4   vPosition   : POSITION,
                       float4   vColor      : COLOR0,
					   					   
					   float3   treePos	    : TEXCOORD0,
					   float2   rotScale	: TEXCOORD1,

                       float3   vTexCoord0  : TEXCOORD2,    // .xy = diffuse texcoords, .z = corner index
                       float4   vGeom       : TEXCOORD3,    // .x = width, .y = height, .z = pivot x, .w = pivot.y
                       float4   vTexCoord2  : TEXCOORD4,    // .x = angle.x, .y = angle.y, .z = wind angle index, .w = dimming
                       float4   vTexCoord3  : TEXCOORD5,    // .x = wind_index1, .y = wind_weight1, .z = wind_index2, .w = wind_weight2
                       float3   vNormal     : NORMAL)
{
    // this will be fed to the leaf pixel shader
    SLeafOutput sOutput;
    
    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fAzimuth = g_vCameraAngles.x;                      // camera azimuth, but no effect if used for leaf meshes
    float fPitch = g_vCameraAngles.y;                        // camera pitch, but no effect if used for leaf meshes
    int nCorner = vTexCoord0.z;                              // which card corner this vertex represents [0,3]
    float fRotAngleX = vTexCoord2.x;                         // angle offset for leaf rocking (helps make it distinct)
    float fRotAngleY = vTexCoord2.y;                         // angle offset for leaf rustling (helps make it distinct)
    float fWindAngleIndex = vTexCoord2.z;                    // which wind matrix this leaf card will follow
    float2 vPivotPoint = vGeom.zw;                           // point about which card will rock and rustle
    float fDimming = vTexCoord2.w;                           // interior leaves are darker (range = [0.0,1.0])

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;
    

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotateInstance = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotateInstance = RotationMatrix_zAxis(rotScale.x);
#endif

    // tree instance rotation is handled by rotating the position, normal,
    // binormal, and tangent before the tree is translated into position
    vPosition.xyz = mul(matRotateInstance, vPosition.xyz);
    vNormal.xyz = mul(matRotateInstance, vNormal.xyz);
    
    // compute new position from wind effect
    // note: improved accuracy may be achieved by also transforming the normal
    //       by the wind matrix, but this was judged too expensive for the 
    //       minimal effect it achieved.
    vPosition = WindEffect_TwoWeights( vPosition, vTexCoord3);
    
    // compute rock and rustle values
    float2 vLeafRockAndRustle = g_vLeafAngleScalars.xy * g_avLeafAngles[fWindAngleIndex].xy;;

    // orient, rock, and rustle the leaf card
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_yAxis(fAzimuth + fRotAngleY);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_zAxis(fRotAngleX + fPitch + vLeafRockAndRustle.y));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(-vLeafRockAndRustle.x));

    // perturb normal for wind effect (optional - not previewed or tunable in CAD)
    /*
#ifdef UPVECTOR_POS_Y
    vNormal.xyz += 0.15f * vLeafRockAndRustle.xxy;
#else
    vNormal.xyz += 0.15f * vLeafRockAndRustle.xyx;
#endif
    vNormal = normalize(vNormal);
    */
        
    // access g_mLeafUnitSquare matrix with corner index and apply scales
    float3 vPivotedPoint = g_mLeafUnitSquare[nCorner].zyx * rotScale.y;
    // adjust by pivot point so rotation occurs around the correct point
    float3 vCorner = vPivotedPoint * vGeom.xyx;


    // apply composite rotation (billboard + rock/rustle) to scaled corner
    vPosition.xyz += mul(matRotation, vCorner);

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;
    
    // compute the leaf lighting (not using normal mapping, but per-vertex lighting)
    //sOutput.vColor.rgb = fDimming * LightDiffse(vPosition.xyz, vNormal.xyz, sunLightDir, sunLightColor.rgb, g_vMaterialDiffuse.rgb);
    sOutput.vColor.rgb = saturate(dot(vNormal.xyz, sunLightDir)) + lightAmbient;
    sOutput.vColor.a = 1.0f;

    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;
    sOutput.vNormal = float4(vNormal, 1);
    

    return sOutput;
}


//  LeafMeshVS
//  Leaf mesh geometry vertex shader
SLeafOutput MainLeafMeshVS(float4   vPosition   : POSITION,
						   float4   vColor      : COLOR0,
						   
						   float3   treePos	    : TEXCOORD0,
						   float2   rotScale	: TEXCOORD1,

						   float4   vTexCoord0  : TEXCOORD2,    // .xy = diffuse texcoords, .z = wind angle index, .w = dimming
						   float4   vWindParams : TEXCOORD3,    // .x = wind_index1, .y = wind_weight1, .z = wind_index2, .w = wind_weight2
						   float3   vOrientX    : TEXCOORD4,    // tangent
						   float3   vOrientY    : TEXCOORD5,    // binormal
						   float3   vOrientZ    : TEXCOORD6,    // normal
						   float3   vOffset     : TEXCOORD7,    // .xyz = mesh placement position
						   float3   vNormal     : NORMAL)
{
    // this will be fed to the leaf pixel shader
    SLeafOutput sOutput;
    
    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fWindAngleIndex = vTexCoord0.z;       // which wind matrix this leaf card will follow
    float fDimming = vTexCoord0.w;              // interior leaves are darker (range = [0.0,1.0])

	//vPosition.xyz -= vOffset.xyz;

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)

	float3x3 matRotateInstance = RotationMatrix_yAxis(rotScale.x);

    
    // compute new position from wind effect
    // note: improved accuracy may be achieved by also transforming the normal
    //       by the wind matrix, but this was judged too expensive for the 
    //       minimal effect it achieved.
    vOffset.xyz *= rotScale.y;
    vOffset.xyz = mul(matRotateInstance, vOffset.xyz);
    vOffset.xyz = WindEffect_TwoWeights(float4(vOffset.xyz, 1.0f), vWindParams).xyz;
    
    // compute rock and rustle values
    float2 vLeafRockAndRustle = g_vLeafAngleScalars.xy * g_avLeafAngles[fWindAngleIndex].xy;
    
    // orient to adjust for camera azimuth
    float3x3 matRockRustle = RotationMatrix_zAxis(vLeafRockAndRustle.y);
    
    // leaf rustle
    matRockRustle = mul(matRockRustle, RotationMatrix_xAxis(vLeafRockAndRustle.x));
    vPosition.xyz = mul(matRockRustle, vPosition.xyz);
    
    // build mesh orientation matrix 
    float3x3 matOrientMesh =
    {
        vOrientX, vOrientY, vOrientZ
    };
    
    // apply orientation matrix to the mesh
    vPosition.xyz = mul(matOrientMesh, vPosition.xyz);
    vNormal.xyz = mul(matOrientMesh, vNormal.xyz);

    // tree instance rotation is handled by rotating the position, normal,
    // binormal, and tangent before the tree is translated into position
    vPosition.xyz = mul(matRotateInstance, vPosition.xyz);
    vNormal.xyz = mul(matRotateInstance, vNormal.xyz);

    vPosition.xyz += vOffset.xyz;

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;

    sOutput.vColor.rgb = saturate(dot(vNormal.xyz, sunLightDir));
    sOutput.vColor.a = 1.0f;
  
    sOutput.vPosition = mul(vPosition, View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
    
    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = float2(vTexCoord0.x, -vTexCoord0.y);
    sOutput.vNormal = float4(vNormal, 1);

    
	return sOutput;
}

//  LeafPS
float4 MainLeafPS(  float4 vPosition        : POSITION,
					float2 vBaseTexCoords   : TEXCOORD0,
					float4 vColor           : COLOR0,
					float4 vNormal          : TEXCOORD1) : COLOR
{
	//return 1;
    // look up the diffuse layer
    float4 texDiffuse = tex2D(samLeavesMap, vBaseTexCoords.xy);
    
    float3 normal = vNormal.xyz;
    
    if(renderBillboard)
    {
		if(colorPass) return texDiffuse;
		return float4((normal + 1.0f) / 2.0f, texDiffuse.a);
    }
    
   
    float nDotL = saturate( dot(normal.xyz, normalize(sunLightDir) ) );
   
    float4 resColor;
    resColor.rgb = (nDotL + lightAmbient.rgb) * texDiffuse.rgb;
    resColor.a = texDiffuse.a;
    return resColor;
    
}

technique MainLeafCard
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafCardVS( );
        PS_COMPILE_COMMAND MainLeafPS( );

        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 90;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
		//FillMode = Point;
    }
}

technique MainLeafMesh
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafMeshVS( );
        PS_COMPILE_COMMAND MainLeafPS( );
        
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

