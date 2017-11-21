#pragma once
#include "unigui/library.include.h"
#include "containers/library.include.h"

#include <Windows.h>
#include <functional>
#include <array>

namespace WindowsGUI
{
	template<class FunctionType, std::size_t maxCount>
	class Functions : public std::array<std::function<FunctionType>, maxCount>
	{
	public:
		Functions(){}
		void add(FunctionType func)	{ this->at(cursor++) = std::move(func); }
		void reset()
		{
			cursor = 0;
			for (auto& func : *this)
			{
				func = std::move(std::function<FunctionType>());
			}
		}

	private:
		std::size_t cursor = 0;
	};

	//
	//
	//
	struct Rect : public unigui::math::Rect<int>
	{
		typedef unigui::math::Rect<int> UniRect;
		inline Rect():UniRect(){}
		inline Rect(int x_, int y_, int w_, int h_):UniRect(x_, y_, w_, h_){}
		inline Rect(const RECT& rect):UniRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top){}
	};

	//
	//
	//
	struct WindowParams
	{
	public:
		WindowParams();

		std::wstring name;
		Rect rect;

		bool operator<(const WindowParams& params) const { return name < params.name; }

		friend stream::ostream& operator<<(stream::ostream& os, const WindowParams& params);
		friend stream::istream& operator>>(stream::istream& is, WindowParams& params);
	};

	struct WindowsRegistry : public std::set<WindowParams> 
	{
		void add(const std::wstring& name);
		void add(const std::wstring& name, const Rect& rect);
		void merge(const WindowsRegistry& params);
		bool exist(const std::wstring& name) const;

		friend stream::ostream& operator<<(stream::ostream& os, const WindowsRegistry& params);
		friend stream::istream& operator>>(stream::istream& is, WindowsRegistry& params);
	};

	//
	//
	//
	class Window;
	class WindowClass
	{
	public:
		WindowClass();
		~WindowClass();

		void create(Window& window, const WindowParams& params) const;
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WindowProcs(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		WNDCLASSEX wndclass;
	};

	//
	//
	//
	class Window
	{
	friend class WindowClass;
	template<class WindowType> friend class Windows; 
	public:
		Window();
		virtual ~Window() {}

		virtual void init(){}
		virtual void activate(){}

		virtual void save() const;
		virtual void load();

		void create(const WindowClass& wndclass, const WindowParams& params);
		void close();
		void show(int cmd);
		void fullscreen(bool enabled, bool windowed);
		void toggeleFullscreen();

		void defaultStyle();
		void noborderStyle();

		bool positionChanged() const;
		bool sizeChanged() const;
		void rect(const Rect& rect);
		Rect rect() const;
		Rect clientRect() const;
		const std::wstring& name() const { return wndName; }
		void caption(const std::wstring& text) const;
	protected:
		WINDOWPLACEMENT placement() const;
		void placement(const WINDOWPLACEMENT& place);

		Functions<LRESULT (HWND, UINT, WPARAM, LPARAM), 16>* windowProcs;

		std::wstring wndName;
		HWND hwnd;
		HDC hdc;
		
		LONG style;
		LONG exStyle;
		WINDOWPLACEMENT place;
		WINDOWPLACEMENT backupPlace;
		mutable Rect currentRect;

		struct FullscreenState
		{
			FullscreenState():fullscreen(false), windowed(true){}
			bool fullscreen;
			bool windowed;			
		};	

		FullscreenState fullscreenState;
	};

	template<class WindowType>
	class WindowT : public Window, public Base::IntrusiveList<WindowType>::Node
	{
	public:
		virtual ~WindowT()
		{
			this->remove();
		}
	};

	//
	//
	//
	template<class WindowType>
	class Windows : public Base::IntrusiveList<WindowType>
	{
	public:		
		template<class FunctionType, int maxCount>
		WindowType* add(const WindowParams& params, Functions<FunctionType, maxCount>* funcs = 0)
		{
			WindowType* window = create();
			window->create(wndclass, params);
			window->load();
			window->activate();
			window->init();			
			//window->show(SW_SHOWDEFAULT);
			window->windowProcs = funcs;

			this->push_back(*window);

			return window;
		}

		template<class FunctionType, int maxCount>
		void createWindows(const WindowsRegistry& params, Functions<FunctionType, maxCount>* funcs = 0)
		{
			for(auto& wnd : params)
			{
				add(wnd, funcs);		
			}	
		}

		void closeWindows()
		{
			std::vector<WindowType*> windows;
			for (auto& wnd : *this)
			{
				windows.push_back(&wnd);
			}

			for (auto& wnd : windows)
			{
				wnd->close();
			}
		}

	protected:
		WindowType* create() { return new WindowType(); }

	private:		
		WindowClass wndclass;
	};
}