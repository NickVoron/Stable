// Copyright (C) 2014-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "dx11/library.include.h"

namespace unigui 
{
namespace draw 
{
		
	template<class Renderer, class Data>
	struct InternalStateBase : public Renderer
	{
		InternalStateBase() :data(0){}

		Data data;
	};

#if defined(USE_WINDOWS)

	namespace dx11
	{
		extern Viewports::ViewportHandle currentViewport;
	}

	template<class Renderer, class Data, PrimitiveRenderType primitiveRenderType>
	struct InternalState9 : public InternalStateBase<Renderer, Data>
	{
		void init()
		{
		}

		void prepare()
		{

			{
				static const char* techs[] = { "RenderLine", "RenderSolid",	"RenderTexture", "RenderFont" };




			}
		}

		void finish()
		{


		}
	};

	template<class Renderer, class Data, PrimitiveRenderType primitiveRenderType>
	struct InternalState11 : public InternalStateBase<Renderer, Data>
	{
		void init()
		{
			gapi::RasterizerState rs;
			rs.fillMode = gapi::SOLID;
			rs.cullMode = gapi::NONE; 

			gapi::BlendState bs;
			bs.renderTarget[0].blendEnable = true;
			bs.renderTarget[0].srcBlend = gapi::BLEND_SRC_ALPHA;
			bs.renderTarget[0].destBlend = gapi::BLEND_INV_SRC_ALPHA;
			bs.renderTarget[0].srcBlendAlpha = gapi::BLEND_SRC_ALPHA;
			bs.renderTarget[0].destBlendAlpha = gapi::BLEND_INV_SRC_ALPHA;
			bs.renderTarget[0].blendOpAlpha = gapi::BLEND_OP_MAX;

			rasterizerState.create(rs);
			blendState.create(bs);

			drawables.add(rasterizerState);
			drawables.add(blendState);
		}
		void prepare()
		{
			Viewports::Viewport* v = dx11::currentViewport.viewport();

			if (v)
			{
				dx11::shaders[primitiveRenderType]->set(*v);
				::dx11::SetPrimitiveTopology<gapi::TRIANGLELIST>().execute(v->deviceIndex());
				::dx11::execute(*v, drawables);
			}			
		}

		void finish()
		{

		}

		::dx11::RasterizerState rasterizerState;
		::dx11::BlendState blendState;
		::dx11::DrawableObjectsList drawables;
	};

#endif

	template<class Renderer, class Data, PrimitiveRenderType primitiveRenderType>
	struct InternalState : public Renderer
	{
		InternalState() : renderer(0)
		{
			if (gapi::initialized(gapi::DX9))
			{
				static InternalState9<Renderer, Data, primitiveRenderType> internalRenderer;
				internalRenderer.init();
				renderer = &internalRenderer;
			}
			else if (gapi::initialized(gapi::DX11))
			{
				static InternalState11<Renderer, Data, primitiveRenderType> internalRenderer;
				internalRenderer.init();
				renderer = &internalRenderer;
			}

			ENFORCE_MSG(renderer, "renderer is not initialized");
		}

		void prepare()	{ ENFORCE(renderer);	renderer->prepare();		}
		void finish()	{ ENFORCE(renderer);	renderer->finish();		}

		Data& data()	{ ENFORCE(renderer);	return renderer->data;	}

		InternalStateBase<Renderer, Data>* renderer;
	};

}
}




// Copyright (C) 2014-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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