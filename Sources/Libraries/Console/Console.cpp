// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "Console.h"
#include "stuff/fileUtils.h"

#include <iostream>
#include <stdio.h>
#include <thread>

namespace Base
{

namespace
{
#ifdef WIN32
	HWND console_hwnd = 0;
#endif
}

#ifdef WIN32
BOOL CALLBACK enum_wnd(HWND hwnd, LPARAM param)
{
	char n[1000];
	::GetWindowTextA(hwnd, n, 1000);
	if(strcmp(n, Base::FileUtils::getModuleName().c_str()) == 0)
	{
		console_hwnd = hwnd;
		return false;
	}
	return true;
}
#endif



void Console::create()
{
#ifdef WIN32
	
	if(!::EnumChildWindows(GetDesktopWindow(), enum_wnd, 0))
	{
		
		::AllocConsole();                 
		freopen("CONOUT$", "a", stderr); 
		freopen("CONOUT$", "a", stdout); 
		freopen("CONIN$", "r", stdin);

		::SetConsoleTitleA(Base::FileUtils::getModuleName().c_str());

		

		
		if (console_hwnd)
		{
			RECT console_rect, desctop_rect;
			::GetWindowRect(console_hwnd, &console_rect);
			::GetWindowRect(GetDesktopWindow(), &desctop_rect);
			::SetWindowPos(console_hwnd, 0, 30, desctop_rect.bottom - (console_rect.bottom - console_rect.top) - 30, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
		}
	}
#endif
}

void Console::destroy()
{
#ifdef WIN32
	::FreeConsole();
#endif
}

int colorImpl(bool background, int clr)
{
	int res = 0;
#ifdef WIN32
	if(Console::COLOR_RED & clr)		res |= background ? BACKGROUND_RED			: FOREGROUND_RED;
	if(Console::COLOR_GREEN & clr)		res |= background ? BACKGROUND_GREEN		: FOREGROUND_GREEN;
	if(Console::COLOR_BLUE & clr)		res |= background ? BACKGROUND_BLUE			: FOREGROUND_BLUE;
	if(Console::COLOR_INTENSITY & clr)	res |= background ? BACKGROUND_INTENSITY	: FOREGROUND_INTENSITY;
#endif
	return res;
}

#ifdef WIN32
void setTextColor(WORD color)
{

	HANDLE hStdout; 
	hStdout = GetStdHandle( STD_OUTPUT_HANDLE ); 
	if ( hStdout == INVALID_HANDLE_VALUE ) return;
	SetConsoleTextAttribute(hStdout, color);
}
#endif

void Console::color(bool background, int c0)
{
#ifdef WIN32
	setTextColor( colorImpl(background, c0) );
#endif
}

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