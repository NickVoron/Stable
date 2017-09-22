float4x4 viewProjection;
float3 lightDir;

struct VInput
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
};

struct VOutput
{
	float4 Position : POSITION0;
	float3 Pos		: TEXCOORD1;
	float3 Normal	: TEXCOORD2;
	float Light		: TEXCOORD3;
};

VOutput VS(VInput In)
{
	VOutput Out;
	
	Out.Position = mul(float4(In.Position, 1), viewProjection);
	Out.Normal = In.Normal;

	Out.Pos = In.Position;
	Out.Light = dot( normalize(In.Normal), lightDir);
	
	return Out;
}

float4 PS(VOutput In) : COLOR
{
	return In.Light;
	float diffuse = dot( normalize(In.Normal), normalize(lightDir - normalize(In.Pos)) );
	return diffuse;
}


technique OceanRefrLand
{
	pass p0
	{
		VertexShader     = compile vs_3_0 VS();
		PixelShader      = compile ps_3_0 PS();

		ZWriteEnable     = true;
		ZEnable          = true;
		AlphaTestEnable  = false;
		AlphaBlendEnable = false;
	}
}