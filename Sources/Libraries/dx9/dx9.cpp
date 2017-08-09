// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dx9.h"
#include "interfaces.h"
#include "device.h"

#include "defaultLogs/library.include.h"
#include "gapi/library.include.h"



namespace dx9
{

bool windowed = true;
int videoMode = 0;

struct Adapter
{
	char name[MAX_DEVICE_IDENTIFIER_STRING];

	struct DisplayMode
	{
		unsigned int width, height;
	};
	DisplayMode mode[32];
	unsigned int modeCount;
};
Adapter adapters[8];
unsigned int numAdapters;

D3DFORMAT format;

LPDIRECT3D9 DeviceCreate(D3DFORMAT fmt)
{
	format = fmt;

	d3d = Direct3DCreate9( D3D_SDK_VERSION );

	
	numAdapters = d3d->GetAdapterCount();
	for(unsigned int a=0; a<numAdapters; a++)
	{
		D3DADAPTER_IDENTIFIER9 pIdentifier;
		d3d->GetAdapterIdentifier(a, 0, &pIdentifier);
		strcpy(adapters[a].name, pIdentifier.Description);

		adapters[a].modeCount=0;
		unsigned int mc = d3d->GetAdapterModeCount(a, format);
		for(unsigned int m=0; m<mc; m++)
		{
			D3DDISPLAYMODE mode;
			d3d->EnumAdapterModes(a, format, m, &mode);

			
			unsigned int em;
			for(em=0; em<adapters[a].modeCount; em++)
				if(adapters[a].mode[em].width == mode.Width && adapters[a].mode[em].height == mode.Height)
					break;
			if(em==adapters[a].modeCount)
			{
				adapters[a].mode[ adapters[a].modeCount ].width = mode.Width;
				adapters[a].mode[ adapters[a].modeCount ].height = mode.Height;
				adapters[a].modeCount++;
			}
		}
	}

	return d3d;
}


void DeviceRelease()
{
	ULONG rc = 0;

	rc = device->Release(); device = 0;
	if(rc != 0)
		LOG_ERROR( "dev->Release(): ref count isn't 0: " << rc );
	
	rc = d3d->Release(); d3d = 0;

	if(rc != 0)
		LOG_ERROR( "d3d->Release(): ref count isn't 0: " << rc );
}


LPDIRECT3DDEVICE9 DeviceInit(HWND hwnd, bool windowed, unsigned int adapterIdx, unsigned int modeIdx)
{
	unsigned int width, height;

	if(windowed)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		width = rect.right-rect.left;
		height = rect.bottom-rect.top;
	}
	else
	{
		width = adapters[adapterIdx].mode[modeIdx].width;
		height = adapters[adapterIdx].mode[modeIdx].height;
	}

	
	HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
	if(hmon==0)	
	{
		CreateNULLRefDevice(hwnd);
		return device;
	}

	
	static HMONITOR curMonitor;
	static unsigned int curWidth, curHeight;
	static bool windowMode;
	if(device && hmon==curMonitor && width==curWidth && height==curHeight && windowed==windowMode)	
	{
		return device;
	}

	
	if(device)	
	{
		DeviceRelease();
	}

	for(unsigned int a=0; a<d3d->GetAdapterCount(); a++)
	{
		HMONITOR hm = d3d->GetAdapterMonitor(a);
		if(hm!=hmon)	continue;

		D3DPRESENT_PARAMETERS d3dpp; 
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		d3dpp.Windowed = windowed;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferCount = 1;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.Flags = 0;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		
		DWORD creationFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE; 
		d3d->CreateDevice( a, D3DDEVTYPE_HAL, hwnd, creationFlags, &d3dpp, &device);
		if(device)
		{
			D3DVIEWPORT9 vprt;
			vprt.X = 0;
			vprt.Y = 0;
			vprt.Width = width;
			vprt.Height = height;
			vprt.MinZ = 0.0f;
			vprt.MaxZ = 1.0f;
			device->SetViewport(&vprt);

			curMonitor = hmon;

			windowMode = windowed;			

			gapi::initialize(gapi::DX9);

			return device;
		}
	}

	ENFORCE(device);

	return device;
}

bool DeviceIsValid()
{
	if( device==0 )	return false;

	if( device->TestCooperativeLevel() != D3D_OK )
	{
		
		DeviceRelease();
		return false;
	}
	return true;
}

Viewport::Viewport()
{

}

int Viewport::width() const
{
	D3DVIEWPORT9 viewport;
	::dx9::device->GetViewport(&viewport);

	return viewport.Width;
}

int Viewport::height() const
{
	D3DVIEWPORT9 viewport;
	::dx9::device->GetViewport(&viewport);
	return viewport.Height;
}

void Viewport::set() const
{
	
}

int Viewport::deviceIndex() const
{
	return 0;
}

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