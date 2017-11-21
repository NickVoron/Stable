#pragma once

#include "inputSysWin/library.include.h"

#include "window.h"
#include "commandProcessor.h"

namespace AppModules
{
	class WinInput : public ModuleBase<WinInput, Loki::MakeTypelist<CommandProcessor>, Loki::MakeTypelist<Window>>
	{
	public:
		template<class Params>  void init(Params& p)		{ initImpl();		}
		template<class Params>  void after_init(Params& p)	{ afterInitImpl(p); }
		template<class Params>  void before_process(Params& p)	{ beforeProcessImpl(); }
		template<class Params>  void process(Params& p)	{ processImpl();	}
		
		static LRESULT InputWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		void initImpl();
		void afterInitImpl(Window::Params& params);
		void beforeProcessImpl();
		void processImpl();
		
	};
}