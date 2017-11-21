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

	char commandBuffer[4096];
	bool exitFromThread = false;
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


//---------------------------------------------------------------------------
void Console::create()
{
#ifdef WIN32
	// create console
	::AllocConsole( );                 // Allocate console window
	freopen("CONOUT$", "a", stderr); // Redirect stderr to console
	freopen("CONOUT$", "a", stdout); // Redirect stdout also
	freopen("CONIN$", "r", stdin);

	::SetConsoleTitleA(Base::FileUtils::getModuleName().c_str());

	//::Sleep(100); // подождать создание окна

	// ищем HWND окна
	::EnumChildWindows(GetDesktopWindow(), enum_wnd, 0);

	// опустим окно вниз, чтоб было видно
	if(console_hwnd)
	{
		RECT console_rect, desctop_rect;
		::GetWindowRect(console_hwnd, &console_rect);
		::GetWindowRect(GetDesktopWindow(), &desctop_rect);
		::SetWindowPos(console_hwnd, 0, 30, desctop_rect.bottom - (console_rect.bottom - console_rect.top) - 30, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
	}

#endif
}

void Console::destroy()
{
	exitFromThread = true;
#ifdef WIN32
	::FreeConsole();
#endif
}

std::string Console::command()
{
	std::string commandString = commandBuffer;
	commandBuffer[0] = 0;
	return commandString;
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
