#pragma once

#include "opengl/library.include.h"
#include "windowsGAPI/library.include.h"
#include "../window.h"

namespace AppModules
{
class OpenGL : public ModuleBase<OpenGL, Loki::MakeTypelist<Window>>
{
public:
	struct Params
	{
		WindowsGAPI::WindowsGL windows;
	};

	template<class Params>  void init(Params& p)	 { create(p, p);	}
	template<class Params>  void release(Params& p) { release();		}

	void create(Window::Params& windowParams, Params& params);
	void release();
};

class OpenGLFrameEnd;
class OpenGLFrameBegin : public ModuleBase<OpenGLFrameBegin, Loki::MakeTypelist<OpenGL>, Loki::MakeTypelist<OpenGLFrameEnd>>
{
public:
	struct Params : public WindowsGAPI::ClearBuffersData
	{
		Params();
	};

	template<class Params>  void init(Params& p)		{	create(p);				}
	template<class Params>  void process(Params& p)	{	update(p, p.windows);	}

	void create(Params& params);
	void update(Params& params, WindowsGAPI::WindowsGL& windows);
};

class OpenGLFrameEnd : public ModuleBase<OpenGLFrameEnd, Loki::MakeTypelist<OpenGLFrameBegin>>
{
public:
	template<class Params>  void process(Params& p) { process(p.windows); }

	void process(WindowsGAPI::WindowsGL& windows);
};


}
