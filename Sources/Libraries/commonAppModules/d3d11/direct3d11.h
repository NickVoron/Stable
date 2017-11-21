#pragma once

#include "dx11/library.include.h"
#include "windowsGAPI/library.include.h"
#include "viewports/library.include.h"

#include "../window.h"

namespace AppModules
{

class Direct3D11 : public ModuleBase<Direct3D11, Loki::MakeTypelist<Window>>
{
public:
	struct Params
	{
		WindowsGAPI::Windows11 windows;
	};
	
	template<class Params>  void init(Params& p)	 { create(p, p);			}
	template<class Params>  void release(Params& p) { release();				}

	void create(Window::Params& windowParams, Params& params);
	void release();

	WindowsGAPI::Windows11* windows = nullptr;
};

class Direct3D11FrameEnd;
class Direct3D11FrameBegin : public ModuleBase<Direct3D11FrameBegin, Loki::MakeTypelist<Direct3D11>, Loki::MakeTypelist<Direct3D11FrameEnd>>
{
public:
	struct Params : public WindowsGAPI::ClearBuffersData
	{
		Params();
	};

	template<class Params>  void init(Params& p)		{	create(p);			}
	template<class Params>  void process(Params& p)	{	process(p.windows);	}

	void create(Params& params);
	void process(WindowsGAPI::Windows11& windows);
};

class Direct3D11FrameEnd : public ModuleBase<Direct3D11FrameEnd, Loki::MakeTypelist<Direct3D11FrameBegin>>
{
public:
	template<class Params>  void process(Params& p)	{ process(p.windows); }

	void process(WindowsGAPI::Windows11& windows);
};

struct DriverD3D11 : public ModuleBase<DriverD3D11, Loki::MakeTypelist<Direct3D11, Direct3D11FrameBegin>>
{
};

}
