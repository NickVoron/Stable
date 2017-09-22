float4x4 view       : VIEW;
float4x4 projection : PROJECTION;

static const float pi = 3.1415926535897932385f;
static const float invPi = 1.0f / pi;

float3 ligthDir;

texture heightMap;
texture normalMap;
texture weightMap;

sampler2D heightSam = sampler_state
{
	Texture = <heightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D normalSam = sampler_state
{
	Texture = <normalMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

sampler2D weightSam = sampler_state
{
	Texture = <weightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};



struct VS_INPUT
{
    float3 Pos  : POSITION;
};

struct VS_OUTPUT
{
    float4 pos	 : POSITION;
	float4 pos3d_radius : TEXCOORD0;
};

float2 getSphereUV(float3 pos, float r)
{
	float v = acos(pos.y / r) * invPi;
	float u = atan2(pos.x, -pos.z) / (pi * 2) + 0.5f;
	return float2(u, v);
}

//
//
//
VS_OUTPUT SphereVS( VS_INPUT input  )
{
	VS_OUTPUT output;

	output.pos3d_radius.xyz	= input.Pos;
	output.pos3d_radius.w = length(input.Pos);

	float2 tc = getSphereUV(output.pos3d_radius.xyz, output.pos3d_radius.w);
	
	float3 posVec = normalize(input.Pos);
	float height = tex2Dlod(heightSam, float4(tc, 0, 0)).r * 0.02f;

	float4x4 viewProj	= mul(view, projection);
 	float4 resPos = mul(float4(input.Pos + posVec * height, 1), viewProj);

	output.pos = resPos;

  	return output;
}

float4 SpherePS( VS_OUTPUT input  ) : COLOR
{
	float2 tc = getSphereUV(input.pos3d_radius.xyz, input.pos3d_radius.w);
	float3 tn = tex2D(normalSam, tc).xyz;
	float height = tex2D(heightSam, tc).r;
	float4 w1 =  tex2D(weightSam, tc);
	float3 sphereNormal = normalize(input.pos3d_radius.xyz);

	//sphereNormal
	float2 temp = normalize( sphereNormal.xz );
	float3 binormal = float3( -temp.y, 0, temp.x );
	float3 tangent = cross( sphereNormal, binormal );
	
	//float3x3 tbn = { tangent, binormal, sphereNormal };
	//float3x3 tbn = { tangent, sphereNormal, binormal  };
	float3x3 tbn = { binormal, sphereNormal , tangent };

	float dampen = dot( sphereNormal, -ligthDir );
	
	//float3 normal = mul( float3(0,1,0), tbn);
	float3 normal = mul( tn , tbn);

	//return float4( normal, 1 );	
	//return float4( float3(0.5f,0.5f,0), 1 );
	
	float nDotL = saturate(dot( normal, ligthDir) );
	float kkk = 1-( (1+height) * 0.2f );
	//return float4(kkk,0,0,1);
	nDotL = clamp( nDotL, 0, (1-dampen*(1+kkk*2) ) );
	//nDotL = sqrt( nDotL );
	
	//return float4(tn, 1);
	//return height;
	//return w1;
	return saturate(nDotL);// * w1;
}

//
//
//
VS_OUTPUT CylinderVS( VS_INPUT input  )
{
	VS_OUTPUT output;

	float4x4	viewProj	= mul(view, projection);
 	float4		resPos		= mul(float4(input.Pos, 1), viewProj);

	output.pos = resPos;
	output.pos3d_radius.xyz	= input.Pos;
	output.pos3d_radius.w = length(input.Pos);
	
  	return output;
}

float4 CylinderPS( VS_OUTPUT input  ) : COLOR
{
	float2 tc = getSphereUV(input.pos3d_radius.xyz, input.pos3d_radius.w);
	return float4(tc, 0, 1);
}

//
//
//
VS_OUTPUT PlaneVS( VS_INPUT input  )
{
	VS_OUTPUT output;

	float4x4	viewProj	= mul(view, projection);
 	float4		resPos		= mul(float4(input.Pos, 1), viewProj);

	output.pos = resPos;
	output.pos3d_radius.xyz	= input.Pos;
	output.pos3d_radius.w = 1;

  	return output;
}

float4 PlanePS( VS_OUTPUT input  ) : COLOR
{
	float2 tc = input.pos3d_radius.xy;
	return float4(tc, 0, 1);
}

technique Sphere
{
	pass P0
	{
		VertexShader  = compile vs_3_0 SphereVS();
		PixelShader  = compile ps_3_0 SpherePS();
		ZEnable = true;
		CullMode = None;
		//FillMode = Wireframe;
	}
}


technique Cylinder
{
	pass P0
	{
		VertexShader  = compile vs_3_0 CylinderVS();
  		PixelShader  = compile ps_3_0 CylinderPS();
		ZEnable = true;
		CullMode = None;
	}
}

technique Plane
{
	pass P0
	{
		VertexShader  = compile vs_3_0 PlaneVS();
   		PixelShader  = compile ps_3_0 PlanePS();
		ZEnable = true;
		CullMode = None;
	}
}
