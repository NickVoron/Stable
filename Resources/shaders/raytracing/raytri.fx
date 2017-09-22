float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Proj : PROJECTION;
float4x4 ViewProj;

float3 camPos;

texture raysSrcTex;
texture raysDstTex;

texture trgTex;

sampler raysSrc = sampler_state {Texture = <raysSrcTex>; MinFilter = NONE; MagFilter = NONE; MipFilter = NONE; AddressU = WRAP; AddressV = WRAP;};
sampler raysDst = sampler_state {Texture = <raysDstTex>; MinFilter = NONE; MagFilter = NONE; MipFilter = NONE; AddressU = WRAP; AddressV = WRAP;};
sampler trgSam = sampler_state {Texture = <trgTex>; MinFilter = NONE; MagFilter = NONE; MipFilter = NONE; AddressU = WRAP; AddressV = WRAP;};

int trgInTex;
float2 texelCost = 1.0f / 1024.0f;

#define RAYTRI_EPSILON 0.000001

struct RAYTRI
{
	float3 org;
	float3 end;
	float3 dir;
	float3 v0;
	float3 v1;
	float3 v2;
};

int intersect_triangle(RAYTRI rt, out float u, out float v)
{
	u = -1.0f;
	v = -1.0f;

	float3 edge1 = rt.v1 - rt.v0;
	float3 edge2 = rt.v2 - rt.v0;
	float3 pvec = cross(rt.dir, edge2);

	float det = dot(edge1, pvec);

	if (det > -RAYTRI_EPSILON && det < RAYTRI_EPSILON)
		return 0;

	float inv_det = 1.0f / det;
	float3 tvec = rt.org - rt.v0;

	/* calculate U parameter and test bounds */
	u = dot(tvec, pvec) * inv_det;
	if (u < 0.0f || u > 1.0f)
		return 0;

	/* prepare to test V parameter */
	float3 qvec = cross(tvec, edge1);

	/* calculate V parameter and test bounds */
	v = dot(rt.dir, qvec) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
		return 0;

	/* calculate t, ray intersects triangle */
	float t = dot(edge2, qvec) * inv_det;
	if(t<0.0f || t>1.0f)	return 0;

	return 1;
}



struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float2 UV : TEXCOORD0;
};

float4 PS(VS_OUTPUT In ) : COLOR
{
//	return float4(In.UV, 0, 1);
	RAYTRI rt;
	
	//rt.org = tex2D(raysSrc, In.UV).xyz;
	//rt.end = tex2D(raysDst, In.UV).xyz;
	rt.org = camPos;
	rt.end = normalize(mul((float3x3)ViewProj, float3(In.UV.x - 0.5f, 0.5f - In.UV.y, 0.5f))) * 100;
	rt.dir = rt.end - rt.org;

	for(int i = 0; i < trgInTex; ++i)
	{
		float4 tuv0 = float4((i*3+0) * texelCost.x, texelCost.y, 0, 0);
		float4 tuv1 = float4((i*3+1) * texelCost.x, texelCost.y, 0, 0);
		float4 tuv2 = float4((i*3+2) * texelCost.x, texelCost.y, 0, 0);

		rt.v0 = tex2Dlod(trgSam, tuv0).xyz;
		rt.v1 = tex2Dlod(trgSam, tuv1).xyz;
		rt.v2 = tex2Dlod(trgSam, tuv2).xyz;
// 		rt.v1 = float3(0.0f, 2.0f, 0.0f);
// 		rt.v2 = float3(1.0f, 0.0f, 0.0f);
		
		float2 uv;
		int res = intersect_triangle(rt, uv.x, uv.y);
		float4 c = 0;
		if(res > 0)
		{
			c.xy = uv;
			return c;
		}
	}
	
	return 0;
}

technique RenderScene
{
    pass P0
    {
		//VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_3_0 PS();
		CullMode = None;
		//FillMode = Wireframe;
		ZEnable = true;
		DepthBias = 1000;
    }
}
