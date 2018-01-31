// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <memory>

#include "windowsGUI/library.include.h"

#include "dx9/library.include.h"
#include "dx11/library.include.h"

#include "opengl/library.include.h"
#include "opengles/library.include.h"

namespace WindowsGAPI
{
	struct ClearBuffersData 
	{
		ClearBuffersData();

		Vector4 color;
		float depth;
		unsigned char stencil;

		bool clearDepth;
		bool clearColor;
		bool clearStencil;
	};

	
	
	
	template<class WindowType>
	class WindowCommon : public WindowsGUI::WindowT<WindowType>
	{
	public:
		virtual void present() = 0;
		void clear(const ClearBuffersData& params) 
		{
			defaultClearData = params;
			clear();
		}

		void clear() { clearImpl(defaultClearData); }

		ClearBuffersData defaultClearData;

	protected:
		virtual void clearImpl(const ClearBuffersData& params) = 0;
	};

	template<class WindowType, class ViewportType>
	class ViewportWindow : public WindowCommon<WindowType>
	{
	public:
		virtual void activate() 
		{
			Viewports::viewports.push_front(viewport);
		}

		ViewportType viewport;		
	};

	template<class WindowType>
	class WindowsCommon : public WindowsGUI::Windows<WindowType>
	{
	public:
		void present()
		{
			for (auto& wnd : *this)
			{
				wnd.present();
			}
		}

		void clear()
		{
			for (auto& wnd : *this)
			{
				wnd.clear();
			}
		}
	};

	
	
	
	class Window9 : public ViewportWindow<Window9, dx9::Viewport>
	{
	public:
		virtual void present();
		virtual void init();
		
	protected:		
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class Windows9 : public WindowsCommon<Window9> {};

	
	
	
	class Window11 : public ViewportWindow<Window11, dx11::Viewport>
	{
	public:
		virtual void present();
		virtual void init();
		virtual void save() const;
		virtual void load();
		
		dx11::OutputSurface surface;
	protected:		
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class Windows11 : public WindowsCommon<Window11> {};

	
	
	
	class WindowGL : public ViewportWindow<WindowGL, opengl::Viewport>
	{
	public:
		virtual void present();
		virtual void init();

		opengl::OpenGL context;
	protected:
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class WindowsGL : public WindowsCommon<WindowGL> {};
	

	

}



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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