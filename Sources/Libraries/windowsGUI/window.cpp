// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "window.h"
#include "settingsRegistry/library.include.h"

namespace WindowsGUI
{	
	
	
	
	stream::ostream& operator<<(stream::ostream& os, const WindowParams& params)	{	return os << params.rect << params.name;	}
	stream::istream& operator>>(stream::istream& is, WindowParams& params)			{	return is >> params.rect >> params.name;	}

	stream::ostream& operator<<(stream::ostream& os, const WindowsRegistry& params)	{	return os << (const std::set<WindowParams>&)params;	}
	stream::istream& operator>>(stream::istream& is, WindowsRegistry& params)		{	return is >> (std::set<WindowParams>&)params;		}

	
	
	
	WindowParams::WindowParams() : rect(100, 100, 1024, 768)
	{
	}

	
	
	
	bool WindowsRegistry::exist(const std::wstring& name) const
	{
		WindowParams params;
		params.name = name;

		return find(params) != end();
	}

	void WindowsRegistry::add(const std::wstring& name)
	{
		if( !exist(name) )
		{
			WindowParams params;
			params.name = name;
			insert(params);	
		}		
	}

	void WindowsRegistry::add(const std::wstring& name, const Rect& rect)
	{
		if( !exist(name) )
		{
			WindowParams params;
			params.name = name;
			params.rect = rect;
			insert(params);	
		}
	}

	void WindowsRegistry::merge(const WindowsRegistry& params)
	{
		for(auto& wnd : params)
		{
			insert(wnd);
		}
	}

	
	
	
	WindowClass::WindowClass()
	{
		memset(&wndclass, 0, sizeof(wndclass));

		wndclass.cbSize        = sizeof(wndclass);
		wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.lpfnWndProc   = &WndProc;
		wndclass.hInstance     = GetModuleHandle(0);
		wndclass.lpszClassName = L"AppModulesWndClass1";
		wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);

		RegisterClassEx(&wndclass);
	}

	WindowClass::~WindowClass()
	{
		UnregisterClass(wndclass.lpszClassName, wndclass.hInstance);
	}

	void HandleLastError() 
	{
		DWORD errCode = GetLastError();
		if (errCode)
		{
			char *err;
			if (!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				errCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(LPSTR) &err,
				0,
				NULL))
				return;

			LOG_ERROR(err);
			LocalFree(err);
		}
	}

	void WindowClass::create(Window& window, const WindowParams& params) const
	{
		window.style	= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;        
		window.exStyle  = WS_EX_APPWINDOW;

		const auto& pos = params.rect.pos;
		const auto& size = params.rect.size;
		window.hwnd = CreateWindowEx(window.exStyle, wndclass.lpszClassName, params.name.c_str(), window.style, pos.x, pos.y, size.x, size.y, NULL, NULL, wndclass.hInstance, NULL);				
		HandleLastError();
		
		SetWindowLongPtr(window.hwnd, GWLP_USERDATA, (LONG_PTR)&window);
		HandleLastError();
	}
	
	LRESULT CALLBACK WindowClass::WindowProcs(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WindowsGUI::Window* window = (WindowsGUI::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if(window && window->windowProcs)
		{
			for (auto& func : *window->windowProcs)
			{
				if (!func)
				{
					break;
				}
				else if(LRESULT res = func(hwnd, message, wParam, lParam))
				{
					return res;
				}
			}	
		}		

		return 0;
	}

	LRESULT CALLBACK WindowClass::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
		case WM_DESTROY:
			{
				WindowProcs(hwnd, message, wParam, lParam);

				LONG_PTR data = GetWindowLongPtr(hwnd, GWLP_USERDATA);
				WindowsGUI::Window* window = (WindowsGUI::Window*)data;

				SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
				delete window;
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
		};

		LRESULT res = WindowProcs(hwnd, message, wParam, lParam);

		return res ? res : DefWindowProc(hwnd, message, wParam, lParam);
	}

	
	
	
	Window::Window()
	{
		hwnd = 0;
		hdc	= 0;
		windowProcs = 0;
	}

	void Window::create(const WindowClass& wndclass, const WindowParams& params)
	{
		wndName = params.name;
		wndclass.create(*this, params);
		
		
		sizeChanged();
		positionChanged();

		hdc = GetDC(hwnd);
				
		DragAcceptFiles(hwnd, TRUE);
		RegisterHotKey(hwnd, 0, 0, VK_SNAPSHOT);
	}

	void Window::caption(const std::wstring& text) const
	{
		SetWindowText(hwnd, text.c_str());
	}

	bool Window::positionChanged() const
	{
		Rect r = rect();
		bool res = (currentRect.pos != r.pos);
		currentRect.pos = r.pos;

		return res;
	}

	bool  Window::sizeChanged() const
	{
		Rect r = rect();
		bool res = (currentRect.size != r.size);
		currentRect.size = r.size;

		return res;
	}
	
	void Window::defaultStyle()
	{
		SetWindowLong(hwnd, GWL_STYLE, style);
		SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
		SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	void Window::noborderStyle()
	{
		LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		SetWindowLong(hwnd, GWL_STYLE, lStyle);

		LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);

		SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}

	void Window::toggeleFullscreen()
	{
		fullscreen(!fullscreenState.fullscreen, true);
	}

	void Window::fullscreen(bool enabled, bool windowed)
	{
		if(fullscreenState.fullscreen != enabled || fullscreenState.windowed != windowed)
		{
			fullscreenState.fullscreen = enabled;
			fullscreenState.windowed = windowed;

			if(windowed)
			{
				if(enabled)
				{
					backupPlace = placement();

					noborderStyle();
					place = placement();					
					place.showCmd = SW_MAXIMIZE;
					placement(place);
				}
				else
				{				
					defaultStyle();
					placement(backupPlace);
					InvalidateRect(NULL, NULL, TRUE);
				}
			}
			else
			{

			}	
		}			
	}

	void Window::save() const
	{
		SettingsRegistry::add("windowsParams_window_" + Base::StrUtils::Convert(name()), std::make_tuple(placement(), backupPlace, fullscreenState));
	}

	void Window::load()
	{
		WINDOWPLACEMENT place = placement();
		WINDOWPLACEMENT backPlace;
		SettingsRegistry::get("windowsParams_window_" + Base::StrUtils::Convert(name()), std::tie(placement(), backPlace, fullscreenState));
				
 		placement(place);

 		if(fullscreenState.fullscreen)
 		{
 			
 			fullscreenState.fullscreen = !fullscreenState.fullscreen;
 			fullscreen(!fullscreenState.fullscreen, fullscreenState.windowed);
 		}		

		backupPlace = backPlace;
	}

	void Window::close()
	{
		DestroyWindow(hwnd);
	}

	void Window::show(int cmd)
	{
		ShowWindow(hwnd, cmd);
	}

	void Window::rect(const Rect& rect)
	{
		SetWindowPos(hwnd, 0, rect.left(), rect.top(), rect.size.x, rect.size.y, 0);
	}

	Rect Window::rect() const		{	RECT rc;	GetWindowRect(hwnd, &rc); return Rect(rc);		}
	Rect Window::clientRect() const	{	RECT rc;	GetClientRect( hwnd, &rc ); return Rect(rc);	}

	WINDOWPLACEMENT Window::placement() const
	{
		WINDOWPLACEMENT result;
		result.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(hwnd, &result);

		return result;
	}

	void Window::placement(const WINDOWPLACEMENT& place)
	{
		WINDOWPLACEMENT result = place;
		result.length = sizeof(WINDOWPLACEMENT);
		SetWindowPlacement(hwnd, &result);

		sizeChanged();
	}
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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