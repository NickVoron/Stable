cbuffer ScatteringData : register(b0)
{
	int4 iSampleCount;
	float4 vInvWaveLength;
	float4 vEye;
	float4 vSunPos;

	float4 fSamples_fScaleDepth_fOuterRadius_fInnerRadius;
	float4 fKr4PI_fKm4PI_fScale_fScaleOverScaleDepth;
};

struct VS_INPUT
{
	float3 Position  : POSITION;
	float2 TextureUV : TEXCOORD0;
};

struct PS_INPUT
{																			  
	float4 pos	: SV_POSITION;
	float2 tex0 : TEXCOORD0;
};

float Scale(float fCos)
{
	float fScaleDepth = fSamples_fScaleDepth_fOuterRadius_fInnerRadius.y;
	float fX = 1.0f - fCos;
	return fScaleDepth * exp(-0.00287f + fX * (0.459f + fX * (3.83f + fX * (-6.80f + fX * 5.25f))));
}

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT Output;
	Output.pos = float4(input.Position, 1);
	Output.tex0 = input.TextureUV;
	return Output;
}			

float4 PS(PS_INPUT input) : SV_TARGET0
{
	//return 1;

	float fSamples = fSamples_fScaleDepth_fOuterRadius_fInnerRadius.x;
	float fScaleDepth = fSamples_fScaleDepth_fOuterRadius_fInnerRadius.y;
	float fOuterRadius = fSamples_fScaleDepth_fOuterRadius_fInnerRadius.z;
	float fInnerRadius = fSamples_fScaleDepth_fOuterRadius_fInnerRadius.w;

	float fKr4PI = fKr4PI_fKm4PI_fScale_fScaleOverScaleDepth.x;
	float fKm4PI = fKr4PI_fKm4PI_fScale_fScaleOverScaleDepth.y;
	float fScale = fKr4PI_fKm4PI_fScale_fScaleOverScaleDepth.z;
	float fScaleOverScaleDepth = fKr4PI_fKm4PI_fScale_fScaleOverScaleDepth.w;

	const float fPI = 3.1415159f;
    float2 vAngles = float2(cos(1), 2.0f * fPI) * input.tex0.yx;

    float3 vPos;
	vPos.x = sin(vAngles.x) * cos(vAngles.y);
	vPos.y = cos(vAngles.x);
	vPos.z = sin(vAngles.x) * sin(vAngles.y);
	vPos *= fOuterRadius;
	 
	float3 vRay = vPos - vEye.xyz;
	float fFar = length(vRay);
	vRay /= fFar;

	float3 vStart = vEye.xyz;
	float fHeight = length(vStart);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - vEye.y));
	float fStartAngle = dot(vRay, vStart) / fHeight;
	float fStartOffset = fDepth * Scale(fStartAngle);

	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 vSampleRay = vRay * fSampleLength;
	float3 vSamplePoint = vStart + vSampleRay * 0.5f;

	float3 vFrontColor = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < iSampleCount.x; ++i)
	{
		float fHeight = length(vSamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(vSunPos.xyz, vSamplePoint) / fHeight;
		float fCameraAngle = dot(vRay, vSamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth * (Scale(fLightAngle) - Scale(fCameraAngle)));
		float3 vAttenuate = exp(-fScatter * (vInvWaveLength.xyz * fKr4PI + fKm4PI));
		vFrontColor += vAttenuate * (fDepth * fScaledLength);
		vSamplePoint += vSampleRay;
	}
	
	vFrontColor.xyz = min(vFrontColor.xyz, 6.5519996e4f);
	float mx = max(vFrontColor.x, max(vFrontColor.y, vFrontColor.z));
	return float4(vFrontColor/mx, mx);
}