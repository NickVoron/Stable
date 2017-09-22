float4x4 world;
float4x4 view_proj;
float3 lightDir;

float4 color;
float4x4 view;
float4x4 proj;

struct VS_INPUT
{
    float3 Pos  : POSITION;
	float3 Normal  : NORMAL;

};

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
	float3 Light : TEXCOORD0;
};

VS_OUTPUT VS( VS_INPUT In )
{
    VS_OUTPUT Out;
	
    float4 pos = mul(float4( In.Pos, 1 ), world );
	float3 N = mul(In.Normal, (float3x3)world);

	Out.Light = saturate( dot(normalize(N), normalize(lightDir)) ) + 0.2;
	Out.Light = normalize(N);
	Out.Pos = mul( pos, view_proj );
	
    return Out;
}

float4 PS( VS_OUTPUT In ) : COLOR
{
    return color; //float4(In.Light, 1);
}


/*
VS_OUTPUT ViewDependent_VS( VS_INPUT In )
{
    VS_OUTPUT Out;

	float3 pos = mul( world[3].xyz, ( float3x3 )view ) + ( In.Pos * float3( world[0][0], world[1][1], world[2][2] ) + view[3].xyz );
	Out.Pos = mul( float4( pos, 1 ), proj );
	Out.Light = 1.0f;
	
    return Out;
}

technique DrawManipPart
{
    pass P0
    {
		VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		//FillMode = Wireframe;
		ZEnable = false;
		//ZWriteEnable = false;
    }
}

technique DrawViewDependentManipPart
{
    pass P0
    {
		VertexShader = compile vs_2_0 ViewDependent_VS();
        PixelShader  = compile ps_2_0 PS();
		CullMode = None;
		//FillMode = Wireframe;
		ZEnable = false;
		//ZWriteEnable = false;
    }
}
		  */