#pragma once
#include "immediateDraw/library.include.h"
#include "resourceShader/library.include.h"

namespace imgui 
{
namespace core 
{
	struct RenderTargets
	{
		void init();
		
		void initTarget(Viewports::Viewport* viewport);

		void draw(Viewports::Viewport* viewport);
		void bind(Viewports::Viewport* viewport);
		void clear(Viewports::Viewport* viewport);

	private:
		typedef std::map<Viewports::Viewport*, dx11::PlainTarget> Targets;
		typedef std::map<Viewports::Viewport*, dx11::ShaderResource> ShaderResources;
		
		Targets targets;
		ShaderResources shaderResources;


		Resources::VertexPixelShader* copyFX = 0;

		dx11::ShaderResource guiTex;

		dx11::BlendState blendState;
		dx11::RasterizerState rasterizerState;

		dx11::Sampler pointSampler;
		dx11::DepthStencilState depth_DSS;
		dx11::DepthStencilState main_DSS;

	};

}//
}//