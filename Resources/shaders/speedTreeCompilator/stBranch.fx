

texture     g_tBranchDiffuseMap;            // bark image, .rgb = diffuse color, .a = alpha noise map
texture     g_tBranchNormalMap;             // bark normal map, .rgb = xyz normals
texture     g_tBranchDetailMap;             // bark detail image, .rgb = color, .a = amount of detail to use

sampler2D samBranchDiffuseMap = sampler_state {	Texture = <g_tBranchDiffuseMap>;MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;};
sampler2D samBranchNormalMap = sampler_state  { Texture = <g_tBranchNormalMap>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;};
sampler2D samBranchDetailMap = sampler_state  { Texture = <g_tBranchDetailMap>;	MaxAnisotropy = 16;	MinFilter = ANISOTROPIC;	MagFilter = LINEAR;	MipFilter = LINEAR;};


struct SMainBranchOutput
{
    float4  vPosition               : POSITION;
    float2  vDiffuseTexCoords       : TEXCOORD0;
    float2  vNormalTexCoords        : TEXCOORD2;
    float2  vDetailTexCoords        : TEXCOORD3;
    float3  vNormal                 : TEXCOORD4;
};

SMainBranchOutput MainBranchVS(
					   float3   vPosition               : POSITION,
					 
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
    float3x3 matRotate = RotationMatrix_yAxis(rotScale.x);

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
	
	sOutput.vNormal = vNormal.xyz;
	
    // project position to the screen
    sOutput.vPosition = mul(float4(vPosition, 1), View );
	sOutput.vPosition = mul(sOutput.vPosition, Projection );
	
    
    // pass through other texcoords exactly as they were received
    sOutput.vDiffuseTexCoords = vDiffuseTexCoords;
    sOutput.vNormalTexCoords = vNormalTexCoords;
    sOutput.vDetailTexCoords = vDetailTexCoords;

    return sOutput;
}


float4 MainBranchPS(SMainBranchOutput In) : COLOR
{
    // look up the diffuse and normal-map layers
    float4 texDiffuse = tex2D(samBranchDiffuseMap, In.vDiffuseTexCoords);
	float4 texNormal = tex2D(samBranchNormalMap, In.vNormalTexCoords) * 2 - 1;
    float4 texDetail = tex2D(samBranchDetailMap, In.vDetailTexCoords);
    texDiffuse.rgb = lerp(texDiffuse.rgb, texDetail.rgb, texDetail.a);
    
    float3 normal = In.vNormal.xyz;
    
    if(renderBillboard)
    {
		if(colorPass) return texDiffuse;
		return float4((normal + 1.0f) / 2.0f, texDiffuse.a);
    }
    
    float nDotL = saturate( dot(normal.xyz, normalize(sunLightDir)) );
    float4 resColor;
    resColor.a = texDiffuse.a;
    resColor.rgb = (nDotL + lightAmbient.rgb) * texDiffuse.rgb;
	return resColor;
}

technique MainBranches
{
    pass P0
    {          
        VS_COMPILE_COMMAND MainBranchVS( );
        PS_COMPILE_COMMAND MainBranchPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		Cullmode = NONE;
		//FillMode = Wireframe;
    }
}
