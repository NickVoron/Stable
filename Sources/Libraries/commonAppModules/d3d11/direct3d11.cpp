// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "imgui/library.include.h"
#include "imguiModules/library.include.h"

#include "direct3d11.h"
#include "../winInput.h"

namespace AppModules
{

struct D3D11CoreEditor : public imgui::modules::Module
{
	D3D11CoreEditor(Direct3D11FrameBegin::Params& p):params(p)
	{
		
	}

	virtual void process()
	{
		imgui::color("Clear Target", params.color);
	}

	virtual const char* name() const
	{
		return "Direct3D11 Core";
	}

	Direct3D11FrameBegin::Params& params;
};




void Direct3D11::create(Window::Params& windowParams, Params& params)
{
	dx11::DeviceCreate();

	windows = &params.windows;
	windowParams.createWindows(params.windows);
}

void Direct3D11::release()
{
	dx11::VertexShaderUtilStorage::release();
	windows->closeWindows();
	dx11::DeviceRelease();
}




Direct3D11FrameBegin::Params::Params()
{
	color	= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
	depth	= 1.0f;
	stencil	= 0;

	clearDepth		= true;
	clearColor		= true;
	clearStencil	= false;
}


void Direct3D11FrameBegin::create(Params& params)
{
	
}

void Direct3D11FrameBegin::process(WindowsGAPI::Windows11& windows)
{
	windows.clear();
}




void Direct3D11FrameEnd::process(WindowsGAPI::Windows11& windows)
{
	windows.present();
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