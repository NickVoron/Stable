// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "commonAppModules/library.include.h"
#include "applib_Params/library.include.h"
#include "applib_Main/library.include.h"

#if defined(USE_WINDOWS)

extern void setup(StdAppParams& params);
extern void init();
extern void gui();
extern void release();
extern void process(StdAppParams& params);

namespace AppModules
{
	struct ApplicationIMGUI : public imgui::modules::Module
	{
		virtual void process() { gui(); }

		virtual const char* name() const { return "Application"; }
	};

	struct ApplicationIMGUIModule : public ModuleBase <ApplicationIMGUIModule>
	{
		ApplicationIMGUI applicationIMGUI;
	};

	struct Application : public ModuleBase<Application>
	{
		struct Params
		{	
			std::vector<std::string> commandLine;
		};

		template<class Params> void init(Params& params)	{ ::init();							}
		template<class Params> void process(Params& params)	
		{ 
			StdAppParams inparams(params);
			::process(inparams);
		}

		template<class Params> void release(Params& params)	{ ::release();						}
	};

	typedef Loki::MakeTypelist<CrtMemDebugger, 
						Logs, 
						
						Console, 
						
						ApplicationIMGUIModule, 
						
						FBX_DX11, 
						ScreenLog11, 
						ImguiApp11, 
						DebugDrawingModule11, 
						Direct3D11, 
						Direct3D11FrameBegin, 
						ResourceWatcher, 
						Camera, 
						IPC,
						Timer, 
						Window, 
						WindowParams, 
						WindowsWatcher,
						DragNDrop, 
						DefaultLightSources, 
						ScreenMouseHelper, 
						CameraCreator, 
						ProcessTerminator, 
						AppModules::ComponentModel,
						ComponentModelHelpers,

						Application, 
						DebugDrawingFlusher, 
						Manipulators3D, 
						UniguiFlusher11, 
						Direct3D11FrameEnd,
						UTPNet
						> Prologue;


}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPWSTR *argvw = CommandLineToArgvW(GetCommandLineW(), &argc);

	char** argv = (char**)alloca(argc);

	for (int i = 0; i < argc; ++i)
	{
		int w_len = lstrlenW(argvw[i]);
		int len = WideCharToMultiByte(CP_ACP, 0, argvw[i], w_len, NULL, 0, NULL, NULL);
		argv[i] = (char*)alloca(len + 1);
		argv[i][len] = 0;

		WideCharToMultiByte(CP_ACP, 0, argvw[i], w_len, argv[i], len + 1, NULL, NULL);
	}

	LocalFree(argvw);

	return appmain<AppModules::Prologue, 0, false>(&setup, argc, argv);
}

#endif



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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