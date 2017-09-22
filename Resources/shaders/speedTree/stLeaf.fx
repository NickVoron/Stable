float4      g_avLeafAngles[MAX_NUM_LEAF_ANGLES]; // each element: .x = rock angle, .y = rustle angle
                                                 // each element is a float4, even though only a float2 is needed, to facilitate
                                                 // fast uploads on all platforms (one call to upload whole array)
float4      g_vLeafAngleScalars;                 // each tree model has unique scalar values: .x = rock scalar, .y = rustle scalar
float4x4    g_mLeafUnitSquare =                  // unit leaf card that's turned towards the camera and wind-rocked/rustled by the
            {                                    // vertex shader.  card is aligned on YZ plane and centered at (0.0f, 0.0f, 0.0f)
                float4(0.0f, 0.5f, 0.5f, 0.0f), 
                float4(0.0f, -0.5f, 0.5f, 0.0f), 
                float4(0.0f, -0.5f, -0.5f, 0.0f), 
                float4(0.0f, 0.5f, -0.5f, 0.0f)
            };

//  Leaf VS Ouput, PS Input

struct SLeafOutput
{
    float4 vPosition        : POSITION;
    float2 vBaseTexCoords   : TEXCOORD0;
    float4 vColor           : COLOR0;
    float4 ScreenProjCoords	: TEXCOORD1;
    float4 fog				: TEXCOORD2;
    float3 vViewWS          : TEXCOORD3;   // view vector in world space, denormalized

};

struct SShadowLeafOutput
{
    float4  vPosition               : POSITION0;
    float2 vBaseTexCoords   : TEXCOORD0;
};

struct SDepthLeafOutput
{
    float4  vPosition      : POSITION0;
    float2 vBaseTexCoords   : TEXCOORD0;
	float Norm_Depth       : TEXCOORD1;
};


//  LeafCardVS
//  Leaf card geometry vertex shader
SShadowLeafOutput ShadowLeafCardVS(float4   vPosition   : POSITION,
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
    SShadowLeafOutput sOutput;


    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fAzimuth = g_vCameraAngles.x;                      // camera azimuth, but no effect if used for leaf meshes
    float fPitch = g_vCameraAngles.y;                        // camera pitch, but no effect if used for leaf meshes
    float fWidth = vGeom.x;                                  // final width of leaf card
    float fHeight = vGeom.y;                                 // final height of leaf card
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
#ifdef UPVECTOR_POS_Y
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_yAxis(fAzimuth + fRotAngleY);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_zAxis(fRotAngleX + fPitch + vLeafRockAndRustle.y));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(-vLeafRockAndRustle.x));
#else
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_zAxis(fAzimuth + fRotAngleX + vLeafRockAndRustle.y);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_yAxis(fPitch + fRotAngleY));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(vLeafRockAndRustle.x));
#endif

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
    float3 vPivotedPoint = g_mLeafUnitSquare[nCorner].xzy * rotScale.y;

    // adjust by pivot point so rotation occurs around the correct point
    vPivotedPoint.yz += vPivotPoint;

    float3 vCorner = vPivotedPoint * vGeom.xyx;


    // apply composite rotation (billboard + rock/rustle) to scaled corner
    vPosition.xyz += mul(matRotation, vCorner);

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;
    
	float4 eyeSpacePos = mul(vPosition, View);
	sOutput.vPosition = mul(eyeSpacePos, Projection);
    

#ifdef D3D9_RASTER_OFFSET
	sOutput.vPosition.x -= ScreenTexelUV.x * sOutput.vPosition.w;
	sOutput.vPosition.y += ScreenTexelUV.y * sOutput.vPosition.w;
#endif
    
    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;

    return sOutput;
}


//  LeafMeshVS
//  Leaf mesh geometry vertex shader
SShadowLeafOutput ShadowLeafMeshVS(float4   vPosition   : POSITION,
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
    SShadowLeafOutput sOutput;
    
    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fWindAngleIndex = vTexCoord0.z;       // which wind matrix this leaf card will follow

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotateInstance = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotateInstance = RotationMatrix_zAxis(rotScale.x);
#endif
    
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

    // tree instance rotation is handled by rotating the position, normal,
    // binormal, and tangent before the tree is translated into position
    vPosition.xyz = mul(matRotateInstance, vPosition.xyz);
    vPosition.xyz += vOffset.xyz;

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;
    
	float4 eyeSpacePos = mul(vPosition, View);
	sOutput.vPosition = mul(eyeSpacePos, Projection);
    

#ifdef D3D9_RASTER_OFFSET
	sOutput.vPosition.x -= ScreenTexelUV.x * sOutput.vPosition.w;
	sOutput.vPosition.y += ScreenTexelUV.y * sOutput.vPosition.w;
#endif

    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;

    return sOutput;
}








//  LeafCardVS
//  Leaf card geometry vertex shader
SDepthLeafOutput DepthLeafCardVS(float4   vPosition   : POSITION,
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
    SDepthLeafOutput sOutput;


    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fAzimuth = g_vCameraAngles.x;                      // camera azimuth, but no effect if used for leaf meshes
    float fPitch = g_vCameraAngles.y;                        // camera pitch, but no effect if used for leaf meshes
    float fWidth = vGeom.x;                                  // final width of leaf card
    float fHeight = vGeom.y;                                 // final height of leaf card
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
#ifdef UPVECTOR_POS_Y
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_yAxis(fAzimuth + fRotAngleY);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_zAxis(fRotAngleX + fPitch + vLeafRockAndRustle.y));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(-vLeafRockAndRustle.x));
#else
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_zAxis(fAzimuth + fRotAngleX + vLeafRockAndRustle.y);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_yAxis(fPitch + fRotAngleY));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(vLeafRockAndRustle.x));
#endif

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
    float3 vPivotedPoint = g_mLeafUnitSquare[nCorner].xzy * rotScale.y;
    // adjust by pivot point so rotation occurs around the correct point
    vPivotedPoint.yz += vPivotPoint;
    float3 vCorner = vPivotedPoint * vGeom.xyx;


    // apply composite rotation (billboard + rock/rustle) to scaled corner
    vPosition.xyz += mul(matRotation, vCorner);

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;
    
    float4 posEyeSpace = mul(float4(vPosition.xyz, 1), View);
    sOutput.vPosition = mul(posEyeSpace, Projection);
	sOutput.Norm_Depth     = posEyeSpace.z;
    
    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;
    
    return sOutput;
}


//  LeafMeshVS
//  Leaf mesh geometry vertex shader
SDepthLeafOutput DepthLeafMeshVS(float4   vPosition   : POSITION,
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
    SDepthLeafOutput sOutput;
    //sOutput.Norm_Depth = 1;
    
    // since there are so many leaf geometry attributes coming in, it because necessary
    // to assign usable names to each of the various .xyzw values coming in (free as compiler
    // optimizes to use original values)
    float fWindAngleIndex = vTexCoord0.z;       // which wind matrix this leaf card will follow
    float fDimming = vTexCoord0.w;              // interior leaves are darker (range = [0.0,1.0])

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotateInstance = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotateInstance = RotationMatrix_zAxis(rotScale.x);
#endif
    
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
    
    // compute the leaf lighting (not using normal mapping, but per-vertex lighting)
    //sOutput.vColor.rgb = LightDiffse(vPosition.xyz, vNormal.xyz, sunLightDir, sunLightColor.rgb, g_vMaterialDiffuse.rgb);

    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
    
    
    float4 posEyeSpace = mul(float4(vPosition.xyz, 1), View);
    sOutput.vPosition = mul(posEyeSpace, Projection);
	sOutput.Norm_Depth     = posEyeSpace.z;
	
	// pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;

    return sOutput;
}


//  LeafCardVS
//  Leaf card geometry vertex shader
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
    float fWidth = vGeom.x;                                  // final width of leaf card
    float fHeight = vGeom.y;                                 // final height of leaf card
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
#ifdef UPVECTOR_POS_Y
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_yAxis(fAzimuth + fRotAngleY);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_zAxis(fRotAngleX + fPitch + vLeafRockAndRustle.y));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(-vLeafRockAndRustle.x));
#else
    // orient to adjust for camera azimuth
    float3x3 matRotation = RotationMatrix_zAxis(fAzimuth + fRotAngleX + vLeafRockAndRustle.y);
    // orient to adjust for camera pitch + leaf rock
    matRotation = mul(matRotation, RotationMatrix_yAxis(fPitch + fRotAngleY));
    // leaf rustle
    matRotation = mul(matRotation, RotationMatrix_xAxis(vLeafRockAndRustle.x));
#endif

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
    float3 vPivotedPoint = g_mLeafUnitSquare[nCorner].xzy * rotScale.y;

    // adjust by pivot point so rotation occurs around the correct point
    vPivotedPoint.yz += vPivotPoint;
    float3 vCorner = vPivotedPoint * vGeom.xyx;

    // apply composite rotation (billboard + rock/rustle) to scaled corner
    vPosition.xyz += mul(matRotation, vCorner);

    // translate tree into position (must be done after the rotation)
    vPosition.xyz += treePos;
    
    // compute the leaf lighting (not using normal mapping, but per-vertex lighting)
    //sOutput.vColor.rgb = fDimming * LightDiffse(vPosition.xyz, vNormal.xyz, sunLightDir, sunLightColor.rgb, g_vMaterialDiffuse.rgb);
    sOutput.vColor.rgb = saturate(dot(vNormal.xyz, sunLightDir));
    sOutput.vColor.a = 1.0f;
    
   // sOutput.vColor.rgb = vNormal.xyz;

    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
    
    // Compute initial parallax displacement direction:
	sOutput.vViewWS = (cameraPosition - sOutput.vPosition).xyz;


	sOutput.vPosition = mul(sOutput.vPosition, Projection );
    
    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;
    
       
    sOutput.ScreenProjCoords.xy = sOutput.vPosition.xy * float2(0.5f, -0.5f) + sOutput.vPosition.ww * 0.5f;
	sOutput.ScreenProjCoords.zw = sOutput.vPosition.zw;
#ifdef D3D9_RASTER_OFFSET
	sOutput.ScreenProjCoords.xy += sOutput.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

	// туман
	float sf = computeSolidFog(sOutput.vPosition.z, minMaxDist);
	sOutput.fog = float4(sf, vPosition.y, sOutput.vPosition.z, 1);
	
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

    // scale the geometry (each tree is uniquely scaled)
    vPosition.xyz *= rotScale.y;

    // each tree is uniquely rotated around the up-axis, so we build a 
    // rotation matrix for around the up axis (yet to be optimized)
#ifdef UPVECTOR_POS_Y
    float3x3 matRotateInstance = RotationMatrix_yAxis(rotScale.x);
#else
    float3x3 matRotateInstance = RotationMatrix_zAxis(rotScale.x);
#endif
    
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
    
    // compute the leaf lighting (not using normal mapping, but per-vertex lighting)
    //sOutput.vColor.rgb = LightDiffse(vPosition.xyz, vNormal.xyz, sunLightDir, sunLightColor.rgb, g_vMaterialDiffuse.rgb);
    sOutput.vColor.rgb = saturate(dot(vNormal.xyz, sunLightDir));
    sOutput.vColor.a = 1.0f;
    
   // sOutput.vColor.rgb = vNormal.xyz;

    
    // Compute initial parallax displacement direction:
	sOutput.vViewWS = (cameraPosition - vPosition).xyz;


    // project position to the screen
    sOutput.vPosition = mul(vPosition, View );
    
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
    
    // pass through other texcoords exactly as they were received
    sOutput.vBaseTexCoords.xy = vTexCoord0.xy;
    
    sOutput.ScreenProjCoords.xy = sOutput.vPosition.xy * float2(0.5f, -0.5f) + sOutput.vPosition.ww * 0.5f;
	sOutput.ScreenProjCoords.zw = sOutput.vPosition.zw;
#ifdef D3D9_RASTER_OFFSET
	sOutput.ScreenProjCoords.xy += sOutput.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

	// туман
	float sf = computeSolidFog(sOutput.vPosition.z, minMaxDist);
	sOutput.fog = float4(sf, vPosition.y, sOutput.vPosition.z, 1);

    return sOutput;
}


//  LeafPS
float4 DepthLeafPS(SDepthLeafOutput In) : COLOR
{
	float a = tex2D(samLeavesMap, In.vBaseTexCoords.xy).a;
	return float4(In.Norm_Depth, 0, 0, a);
}

//  LeafPS
float4 ShadowLeafPS(SLeafOutput In) : COLOR
{
	float a = tex2D(samLeavesMap, In.vBaseTexCoords.xy).a;
	//clip(0.5f - In.vColor.r);
	//if(In.vColor.r < 0.3f)	a = 0.0f;
    return float4(0,0,0, a);
}

//  LeafPS
float4 MainLeafPS(SLeafOutput In) : COLOR
{/*
    float4 texDiffuse = tex2D(samLeavesMap, In.vBaseTexCoords.xy);
    float4 outColor;
	outColor.rgb =  texDiffuse.rgb * ( In.vColor.rgb *sunLightColor().rgb + lightAmbient().rgb );
	outColor.a = 1;*/

    float4 texDiffuse = tex2D(samLeavesMap, In.vBaseTexCoords.xy);
    float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
    float4 outColor;
//	if(In.vColor.r > 0.5f) shadow = 1.0f;
	//shadow = (shadow + 1.0f) / 2.0f;
	outColor.rgb = texDiffuse.rgb * 
		( In.vColor.rgb*sunLightColor().rgb * 1 + 
		lightAmbient().rgb );
	//outColor.rgb = In.vColor.rgb;
	outColor.a = In.vColor.a * texDiffuse.a;
    return outColor;
}

float4 OceanLeafPS(SLeafOutput In, out float4 depth : COLOR1) : COLOR
{
    float4 texDiffuse = tex2D(samLeavesMap, In.vBaseTexCoords.xy);
    float shadow = tex2Dproj(sampScreenShadow, In.ScreenProjCoords).r;
    float4 outColor;
	outColor.rgb = texDiffuse.rgb * ( shadow*In.vColor.rgb*sunLightColor().rgb + lightAmbient().rgb );
	outColor.a = In.vColor.a * texDiffuse.a;
	depth = In.ScreenProjCoords.w;
    return outColor;
}


//  Techniques
technique ShadowLeafCard
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafCardVS( );
        PS_COMPILE_COMMAND ShadowLeafPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;
    }
}

//  Techniques
technique DepthLeafCard
{
    pass P0
    {          
        VS_COMPILE_COMMAND DepthLeafCardVS( );
        PS_COMPILE_COMMAND DepthLeafPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = NONE;
		AlphaRef = 127;
		AlphaFunc = Greater;
    }
}

//  Techniques
technique MainLeafCard
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafCardVS( );
        PS_COMPILE_COMMAND MainLeafPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaRef = 127;
		AlphaFunc = Greater;
		Cullmode = NONE;
    }
}

technique OceanLeafCard
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafCardVS( );
        PS_COMPILE_COMMAND OceanLeafPS( );

        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 89;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
    }
}



technique ShadowLeafMesh
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafMeshVS( );
        PS_COMPILE_COMMAND ShadowLeafPS( );
        
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CW;
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;
    }
}

technique DepthLeafMesh
{
    pass P0
    {          
        VS_COMPILE_COMMAND DepthLeafMeshVS( );
        PS_COMPILE_COMMAND DepthLeafPS( );
        
		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaRef = 127;
		AlphaFunc = Greater;
		FillMode         = Solid;
		CullMode         = CW;
    }
}

technique MainLeafMesh
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafMeshVS( );
        PS_COMPILE_COMMAND MainLeafPS( );
        
		ZWriteEnable     = false;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaRef = 127;
		AlphaFunc = Greater;
		Cullmode = CW;
    }
}

technique OceanLeafMesh
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainLeafMeshVS( );
        PS_COMPILE_COMMAND OceanLeafPS( );
        
        ZWriteEnable     = true;
		ZEnable          = true;
		
		AlphaRef = 127;
		AlphaFunc = Greater;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		Cullmode = NONE;
    }
}
