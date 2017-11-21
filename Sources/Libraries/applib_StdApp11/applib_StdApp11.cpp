#include "commonAppModules/library.include.h"
#include "applib_Params/library.include.h"
#include "applib_Main/library.include.h"

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
						QtLog,
						Console, 
						//UDTNet, 
						ApplicationIMGUIModule, 
						//OpenAL,
						FBX_DX11, 
						ScreenLog11, 
						ImguiApp11, 
						DebugDrawingModule11, 
						Direct3D11, 
						Direct3D11FrameBegin, 
						ResourceWatcher, 
						Camera, 
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
//						RenderPipeline, 
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