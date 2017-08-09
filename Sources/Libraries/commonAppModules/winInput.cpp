// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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