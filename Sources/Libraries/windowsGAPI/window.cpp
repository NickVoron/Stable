// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "window.h"
#include "settingsRegistry/library.include.h"

namespace WindowsGAPI
{	
	ClearBuffersData::ClearBuffersData()
	{
		color		= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
		depth		= 1.0f;
		stencil		= 0;

		clearDepth		= true;
		clearColor		= true;
		clearStencil	= false;
	}

	
	
	
	void Window11::init()
	{
		int deviceIndex = 0;
		surface.init(deviceIndex, hwnd, true, false);
		viewport.init(name(), surface);

		caption(::dx11::DeviceDescription(deviceIndex));

		surface.set();
	}

	void Window11::present()
	{
		surface.present();
	}

	void Window11::save() const
	{
		WindowsGUI::Window::save();
		SettingsRegistry::add("windowsParams_window_" + Base::StrUtils::Convert(name()) + "_camera", viewport.camera.defaultParams());
	}

	void Window11::load()
	{
		WindowsGUI::Window::load();
		SettingsRegistry::get("windowsParams_window_" + Base::StrUtils::Convert(name()) + "_camera", viewport.camera.defaultParams());
	}



	void Window11::clearImpl(const ClearBuffersData& params)
	{
		viewport.clearSecondary();

		surface.set();

		if(params.clearColor)
		{
			surface.clearColor(params.color);
		}	

		if(params.clearStencil || params.clearDepth)
		{
			UINT flags = 0;
			if(params.clearDepth) flags |= D3D11_CLEAR_DEPTH;
			if(params.clearStencil) flags |= D3D11_CLEAR_STENCIL;
			surface.clearDepthStencil(flags, params.depth, params.stencil);
		}	
	}

	
	
	
	void Window9::init()
	{
		dx9::DeviceCreate(D3DFMT_A8R8G8B8);
		dx9::DeviceInit(hwnd, true, 0, 0);
		viewport.name = name();
	}

	void Window9::present()
	{
		if( dx9::DeviceIsValid() )
		{
			dx9::device->EndScene();
			dx9::device->Present(0,0,0,0);
		}
	}

	void Window9::clearImpl(const ClearBuffersData& params)
	{
		if( dx9::DeviceIsValid() )
		{
			dx9::device->BeginScene();
			UINT flags = 0;
			if (params.clearDepth)		flags |= D3DCLEAR_ZBUFFER;
			if (params.clearStencil)	flags |= D3DCLEAR_STENCIL;
			if (params.clearColor)		flags |= D3DCLEAR_TARGET;

			dx9::device->Clear(0, 0, flags, 0, params.depth, params.stencil); 																 			
		}
	}

	
	
	
	
}



// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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