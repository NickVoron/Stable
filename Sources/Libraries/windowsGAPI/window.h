#pragma once

#include <memory>

#include "windowsGUI/library.include.h"

#include "dx9/library.include.h"
#include "dx11/library.include.h"

#include "opengl/library.include.h"
#include "opengles/library.include.h"

namespace WindowsGAPI
{
	struct ClearBuffersData 
	{
		ClearBuffersData();

		Vector4 color;
		float depth;
		unsigned char stencil;

		bool clearDepth;
		bool clearColor;
		bool clearStencil;
	};

	//
	//
	//
	template<class WindowType>
	class WindowCommon : public WindowsGUI::WindowT<WindowType>
	{
	public:
		virtual void present() = 0;
		void clear(const ClearBuffersData& params) 
		{
			defaultClearData = params;
			clear();
		}

		void clear() { clearImpl(defaultClearData); }

		ClearBuffersData defaultClearData;

	protected:
		virtual void clearImpl(const ClearBuffersData& params) = 0;
	};

	template<class WindowType, class ViewportType>
	class ViewportWindow : public WindowCommon<WindowType>
	{
	public:
		virtual void activate() 
		{
			Viewports::viewports.push_front(viewport);
		}

		ViewportType viewport;		
	};

	template<class WindowType>
	class WindowsCommon : public WindowsGUI::Windows<WindowType>
	{
	public:
		void present()
		{
			for (auto& wnd : *this)
			{
				wnd.present();
			}
		}

		void clear()
		{
			for (auto& wnd : *this)
			{
				wnd.clear();
			}
		}
	};

	//
	//
	//
	class Window9 : public ViewportWindow<Window9, dx9::Viewport>
	{
	public:
		virtual void present();
		virtual void init();
		
	protected:		
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class Windows9 : public WindowsCommon<Window9> {};

	//
	//
	//
	class Window11 : public ViewportWindow<Window11, dx11::Viewport>
	{
	public:
		virtual void present();
		virtual void init();
		virtual void save() const;
		virtual void load();
		
		dx11::OutputSurface surface;
	protected:		
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class Windows11 : public WindowsCommon<Window11> {};

	//
	//
	//
	class WindowGL : public ViewportWindow<WindowGL, opengl::Viewport>
	{
	public:
		virtual void present();
		virtual void init();

		opengl::OpenGL context;
	protected:
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class WindowsGL : public WindowsCommon<WindowGL> {};
	

	/*
	//
	//
	//
	class WindowGLES : public ViewportWindow<WindowGLES, opengl::Viewport>
	{
	public:
		virtual void present();
		virtual void init();

		std::unique_ptr<EGLWindow> eglWindow;
	protected:
		virtual void clearImpl(const ClearBuffersData& params);
	};
	class WindowsGLES : public WindowsCommon<WindowGLES> {};	
	*/

}