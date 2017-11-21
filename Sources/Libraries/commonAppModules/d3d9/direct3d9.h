#pragma once

#include "imguiModules/library.include.h"
#include "windowsGAPI/library.include.h"

#include "../window.h"

namespace AppModules
{

class Direct3D9 : public ModuleBase<Direct3D9, Loki::MakeTypelist<Window>>
{
public:
	struct Params
	{
		WindowsGAPI::Windows9 windows;				
	};

	template<class Params>  void init(Params& p)	 { create(p, p);	}
	template<class Params>  void release(Params& p) { release();		}

	void create(Window::Params& windowParams, Params& params);
	void release();
};

class Direct3D9FrameEnd;
class Direct3D9FrameBegin : public ModuleBase<Direct3D9FrameBegin, Loki::MakeTypelist<Direct3D9>, Loki::MakeTypelist<Direct3D9FrameEnd>>
{
public:
	struct Params : public WindowsGAPI::ClearBuffersData
	{
		Params();
	};

	template<class Params>  void init(Params& p)		{ create(p);			}
	template<class Params>  void process(Params& p)	{ process(p.windows); }

	void create(Params& params);
	void process(WindowsGAPI::Windows9& windows);
};

class Direct3D9FrameEnd : public ModuleBase<Direct3D9FrameEnd, Loki::MakeTypelist<Direct3D9FrameBegin>>
{
public:
	template<class Params>  void process(Params& p)	{	process(p.windows9);	}

	void process(WindowsGAPI::Windows9& windows);
};

class DriverD3D9 : public ModuleBase<DriverD3D9, Loki::MakeTypelist<Direct3D9, Direct3D9FrameBegin>>
{
public:
	template<class Params>  void init(const Params& p){};

	template<class Params>  void release(Params& params)	
	{
		dx9::VertexBuffer::clearAll();
		dx9::IndexBuffer::clearAll();
		dx9::VertexDeclaration::releaseAllDeclarations();
	};
};

}
