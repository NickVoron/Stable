#include "windowParams.h"

namespace AppModules
{

LRESULT WindowParams::WindowParamsWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	WindowsGUI::Window* window = (WindowsGUI::Window*)data;

	bool needReInit = window->sizeChanged();
	bool needSave = window->positionChanged() | needReInit;
	

	switch (message)
	{
	case WM_CLOSE: 
		{
			needSave = true;
			break;
		} 
	case WM_KEYDOWN:
		{
			UINT keyCode = ( BYTE )( wParam & 0xFF );
			switch(keyCode)
			{
			case VK_F11:
				{
					window->toggeleFullscreen();					
					break;
				}
			};
			
		}
	}

	if(needSave)
	{
		window->save();
		SettingsRegistry::save();
	}

	if(needReInit)
	{
		window->init();
	}

	return 0;
}

void WindowParams::create(Window::Params& windowParams)
{
	windowParams.windowProcs.add(&WindowParamsWndProc);
}


}