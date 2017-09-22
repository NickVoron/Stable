float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float time;

float2 ScreenTexelUV;

float3 particleSystemPivot;
int ObjType = 0;

float spriteHorizontalStep;
float spriteVerticalStep;			

float numHorizSprites;
float numVertSprites;

texture Tex;
sampler spriteSampler = sampler_state
{
	texture = <Tex>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Anisotropic;
	MagFilter = Anisotropic;
};

/*

texture Sprite;
sampler SpriteMap = sampler_state
{
	texture = <Sprite>;
	AddressU  = Clamp;
	AddressV  = Clamp;

	MipFilter = None;
	MinFilter = Anisotropic;
	MagFilter = Anisotropic;
}; 

*/

struct VS_INPUT
{
    float3 Pos  : POSITION;
    float3 WPos : TEXCOORD0;
	float2 TexCoord0 : TEXCOORD1;
	float4 SizeStretchRotationOpacity : TEXCOORD2;
	float3 DiffuseEmissionSubTexIdx : TEXCOORD3;
	float3 baseHSV : TEXCOORD4;
	float4 emitHSV : TEXCOORD5;
};

struct VS_MAIN_OUTPUT
{
    float4 Pos  : POSITION;
 	float2 TexCoord0 : TEXCOORD0;
	float3 baseHSV : TEXCOORD1;
	float3 emitHSV : TEXCOORD2; 
	float Opacity : TEXCOORD3;
	float4 ScreenProjCoords : TEXCOORD4;
};

struct VS_SHADOW_OUTPUT
{
    float4 Pos  : POSITION;
};

struct VS_DEPTH_OUTPUT
{
    float4 Pos  : POSITION;
    float depth	: TEXCOORD0;
};

float3 hsv_to_rgb( float3 HSV )
{
    float3 RGB = HSV.z;
    if( HSV.y != 0 )
    {
       float var_h = HSV.x * 6;
       float var_i = floor( var_h );   // Or ... var_i = floor( var_h )
       float var_1 = HSV.z * ( 1.0 - HSV.y );
       float var_2 = HSV.z * ( 1.0 - HSV.y * ( var_h-var_i ));
       float var_3 = HSV.z * ( 1.0 - HSV.y * ( 1 - ( var_h-var_i )));
       if     ( var_i == 0 ) { RGB = float3( HSV.z, var_3, var_1 ); }
       else if( var_i == 1 ) { RGB = float3( var_2, HSV.z, var_1 ); }
       else if( var_i == 2 ) { RGB = float3( var_1, HSV.z, var_3 ); }
       else if( var_i == 3 ) { RGB = float3( var_1, var_2, HSV.z ); }
       else if( var_i == 4 ) { RGB = float3( var_3, var_1, HSV.z ); }
       else { RGB = float3( HSV.z, var_1, var_2 ); }
   }
   return ( RGB );
}

VS_MAIN_OUTPUT MainVS( VS_INPUT In)
{
    VS_MAIN_OUTPUT Out;
    float4x4 WorldView = mul( World, View );

	float Sin = sin( In.SizeStretchRotationOpacity.z * time );
	float Cos = cos( In.SizeStretchRotationOpacity.z * time );//cos( 0 * time );

	// Rotation around Z-Axis
	float3 Pos = In.Pos;
	In.Pos.x = Pos.x * Cos - Pos.y * Sin;
	In.Pos.y = Pos.x * Sin + Pos.y * Cos;

	// Resize
	In.Pos.xyz *= In.SizeStretchRotationOpacity.x;

	// Strech that
	In.Pos.y *= In.SizeStretchRotationOpacity.y;

	//In.WPos.xyz += particleSystemPivot.xyz;	
	float3 P = mul( float4( In.WPos, 1 ), (float4x3)WorldView ) + ( In.Pos );  // position (view space)
   	Out.Pos  = mul( float4( P, 1 ), Projection );
	
	float horizontalIdx = In.DiffuseEmissionSubTexIdx.z / numHorizSprites;
	horizontalIdx = frac( horizontalIdx ) * numHorizSprites;

	float verticalIdx = In.DiffuseEmissionSubTexIdx.z / numHorizSprites;
	verticalIdx = verticalIdx - frac( verticalIdx );			

	Out.TexCoord0.x = ( In.TexCoord0.x + horizontalIdx ) * spriteHorizontalStep;
	Out.TexCoord0.y = ( In.TexCoord0.y + verticalIdx ) * spriteVerticalStep;
	Out.Opacity = In.SizeStretchRotationOpacity.w;

	Out.baseHSV = In.baseHSV;
	Out.emitHSV = In.emitHSV;

	Out.ScreenProjCoords.xy = Out.Pos.xy * float2( 0.5f, -0.5f ) + Out.Pos.ww * 0.5f;
	Out.ScreenProjCoords.zw = Out.Pos.zw;

#ifdef D3D9_RASTER_OFFSET
	Out.ScreenProjCoords.xy += Out.ScreenProjCoords.ww * 0.5f * ScreenTexelUV;
#endif

    return Out;
}

VS_DEPTH_OUTPUT DepthVS( VS_INPUT In )
{
    VS_DEPTH_OUTPUT Out;
    float4x4 WorldView = mul( World, View );

    float Sin = sin( In.SizeStretchRotationOpacity.z );
    float Cos = cos( In.SizeStretchRotationOpacity.z );

    // Rotation around Z-Axis
    float3 Pos = In.Pos;
    In.Pos.x = Pos.x * Cos - Pos.y * Sin;
    In.Pos.y = Pos.x * Sin + Pos.y * Cos;

    // Resize
    In.Pos.xyz *= In.SizeStretchRotationOpacity.x;

    // Strech that
    In.Pos.y *= In.SizeStretchRotationOpacity.y;

    float3 P = mul( float4( In.WPos, 1 ), (float4x3)WorldView ) + ( In.Pos );  // position (view space)
    Out.Pos  = mul( float4( P, 1 ), Projection );
    Out.depth = Out.Pos.w;
    
    return Out;
}

VS_SHADOW_OUTPUT ShadowVS( VS_INPUT In )
{
    VS_SHADOW_OUTPUT Out;
    float4x4 WorldView = mul( World, View );

	float Sin = sin( In.SizeStretchRotationOpacity.z );
	float Cos = cos( In.SizeStretchRotationOpacity.z ); // this may cause texture coordinates (mirrong issue)

	// Rotation around Z-Axis
	float3 Pos = In.Pos;
	In.Pos.x = Pos.x * Cos - Pos.y * Sin;
	In.Pos.y = Pos.x * Sin + Pos.y * Cos;

	// Resize
	In.Pos.xyz *= In.SizeStretchRotationOpacity.x;

	// Strech that
	In.Pos.y *= In.SizeStretchRotationOpacity.y;

	float3 P = mul( float4( In.WPos, 1 ), (float4x3)WorldView ) + ( In.Pos );  // position (view space)
    Out.Pos  = mul( float4( P, 1 ), Projection );

#ifdef D3D9_RASTER_OFFSET
	Out.Pos.x -= ScreenTexelUV.x * Out.Pos.w;
	Out.Pos.y += ScreenTexelUV.y * Out.Pos.w;
#endif
	
    return Out;
}


float4 MainPS( VS_MAIN_OUTPUT In, out float4 depth : COLOR1 ) : COLOR
{
	float4 color = tex2D( spriteSampler, In.TexCoord0 );
	color.a = In.Opacity;
	
	color.rgb = color.rgb * hsv_to_rgb( In.baseHSV ) + hsv_to_rgb( In.emitHSV );
 	
	depth = In.ScreenProjCoords.w;
	return color;
}

float4 DepthPS( VS_DEPTH_OUTPUT In ) : COLOR
{
    return float4(In.depth, 0, 0, 0);
}

float4 ShadowPS( VS_SHADOW_OUTPUT In ) : COLOR
{
    return 0;
}

technique MainParticles
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = None;
		ZEnable = false; // если включено партикли глючат!
		ZWriteEnable = false;
		AlphaTestEnable = true;
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = INVSRCALPHA;
    }
}

technique OceanParticles
{
    pass P0
    {
		VertexShader = compile vs_3_0 MainVS();
        PixelShader  = compile ps_3_0 MainPS();
		CullMode = None;
		ZEnable = true;
		ZWriteEnable = true;
		AlphaTestEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = INVSRCALPHA;
    }
}

technique DepthParticles
{
    pass P0
    {
		VertexShader = compile vs_3_0 DepthVS();
        PixelShader  = compile ps_3_0 DepthPS();

		ZWriteEnable     = true;
		ZEnable          = true;
		
		StencilEnable    = true;
		StencilRef       = <ObjType>;
		StencilPass      = Replace;
		
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
    }
}

technique ShadowParticles
{
    pass P0
    {
		VertexShader = compile vs_3_0 ShadowVS();
        PixelShader  = compile ps_3_0 ShadowPS();

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		FillMode         = Solid;
		CullMode         = None;
		
		ColorWriteEnable = 0;
		
		SlopeScaleDepthBias = 1.001; //2 for PCF1, 4 - for PCF3x3
		DepthBias           = 0.00001;
    }
}


