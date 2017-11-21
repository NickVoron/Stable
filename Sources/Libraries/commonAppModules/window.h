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