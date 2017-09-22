float4 texCoords360;
float f_spriteCount = 10;
float stretchFactor = 1.0;

struct SBillboardInput
{
    float2  pos       : POSITION;
    float3  wPos      : TEXCOORD0;
    float2  rotScale  : TEXCOORD1;
    float2  uv		  : TEXCOORD2;
};

struct SBillboardOutput
{
    float4 pos : POSITION;
    float2 texCoord : TEXCOORD0;
	float4 fog : TEXCOORD1;
	float3 worldPosVS : TEXCOORD2;
};

const float PI = 3.1415926f;
const float TwoPI = 3.1415926f * 2.0f;

SBillboardOutput BillboardVS(SBillboardInput In)
{
    SBillboardOutput output;
    
    // Count sprite number: efge
    /*
    float3 distVec = In.wPos - cameraPosition.xyz;
    float cameraAzimuth = atan2( -distVec.x, distVec.z);
    float relativeAngle = TwoPI-In.rotScale.x - cameraAzimuth; // now we are  [-PI .. 3xPI];
    float sprite_fraction = frac( ( relativeAngle + TwoPI ) / TwoPI );
    int spriteNo = round(f_spriteCount - 1 - sprite_fraction * f_spriteCount);
    if ( spriteNo >= f_spriteCount ) spriteNo -= f_spriteCount;

    */
    
    float3 distVec = In.wPos - cameraPosition.xyz;
	output.worldPosVS = -distVec;
    float cameraAzimuth = atan2( distVec.x, -distVec.z);
    float relativeAngle = In.rotScale.x + cameraAzimuth + TwoPI; // now we are  [-PI .. 3xPI];
    float sprite_fraction = frac( ( relativeAngle + PI ) / TwoPI );
//  float sprite_fraction = In.rotScale.x / TwoPI;
    int spriteNo = round(sprite_fraction * (f_spriteCount) ) + 1 ;
    if ( spriteNo >= f_spriteCount ) spriteNo -= f_spriteCount;

    // do rest jobd: 
    float3x3 matRotation = RotationMatrix_yAxis( cameraAzimuth );
    float2 pos2 = In.pos.xy * texCoords360.zw;
    pos2.y -= texCoords360.y;
    float3 pos = mul( float3(pos2.xy * In.rotScale.y , 0), matRotation); 
    output.texCoord = In.uv;
    output.texCoord.x *= texCoords360.x;  
    output.texCoord.x = -output.texCoord.x;
    output.texCoord.y = -output.texCoord.y;

    float3 worldPos = pos + In.wPos;
    output.pos = mul(float4(worldPos, 1), View);
    output.pos = mul(output.pos, Projection);

    // сместиться на другой спрайт:
    output.texCoord.x += spriteNo * texCoords360.x;

	// туман
	float sf = computeSolidFog(output.pos.z, minMaxDist);
	output.fog = float4(sf, worldPos.y, output.pos.z, 1);

    return output;
}

float4 BillboardPS(SBillboardOutput In) : COLOR
{
	//return float4(In.texCoord, 0, 1);
	float3 n = normalize(tex2D(samBillboardNormalMap, In.texCoord).xyz) * 2 - 1;
	float4 texDiffuse = tex2D(samBillboardDiffuseMap, In.texCoord);
	
	float3 lightDirVS = mul(sunLightDir, (float3x3)View);

	float nDotL = saturate( dot(n.xyz, normalize(lightDirVS)) );
    float4 resColor;
    resColor.rgb = (nDotL + lightAmbient()) * texDiffuse.xyz;
    //resColor.rgb = n;
    resColor.a = texDiffuse.a;


	float3 tc = In.worldPosVS;
	tc.y = max(tc.y, 0);
	float4 fc = float4( getAtmosphereColor(tc), 1.0f );
	resColor = lerp(resColor, fc, In.fog.x);
    


    //resColor.rgb = texDiffuse.xyz;
    return resColor;
    
    return texDiffuse;

}


technique MainBillboards
{
    pass P0
    {          
        VS_COMPILE_COMMAND BillboardVS( );
        PS_COMPILE_COMMAND BillboardPS( );
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaRef = 127;
		AlphaFunc = Greater;
		Cullmode = CCW;

    }
}

technique OceanBillboards
{
    pass P0
    {          
        VS_COMPILE_COMMAND BillboardVS( );
        PS_COMPILE_COMMAND BillboardPS( );
		Cullmode = NONE;
		AlphaRef = 127;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		AlphaFunc = Greater;
    }
}

technique ShadowBillboards
{
    pass P0
    {          
        VS_COMPILE_COMMAND BillboardVS( );
        PS_COMPILE_COMMAND BillboardPS( );

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = true;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = CCW;
		AlphaRef = 127;
		AlphaFunc = Greater;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.0000001;

    }
}

technique DepthBillboards
{
    pass P0
    {          
        VS_COMPILE_COMMAND BillboardVS( );
        PS_COMPILE_COMMAND BillboardPS( );

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
		CullMode         = CCW;
    }
}
