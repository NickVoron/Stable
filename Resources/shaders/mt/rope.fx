float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float4 CamPos;
float width;

texture ropeTex;
sampler2D ropeTexSam = sampler_state 
{
	Texture = <ropeTex>;
	MaxAnisotropy = 16;		
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};

struct VS_ROPE_OUTPUT
{
	float4 Position : POSITION;
	float2 uv : TEXCOORD0;
};

VS_ROPE_OUTPUT ropeVS( float3 vPos : POSITION, float3 pos0 : TEXCOORD0, float3 pos1 : TEXCOORD1, float3 pos2 : TEXCOORD2, float3 pos3 : TEXCOORD3)
{
	VS_ROPE_OUTPUT Output;

	/*
	Сшивка верёвок возможна только для угла между ними менее 90 градусов.
	угол квада смещается по оси верёвки на расстояние от нуля до толщины верёвки при угле между ними 90.
	Такое смещение работает только когда мы смотрим на этот угол ровно перпендикулярно плоскости между верёвками
	Если угол между перпендикуляром к плоскости верёвок и камерой, смотрящей на их стык равен нулю, то смещение тоже нулевое
	остаётся только вопрос, как сместить нужную вершину. Предположение: через перемножение координат квада:
	если Х-координата отрицательна, значит учитываем pos0-координату верёвки, иначе pos1
	если Y-координата
	*/

	/*
	С расстоянием верёвка должна сохранить минимальную точность 1 пиксел экрана, для этого надо редактировать толщину квада в нужной точке
	*/

	/*
	освещение должно быть по цилиндру, в тени верёвку надо рендерить толще, чем она есть на самом деле
	*/
	
	float3 vX = normalize(pos2.xyz-pos1.xyz);
	float3 vZ = normalize( CamPos.xyz - (dot( CamPos.xyz-pos1.xyz, vX) * vX + pos1.xyz) );
	float3 vY = cross( vX, vZ );
	float3x3 rot = float3x3(vX, vY, vZ);

	float3 vp = vPos;
	//float ll = mul(float4(vPos.xyz, 1), View).z;
	//if( ll > 10.0f)	vp.y *= ll/10.0f;

	float l = length(pos2.xyz-pos1.xyz);
	float3 pp = float3(vp.x*l, vp.y, 0.0f);
	float3 p = mul( pp, rot );
	p += pos1.xyz;

	float4x4 viewProjection = mul(View, Projection);
	Output.Position = mul(float4(p, 1), viewProjection);
	
	Output.uv = float2(vp.x*l/width/4.0f, vp.y/width);

	return Output;    
}


struct PS_ROPE_OUTPUT
{
	float4 color : COLOR0;
};

PS_ROPE_OUTPUT ropePS( VS_ROPE_OUTPUT In ) 
{ 
	PS_ROPE_OUTPUT Output;
	Output.color = 1.0f;//tex2D(ropeTexSam, In.uv);
	return Output;
}


technique RenderRope
{
	pass P0
	{          
		VertexShader = compile vs_3_0 ropeVS();
		PixelShader  = compile ps_3_0 ropePS(); 
		//FillMode = Wireframe;
		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
		CullMode = None;
	}
}
