// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "appModule/library.include.h"
#include "windowsGUI/library.include.h"

#include "settings.h"

namespace AppModules
{

	class Window : public ModuleBase<Window, Loki::MakeTypelist<Settings>>
	{
	public:
		struct Params
		{
			Params();

			template<class WindowType>
			void createWindows(WindowsGUI::Windows<WindowType>& windows)
			{
				windows.createWindows(windowsParams, &windowProcs);
			}
		
			WindowsGUI::WindowsRegistry windowsParams;
			WindowsGUI::Functions<LRESULT (HWND, UINT, WPARAM, LPARAM), 16> windowProcs;
			bool needExit;
		};

		template<class Params>  void process(Params& p) { process_impl(); p.exit = p.needExit; }
		template<class Params>  void release(Params& p) { release_impl(p); }
	
		void process_impl();
		void release_impl(Params& params);

		MSG msg;
	};

	class WindowsWatcher : public ModuleBase<WindowsWatcher, Loki::MakeTypelist<Window>>
	{
	public:
		template<class Params>  void process(Params& p) 
		{ 
			p.needExit = p.windows.empty();
		}
	};


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