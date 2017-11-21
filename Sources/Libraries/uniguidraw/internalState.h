#pragma once

#include "dx11/library.include.h"

namespace unigui 
{
namespace draw 
{

	namespace dx11
	{
		extern Viewports::ViewportHandle currentViewport;
	}
		
	template<class Renderer, class Data>
	struct InternalStateBase : public Renderer
	{
		InternalStateBase() :data(0){}

		Data data;
	};

	template<class Renderer, class Data, PrimitiveRenderType primitiveRenderType>
	struct InternalState9 : public InternalStateBase<Renderer, Data>
	{
		void init()
		{
		}

		void prepare()
		{
//			if (dx9::effectFX)
			{
				static const char* techs[] = { "RenderLine", "RenderSolid",	"RenderTexture", "RenderFont" };

//  				dx9::backup.backup();
//  				dx9::depthStencil.set();
//				dx9::effectFX->Begin(techs[primitiveRenderType]);
			}
		}

		void finish()
		{
//			dx9::effectFX->End();
//			dx9::backup.restore();
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
