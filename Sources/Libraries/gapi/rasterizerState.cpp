#include "rasterizerState.h"

namespace gapi
{
	RasterizerState::RasterizerState()
	{
		fillMode = SOLID;
		cullMode = BACK;
		frontCounterClockwise = false;
		depthBias = 0;
		slopeScaledDepthBias = 0.0f;
		depthBiasClamp = 0.0f;
		depthClipEnable = true;
		scissorEnable = false;
		multisampleEnable = false;
		antialiasedLineEnable = false;
	}

	RenderTargetBlend::RenderTargetBlend()
	{
		blendEnable				= false;
		srcBlend				= BLEND_ONE;
		destBlend				= BLEND_ZERO;
		blendOp					= BLEND_OP_ADD;
		srcBlendAlpha			= BLEND_ONE;
		destBlendAlpha			= BLEND_ZERO;
		blendOpAlpha			= BLEND_OP_ADD;
		renderTargetWriteMask	= COLOR_WRITE_ENABLE_ALL;
	}


	BlendState::BlendState()
	{
		alphaToCoverageEnable = false;
		independentBlendEnable = false;
	}

	DepthStencilOpDesc::DepthStencilOpDesc()
	{
		stencilFailOp = STENCIL_OP_KEEP;
		stencilDepthFailOp = STENCIL_OP_KEEP;
		stencilPassOp = STENCIL_OP_KEEP;
		stencilFunc = COMPARISON_NEVER;
	}
	
	DepthStencilState::DepthStencilState()
	{
		depthEnable			= true;
		depthWriteMask		= MASK_ALL;
		depthFunc			= COMPARISON_LESS_EQUAL;
		stencilEnable		= false;
		stencilReadMask		= 0;
		stencilWriteMask	= 0;
		
	}
}