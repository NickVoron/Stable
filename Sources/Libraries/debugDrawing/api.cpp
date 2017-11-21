#include "api.h"

namespace DebugDrawing
{
	//
	//
	//
	ShaderDataPerObject::ShaderDataPerObject()
	{
		mem::memzero(colorFromVertex);
	}

	//
	//
	//
	void initRasterizerState(dx11::RasterizerState& rasterizerState, ShaderMode shaderMode)
	{
		gapi::RasterizerState rs;

		switch(shaderMode)
		{
		case INTERNAL_DEFAULT_SHADER:
			{
				rs.fillMode = gapi::WIREFRAME;
				rs.cullMode = gapi::NONE;
				break;
			}
		case INTERNAL_SOLID_SHADER:
			{
				rs.fillMode = gapi::SOLID;
				rs.cullMode = gapi::NONE;
				break;
			}
		};

		rasterizerState.create(rs);
	}

	//
	//
	//
/*
	void ToolDX9::init(const char* fxFileName)
	{
		Resources::load(effect, fxFileName);
		Streaming::Streamer::commit();
	}

	void ToolDX9::uploadPerObject(Viewports::Viewport& viewport, const ShaderDataPerObject& data)
	{
		effect->SetValue("World", data.world);
		effect->SetValue("currentColor", data.color);
	}

	void ToolDX9::uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data)
	{
		effect->SetValue("View", data.view);
		effect->SetValue("Projection", data.projection);
	}

	void ToolDX9::begin(Viewports::Viewport& viewport, ShaderMode mode)
	{ 
		effect->Begin();	
	}

	void ToolDX9::end()
	{
		effect->End();	
	}
	
*/

	//
	//
	//
	void ToolDX11::init(const char* fxFileName)
	{
		currentShaderMode = INTERNAL_DEFAULT_SHADER;

		const char* shaderNames[INTERNAL_SHADERS_COUNT] = 
		{	
			"common/bodies11.fx", 
			"common/bodies11.fx"
		};

		for (int i = 0; i < INTERNAL_SHADERS_COUNT; ++i)
		{
			Resources::load(shader[i], shaderNames[i]);
		}
		
		Streaming::Streamer::commit();

		initRasterizerState(rasterizerState, INTERNAL_DEFAULT_SHADER);

		cb0.vertex(0);
		cb0.pixel(0);

		cb1.vertex(1);
		cb1.pixel(1);

		drawables.add(cb1);
		drawables.add(cb0);
		drawables.add(rasterizerState);
	}

	void ToolDX11::uploadPerObject(Viewports::Viewport& viewport, const ShaderDataPerObject& data)
	{
		cb1.upload(data);
	}

	void ToolDX11::uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data)
	{
		cb0.upload(data);
	}

	void ToolDX11::begin(Viewports::Viewport& viewport, ShaderMode mode)
	{
		shader[mode]->resource([&viewport, this](auto& shaders)
		{				
			dx11::execute(viewport, drawables);
			dx11::DrawableObjectsList shd;
			shd.add(shaders);
			dx11::execute(viewport, shd);
		});
	}

	void ToolDX11::end()
	{ 

	}

	void ToolSwitcher::init(const char* fxFileName, gapi::Id api_)
	{
		api = api_;
		switch (api)
		{
//		case gapi::DX9: toolDX9.init(fxFileName); break;
		case gapi::DX11: toolDX11.init(fxFileName); break;
		}
	}

	void ToolSwitcher::uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data)
	{
		switch (api)
		{
//		case gapi::DX9: toolDX9.uploadPerCamera(viewport, data); break;
		case gapi::DX11: toolDX11.uploadPerCamera(viewport, data); break;
		}
	}

/*


	void ToolSwitcher::uploadPerObject(Viewports::Viewport& viewport, const ShaderDataPerObject& data)
	{
		switch (api)
		{
		case gapi::DX9: toolDX9.uploadPerObject(viewport, data); break;
		case gapi::DX11: toolDX11.uploadPerObject(viewport, data); break;
		}
	}

	void ToolSwitcher::begin(Viewports::Viewport& viewport, ShaderMode mode)
	{
		switch (api)
		{
		case gapi::DX9: toolDX9.begin(viewport, mode); break;
		case gapi::DX11: toolDX11.begin(viewport, mode); break;
		}
	}

	void ToolSwitcher::end()
	{
		switch (api)
		{
		case gapi::DX9: toolDX9.end(); break;
		case gapi::DX11: toolDX11.end(); break;
		}
	}*/


}




