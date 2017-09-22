#include "../common/globalConstants.fx"

float3 lightDir = {1, 1, 1};


// map block position:
float2 mapBlockSize;
float2 mapBlockIndex;
float2 fragCountInv;


// light intensity
float4 I_a = { 0.1f, 0.1f, 0.1f, 1.0f };    // ambient
float4 I_d = { 1.0f, 1.0f, 1.0f, 1.0f };    // diffuse
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // specular


// material reflectivity
float keyColor = 0.5f;
float4 k_a : MATERIALAMBIENT = { 0.5f,  0.5f,  0.5f, 1.0f };    // ambient
float4 k_d : MATERIALDIFFUSE = {  0.5f,  0.5f,  0.5f, 1.0f };    // diffuse
float4 k_s : MATERIALSPECULAR= {  0.5f,  0.5f,  0.5f, 1.0f };    // specular
float  n   : MATERIALPOWER = 2.0f;                           // power



// texture
texture texDiffuse;
texture texNormal;


struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Spec : COLOR1;
    float2 Tex  : TEXCOORD0;
};


VS_OUTPUT VS(
    float3 Pos  : POSITION, 
    //float3 Norm : NORMAL, 
    float h  : TEXCOORD0)
{
	float3 Norm = float3(0,1,0);
	float3 truePos = float3( Pos.x * mapBlockSize.x, h * 2, Pos.y * mapBlockSize.y );

    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 L = lightDir;

    float4x4 WorldView = mul(World, View);

    float3 P = mul(float4(truePos, 1), (float4x3)WorldView);  // position (view space)
    float3 N = normalize(mul(Norm, (float3x3)WorldView)); // normal (view space)

    float3 R = normalize(2 * dot(N, L) * N - L);          // reflection vector (view space)
    float3 V = -normalize(P);                             // view direction (view space)

    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Diff = I_a * k_a + I_d * k_d * max(0, dot(N, L)); // diffuse + ambient
    Out.Spec = I_s * k_s * pow(max(0, dot(R, V)), n/4);   // specular
    Out.Tex  = float2(
		Pos.x * fragCountInv.x + mapBlockIndex.x * fragCountInv.x,
		(Pos.y * fragCountInv.x + mapBlockIndex.y * fragCountInv.x) );

	//Out.Diff.x = h / 400;

    return Out;
}


sampler SamplerDiffuse = sampler_state
{
    Texture   = <texDiffuse>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler SamplerNormal = sampler_state
{
    Texture   = <texNormal>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};




float4 PS(
		float4 Diff : COLOR0,
		float4 Spec : COLOR1,
		float2 Tex  : TEXCOORD0
    ) : COLOR
{
	//return Diff;
    float4 colorMain = tex2D(SamplerDiffuse, float2(Tex.x, 1 - Tex.y));// * Diff;
	float4 normal    = tex2D(SamplerNormal, Tex);// * Diff;
	
	float3 N =normalize(normal).xyz * 2 - 1; // normal (view space)

        
    //return ( 1.0f - damageAmount ) * colorMain + damageAmount * colorDamage;
    //return ( 1.0f - cutted ) * colorMain + cutted * colorDamage;
    //return 1.0f;
    return (saturate(dot(N, normalize(lightDir))) + 0.7 ) * colorMain;
}




technique RenderScene
{
    pass P0
    {
	VertexShader = compile vs_3_0 VS();
    PixelShader  = compile ps_3_0 PS();
    
    // shaders
    //VertexShader = NULL;
	//PixelShader  = NULL;
	//FillMode = Wireframe;
    }
}
