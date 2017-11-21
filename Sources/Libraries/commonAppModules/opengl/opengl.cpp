#pragma once

#include "imguiModules/library.include.h"
#include "opengl.h"

#include "../winInput.h"

namespace AppModules
{

	//
	//
	//
	OpenGLFrameBegin::Params::Params()
	{
		//color	= Vector4(0.7f, 0.425f, 0.0f, 1.0f);
		color = Vector4(Base::MathUtils::GetRandomThetha(), Base::MathUtils::GetRandomThetha(), Base::MathUtils::GetRandomThetha(), 1.0f);
		depth	= 1.0f;
		stencil	= 0;

		clearDepth		= true;
		clearColor		= true;
		clearStencil	= false;
	}

	//
	//
	//
	void OpenGL::create(Window::Params& windowParams, Params& params)
	{
		windowParams.createWindows(params.windows);
	}

	void OpenGL::release()
	{
	}


	//
	//
	//
	void OpenGLFrameBegin::create(Params& params)
	{

	}

	void OpenGLFrameBegin::update(Params& params, WindowsGAPI::WindowsGL& windows)
	{
 		windows.clear();
	}

	//
	//
	//
	void OpenGLFrameEnd::process(WindowsGAPI::WindowsGL& windows)
	{
		windows.present();
	}
}
