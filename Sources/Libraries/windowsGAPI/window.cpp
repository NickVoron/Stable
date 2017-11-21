#include "window.h"
#include "settingsRegistry/library.include.h"

namespace WindowsGAPI
{	
	ClearBuffersData::ClearBuffersData()
	{
		color		= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
		depth		= 1.0f;
		stencil		= 0;

		clearDepth		= true;
		clearColor		= true;
		clearStencil	= false;
	}

	//
	//
	//
	void Window11::init()
	{
		int deviceIndex = 0;
		surface.init(deviceIndex, hwnd, true, false);
		viewport.init(name(), surface);

		caption(::dx11::DeviceDescription(deviceIndex));

		surface.set();
	}

	void Window11::present()
	{
		surface.present();
	}

	void Window11::save() const
	{
		WindowsGUI::Window::save();
		SettingsRegistry::add("windowsParams_window_" + Base::StrUtils::Convert(name()) + "_camera", viewport.camera.defaultParams());
	}

	void Window11::load()
	{
		WindowsGUI::Window::load();
		SettingsRegistry::get("windowsParams_window_" + Base::StrUtils::Convert(name()) + "_camera", viewport.camera.defaultParams());
	}



	void Window11::clearImpl(const ClearBuffersData& params)
	{
		viewport.clearSecondary();

		surface.set();

		if(params.clearColor)
		{
			surface.clearColor(params.color);
		}	

		if(params.clearStencil || params.clearDepth)
		{
			UINT flags = 0;
			if(params.clearDepth) flags |= D3D11_CLEAR_DEPTH;
			if(params.clearStencil) flags |= D3D11_CLEAR_STENCIL;
			surface.clearDepthStencil(flags, params.depth, params.stencil);
		}	
	}

	//
	//
	//
	void Window9::init()
	{
		dx9::DeviceCreate(D3DFMT_A8R8G8B8);
		dx9::DeviceInit(hwnd, true, 0, 0);
		viewport.name = name();
	}

	void Window9::present()
	{
		if( dx9::DeviceIsValid() )
		{
			dx9::device->EndScene();
			dx9::device->Present(0,0,0,0);
		}
	}

	void Window9::clearImpl(const ClearBuffersData& params)
	{
		if( dx9::DeviceIsValid() )
		{
			dx9::device->BeginScene();
			UINT flags = 0;
			if (params.clearDepth)		flags |= D3DCLEAR_ZBUFFER;
			if (params.clearStencil)	flags |= D3DCLEAR_STENCIL;
			if (params.clearColor)		flags |= D3DCLEAR_TARGET;

			dx9::device->Clear(0, 0, flags, 0, params.depth, params.stencil); 																 			
		}
	}

	//
	//
	//
	void WindowGL::init()
	{
		context.init(hdc);
	}

	void WindowGL::present()
	{
		SwapBuffers(hdc);
	}

	void WindowGL::clearImpl(const ClearBuffersData& params)
	{
		wglMakeCurrent(hdc, context.context());
		//glViewport(0, 0, clientRect().size.x, clientRect().size.y);		
		if(params.clearColor || params.clearDepth || params.clearStencil)
		{
			GLbitfield res = 0;

			if(params.clearColor)		
			{
				glClearColor(params.color.x, params.color.y, params.color.z, params.color.w);
				res |= GL_COLOR_BUFFER_BIT;
			}

			if(params.clearDepth)		
			{
				glClearDepthf(params.depth);
				res |= GL_DEPTH_BUFFER_BIT;
			}

			if(params.clearStencil)		
			{
				glClearStencil(params.stencil);
				res |= GL_STENCIL_BUFFER_BIT;
			}

			glClear(res);
		}
	}
				/*
	//
	//
	//
	void WindowGLES::present()
	{
		eglWindow->swap();
	}

	void WindowGLES::init()
	{
 		EGLint glesMajorVersion = 2;
 		EGLint glesMinorVersion = 0;
 		EGLint requestedRenderer = EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE;
 
 		eglWindow.reset(new EGLWindow(glesMajorVersion, glesMinorVersion, EGLPlatformParameters(requestedRenderer)));
		eglWindow->init(hwnd, hdc);
		viewport.init(clientRect().size.x, clientRect().size.y);
	}

	void WindowGLES::clearImpl(const ClearBuffersData& params)
	{
		// Set the viewport
		glViewport(0, 0, clientRect().size.x, clientRect().size.y);

		if (params.clearColor || params.clearDepth || params.clearStencil)
		{
			GLbitfield res = 0;

			if (params.clearColor)
			{
				glClearColor(params.color.x, params.color.y, params.color.z, params.color.w);
				res |= GL_COLOR_BUFFER_BIT;
			}

			if (params.clearDepth)
			{
				glClearDepthf(params.depth);
				res |= GL_DEPTH_BUFFER_BIT;
			}

			if (params.clearStencil)
			{
				glClearStencil(params.stencil);
				res |= GL_STENCIL_BUFFER_BIT;
			}

			glClear(res);
		}
	}
	*/
}