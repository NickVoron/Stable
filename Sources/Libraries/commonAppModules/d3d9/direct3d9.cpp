// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "settingsRegistry/library.include.h"

#include "direct3d9.h"
#include "../winInput.h"

namespace AppModules
{







Direct3D9FrameBegin::Params::Params()
{
	color	= Vector4(0.0f, 0.425f, 0.7f, 1.0f);
	depth	= 1.0f;
	stencil	= 0;

	clearDepth		= true;
	clearColor		= true;
	clearStencil	= false;
}



void Direct3D9::create(Window::Params& windowParams, Params& params)
{
	

	windowParams.createWindows(params.windows);
}

void Direct3D9::release()
{
	
}

void Direct3D9FrameBegin::create(Params& params)
{
	
}

void Direct3D9FrameBegin::process(WindowsGAPI::Windows9& windows)
{
	windows.clear();
}

void Direct3D9FrameEnd::process(WindowsGAPI::Windows9& windows)
{
	windows.present();
}

}




// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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