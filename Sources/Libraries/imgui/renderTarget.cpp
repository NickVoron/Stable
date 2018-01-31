// Copyright (C) 2015-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "renderTarget.h"

namespace imgui 
{
namespace core 
{
#if defined(USE_WINDOWS)
	D3DXVECTOR4 clearColor(0.0f, 0.0f, 0.0f, 0.0);

	struct Camera
	{
		D3DXMATRIX projTM;
		D3DXMATRIX viewTM;
	};

	void RenderTargets::init()
	{
		Resources::load(copyFX, "postFX/dx11/copy.fx");

		pointSampler.samplerDesc.filter = gapi::MIN_MAG_MIP_POINT;
		pointSampler.samplerDesc.addressU = gapi::CLAMP;
		pointSampler.samplerDesc.addressV = gapi::CLAMP;

		pointSampler.create();
		pointSampler.pixel(0);

		gapi::RasterizerState rs;
		rs.cullMode = gapi::NONE;
		rs.fillMode = gapi::SOLID;
		rasterizerState.create(rs);

		gapi::BlendState gbs;
		gbs.alphaToCoverageEnable = true;
		gbs.renderTarget[0].blendEnable = true;
		gbs.renderTarget[0].srcBlend = gapi::BLEND_SRC_ALPHA;
		gbs.renderTarget[0].destBlend = gapi::BLEND_INV_SRC_ALPHA;
		gbs.renderTarget[0].blendOp = gapi::BLEND_OP_ADD;
		gbs.renderTarget[0].srcBlendAlpha = gapi::BLEND_ZERO;
		gbs.renderTarget[0].destBlendAlpha = gapi::BLEND_ZERO;
		gbs.renderTarget[0].blendOpAlpha = gapi::BLEND_OP_ADD;
		gbs.renderTarget[0].renderTargetWriteMask = 0x0F;
		blendState.create(gbs);

		pointSampler.create();

		main_DSS.state.depthEnable = true;
		main_DSS.state.depthWriteMask = gapi::MASK_ZERO;
		main_DSS.create();
	}

	void RenderTargets::bind(Viewports::Viewport* viewport)
	{
		dx11::PlainTarget& target = targets[viewport];
		target.set(true);
		
	}

	void RenderTargets::initTarget(Viewports::Viewport* viewport)
	{
		dx11::PlainTarget& target = targets[viewport];
		dx11::ShaderResource& resource = shaderResources[viewport];

		int width = viewport->width();
		int height = viewport->height();

		target.release();
		target.create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
		target.clearColor(clearColor);
		resource.create(target);
	}

	void RenderTargets::clear(Viewports::Viewport* viewport)
	{
		dx11::PlainTarget& target = targets[viewport];
		target.clearColor(clearColor);
	}

	void RenderTargets::draw(Viewports::Viewport* viewport)
	{
		copyFX->resource([this, viewport](auto& shaders)
		{
			dx11::ShaderResource& texResource = shaderResources[viewport];
			dx11::DefaultRenderTargets::data[viewport->deviceIndex()].set();

			texResource.pixel(0);
			pointSampler.pixel(0);

			dx11::DrawableObjectsList drawables;
			drawables.add(shaders);

			drawables.add(texResource);
			drawables.add(pointSampler);
			
			drawables.add(blendState);
			drawables.add(rasterizerState);
			drawables.add(main_DSS);
			dx11::execute(*viewport, drawables);

			dx11::im::DrawFSQuad(*viewport);
			dx11::resetShaderResourcePS(0);
		});
	}
#endif
}
}//



// Copyright (C) 2015-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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