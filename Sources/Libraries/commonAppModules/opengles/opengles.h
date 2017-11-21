#pragma once

#include "opengles/library.include.h"
#include "windowsGAPI/library.include.h"
#include "../window.h"

namespace AppModules
{

/*
class OpenGLES : public ModuleBase<OpenGLES, Loki::MakeTypelist<Window>>
{
public:
	struct Params
	{
		WindowsGAPI::WindowsGLES windows;
	};

	template<class Params>  void init(Params& p)	 { create(p, p);	}
	template<class Params>  void release(Params& p) { release();		}

	void create(Window::Params& windowParams, Params& params);
	void release();
};

class OpenGLESFrameEnd;
class OpenGLESFrameBegin : public ModuleBase<OpenGLESFrameBegin, Loki::MakeTypelist<OpenGLES>, Loki::MakeTypelist<OpenGLESFrameEnd>>
{
public:
	struct Params : public WindowsGAPI::ClearBuffersData
	{
		Params();
	};

	template<class Params>  void init(Params& p)		{	create(p);				}
	template<class Params>  void process(Params& p)	{	update(p, p.windows);	}

	void create(Params& params);
	void update(Params& params, WindowsGAPI::WindowsGLES& windows);
};

class OpenGLESFrameEnd : public ModuleBase<OpenGLESFrameEnd, Loki::MakeTypelist<OpenGLESFrameBegin>>
{
public:
	template<class Params>  void process(Params& p) { process(p.windows); }

	void process(WindowsGAPI::WindowsGLES& windows);
};
*/

}
