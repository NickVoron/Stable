struct SVSInput 
{
	float4 vPos : POSITION;
};

struct SVSOutput 
{
	float2 vTex		: TEXCOORD0;
	float height	: TEXCOORD1;
	float4 vPos		: POSITION;
};

//------------------------------------------------
// Vertex shader parameters
//------------------------------------------------
float4x4 mW2C;   // world to projection matrix
float4 vXZParam; // scale and offset for x and z position
float2 vHeight;  // height parameter
float3 vEye;     // view position
float4 vUVParam; // uv scale and offset


//------------------------------------------------
// Vertex shader
//------------------------------------------------
void mainVS(out SVSOutput _Output, in SVSInput _Input)
{
	// compute world position
	float4 vWorldPos;
	vWorldPos.xz = _Input.vPos.xy * vXZParam.xy + vXZParam.zw;
	// height is propotional to the square distance in horizontal direction.
	float2 vDir = vEye.xz - vWorldPos.xz;
	float fSqDistance = dot( vDir, vDir );
	vWorldPos.y = fSqDistance * vHeight.x + vHeight.y;
	vWorldPos.w = 1.0f;
	_Output.height = vWorldPos.y;
	
	// transform and projection
	_Output.vPos = mul( vWorldPos, mW2C);
	
	// texture coordinate 
	_Output.vTex = _Input.vPos.zw * vUVParam.xy + vUVParam.zw;
}

sampler sCloud : register(s0);
float4 vCloudsCover;

float4 mainPS(SVSOutput In) : COLOR0
{
	float4 clTex = tex2Dlod( sCloud, float4(In.vTex, 0, 0) );
	float res = dot( clTex, vCloudsCover );
	res = lerp(0.0f, res, saturate(In.height / 0.5f));
	//res = 1;
	return res;
}

technique RenderTech
{
	pass Pass0
	{
    	VertexShader = compile vs_3_0 mainVS();
    	PixelShader = compile ps_3_0 mainPS();
	}
}

