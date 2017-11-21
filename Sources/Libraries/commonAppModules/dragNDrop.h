#pragma once

#include "window.h"

namespace AppModules
{
	class DragNDrop : public ModuleBase<DragNDrop, Loki::MakeTypelist<Window>>
	{
	public:	
		static LRESULT DragNDropWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
		template<class Params>  void init(Params& p)		{ create(p); }

		void create(Window::Params& windowParams);
	};
}