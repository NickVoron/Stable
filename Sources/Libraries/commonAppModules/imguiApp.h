// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "imgui.h"
#include "settings.h"
#include "debugDrawing.h"
#include "window.h"

#include "imguiModules/library.include.h"


namespace AppModules
{
	class ImguiAppBase
	{
	public:
		void release();
		void process();
	};

	template<class Prologue>
	class ImguiAppBaseT : public ImguiAppBase, public ModuleBase<ImguiAppBaseT<Prologue>, Prologue>
	{
	public:
		template<class Params>  void release(Params& p)	{ ImguiAppBase::release();	}
		template<class Params>  void process(Params& p)	{ ImguiAppBase::process();	}

		template<class WindowType>
		void create(WindowsGUI::Windows<WindowType>& windows)
		{
			imgui::modules::Application::load();
		}
	};

	struct ImguiApp9	: public ImguiAppBaseT<Loki::MakeTypelist<Window, Imgui9, Settings, DebugDrawingModule9>> { };
	struct ImguiApp11	: public ImguiAppBaseT<Loki::MakeTypelist<Window, Imgui11, Settings, DebugDrawingModule11>>
	{
		template<class Params>  void init(Params& p)		{	create(p.windows);	}	
	};
}



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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