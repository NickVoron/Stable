// Copyright (C) 2013-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "blendState.h"
#include "device.h"

namespace dx11
{
	inline void renderTargetBlend(D3D11_RENDER_TARGET_BLEND_DESC& dst, const gapi::RenderTargetBlend& src) 
	{
		dst.BlendEnable				= src.blendEnable;
		dst.BlendOp					= (D3D11_BLEND_OP)src.blendOp;
		dst.BlendOpAlpha			= (D3D11_BLEND_OP)src.blendOpAlpha;
		dst.DestBlend				= (D3D11_BLEND)src.destBlend;
		dst.DestBlendAlpha			= (D3D11_BLEND)src.destBlendAlpha;
		dst.SrcBlend				= (D3D11_BLEND)src.srcBlend;
		dst.SrcBlendAlpha			= (D3D11_BLEND)src.srcBlendAlpha;
		dst.RenderTargetWriteMask	= src.renderTargetWriteMask;
	}

	void blendDesc(D3D11_BLEND_DESC& res, const gapi::BlendState& state)
	{
		res.AlphaToCoverageEnable = state.alphaToCoverageEnable;
		res.IndependentBlendEnable = state.independentBlendEnable;

		for (int i = 0; i < 8; ++i)
		{
			renderTargetBlend(res.RenderTarget[i], state.renderTarget[0]);
		}
	}

	DeviceBlendState::DeviceBlendState()
	{

	}

	DeviceBlendState::DeviceBlendState(const gapi::BlendState& state)
	{
		create(state);
	}

	DeviceBlendState::~DeviceBlendState()
	{
		clear();
	}
	
	void DeviceBlendState::create(const gapi::BlendState& state)
	{
		clear();
		D3D11_BLEND_DESC desc;
		blendDesc(desc, state);
		device->device->CreateBlendState(&desc, &ptr);
	}

	void DeviceBlendState::clear()
	{
		SAFE_RELEASE(ptr);
	}

	void DeviceBlendState::set() const
	{
		float factor[4] = {1.0f, 1.0f, 1.0f, 1.0};
		device->context->OMSetBlendState(ptr, factor, 0xFFFFFFFF);
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