// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "device.h"
#include "dx9/interfaces.h"
#include "common/delete.h"

namespace dx9 
{

	bool deviceIsNullReference = false;

	void CreateNULLRefDevice()
	{
		CreateNULLRefDevice(GetConsoleWindow());	
	}

	void CreateNULLRefDevice(HWND hwnd)
	{
		if(device)	return;

		HRESULT hr;
		d3d = Direct3DCreate9( D3D_SDK_VERSION );
		ENFORCE(d3d);

		D3DDISPLAYMODE Mode;
		d3d->GetAdapterDisplayMode( 0, &Mode );

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory( &pp, sizeof( D3DPRESENT_PARAMETERS ) );
		pp.BackBufferWidth = 1;
		pp.BackBufferHeight = 1;
		pp.BackBufferFormat = Mode.Format;
		pp.BackBufferCount = 1;
		pp.SwapEffect = D3DSWAPEFFECT_COPY;
		pp.Windowed = TRUE;

		hr = d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE, &pp, &device);
		ENFORCE(device);
	}

	void ReleaseD3D()
	{
		SAFE_RELEASE(device);
		SAFE_RELEASE(d3d);
	}
} 



// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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