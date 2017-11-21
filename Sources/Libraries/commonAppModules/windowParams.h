#pragma once

#include "window.h"

namespace AppModules
{

	class WindowParams : public ModuleBase<WindowParams, Loki::MakeTypelist<Window>>
	{
	public:	
		static LRESULT WindowParamsWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		template<class Params>  void init(Params& p)		{ create(p); }

		void create(Window::Params& windowParams);
	};

}									