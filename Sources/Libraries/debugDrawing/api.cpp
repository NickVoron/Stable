// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "api.h"

namespace DebugDrawing
{
	
	
	
	ShaderDataPerObject::ShaderDataPerObject()
	{
		mem::memzero(colorFromVertex);
	}

	
	
	
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

		case gapi::DX11: toolDX11.init(fxFileName); break;
		}
	}

	void ToolSwitcher::uploadPerCamera(Viewports::Viewport& viewport, const ShaderDataPerCamera& data)
	{
		switch (api)
		{

		case gapi::DX11: toolDX11.uploadPerCamera(viewport, data); break;
		}
	}




}








// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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