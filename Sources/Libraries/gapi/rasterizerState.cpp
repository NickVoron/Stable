// Copyright (C) 2013-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2013-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.