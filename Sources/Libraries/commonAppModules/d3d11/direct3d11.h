// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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