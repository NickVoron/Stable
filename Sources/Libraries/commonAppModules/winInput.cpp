#include "winInput.h"
#include <WindowsX.h>

namespace AppModules
{



Input::WinInput core;



void keyboard(unsigned int code, bool down)
{
	Input::WinInput* core = (Input::WinInput*)Input::InputSystem::get();
	ENFORCE(core);
	core->keyboardProc(code, down);
}

void mouse(bool leftDown, bool rightDown, bool middleDown, bool side1Down, bool side2Down, float wheelDelta, int xPos, int yPos)
{
	Input::WinInput* core = (Input::WinInput*)Input::InputSystem::get();
	ENFORCE(core);
	core->mouseProc(leftDown, rightDown, middleDown, side1Down, side2Down, wheelDelta, (float)xPos, (float)yPos);
}

LRESULT WinInput::InputWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_KEYDOWN ||
		uMsg == WM_SYSKEYDOWN ||
		uMsg == WM_KEYUP ||
		uMsg == WM_SYSKEYUP || 
		uMsg == WM_HOTKEY)
	{
		bool bKeyDown = ( uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN  || uMsg == WM_HOTKEY);
		DWORD dwMask = ( 1 << 29 );
		bool bAltDown = ( ( lParam & dwMask ) != 0 );

		UINT keyCode = (uMsg != WM_HOTKEY) ? ( BYTE )( wParam & 0xFF ) : HIWORD(lParam);


		if(wParam == VK_SHIFT)
		{
			keyboard(keyCode, bKeyDown);
			keyCode = (lParam & (1 << 24)) ? VK_LSHIFT : VK_RSHIFT;
		}

		if(wParam == VK_CONTROL)
		{
			keyboard(keyCode, bKeyDown);
			keyCode = (lParam & (1 << 24)) ? VK_RCONTROL : VK_LCONTROL;
		}

		if(wParam == VK_MENU)
		{
			keyboard(keyCode, bKeyDown);
			keyCode = (lParam & (1 << 24)) ? VK_RMENU : VK_LMENU;
		}

		keyboard(keyCode, bKeyDown);

		return true;
	}

	// Consolidate the mouse button messages and pass them to the app's mouse callback
	if( uMsg == WM_LBUTTONDOWN ||
		uMsg == WM_LBUTTONUP ||
		uMsg == WM_LBUTTONDBLCLK ||
		uMsg == WM_MBUTTONDOWN ||
		uMsg == WM_MBUTTONUP ||
		uMsg == WM_MBUTTONDBLCLK ||
		uMsg == WM_RBUTTONDOWN ||
		uMsg == WM_RBUTTONUP ||
		uMsg == WM_RBUTTONDBLCLK ||
		uMsg == WM_XBUTTONDOWN ||
		uMsg == WM_XBUTTONUP ||
		uMsg == WM_XBUTTONDBLCLK ||
		uMsg == WM_MOUSEWHEEL ||
		uMsg == WM_MOUSEMOVE )
	{

		int xPos = GET_X_LPARAM( lParam );
		int yPos = GET_Y_LPARAM( lParam );

		if( uMsg == WM_MOUSEWHEEL )
		{
			// WM_MOUSEWHEEL passes screen mouse coords
			// so convert them to client coords
			POINT pt;
			pt.x = xPos; pt.y = yPos;
			ScreenToClient( hWnd, &pt );
			xPos = pt.x; yPos = pt.y;
		}

		const float denominator = 120.0f;
		float mouseWheelDelta = 0;
		if( uMsg == WM_MOUSEWHEEL )
		{
			mouseWheelDelta = GET_WHEEL_DELTA_WPARAM( wParam ) / denominator;
		}

		int nMouseButtonState = GET_KEYSTATE_WPARAM( wParam );
		bool bLeftButton	= ( ( nMouseButtonState & MK_LBUTTON ) != 0 );
		bool bRightButton	= ( ( nMouseButtonState & MK_RBUTTON ) != 0 );
		bool bMiddleButton	= ( ( nMouseButtonState & MK_MBUTTON ) != 0 );
		bool bSideButton1	= ( ( nMouseButtonState & MK_XBUTTON1 ) != 0 );
		bool bSideButton2	= ( ( nMouseButtonState & MK_XBUTTON2 ) != 0 );

		mouse(bLeftButton, bRightButton, bMiddleButton, bSideButton1, bSideButton2, mouseWheelDelta, xPos, yPos);

		return true;
	}

	return false;
}


void WinInput::initImpl()
{
	core.init();
}

void WinInput::processImpl()
{
	core.update();
}

void WinInput::beforeProcessImpl()
{
	core.enabled(true);
}

void WinInput::afterInitImpl(Window::Params& params)
{
	params.windowProcs.add(&InputWndProc);
}

}