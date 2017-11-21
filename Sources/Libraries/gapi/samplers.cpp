#include "samplers.h"
#include <float.h>

namespace gapi
{	
	SamplerDesc::SamplerDesc()
	{
		addressU = WRAP;
		addressV = WRAP;
		addressW = WRAP;

		filter = MIN_MAG_MIP_POINT;
		minLod = -FLT_MAX;
		maxLod = FLT_MAX;
		mipLODBias = 0.0f;
		maxAnisotropy = 1;
		comparisonFunc = COMPARISON_NEVER;

		BorderColor[0] = 1.0f;
		BorderColor[1] = 1.0f;
		BorderColor[2] = 1.0f;
		BorderColor[3] = 1.0f;					  
	}
}




