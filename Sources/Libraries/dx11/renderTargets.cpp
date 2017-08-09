// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "renderTargets.h"

#include "common/delete.h"

namespace dx11
{




DeviceDefaultRenderTargets::DeviceDefaultRenderTargets()
{
	device = nullptr;
	target = nullptr;
	depthStencil = nullptr;

	mem::memzero(viewport);
}

void DeviceDefaultRenderTargets::set()
{
	device->context->OMSetRenderTargets(1, &target, depthStencil);
	device->context->RSSetViewports(1, &viewport);
}

void DeviceDefaultRenderTargets::clearColor(const D3DXVECTOR4& color)
{
	if (device->context)
	{
		device->context->ClearRenderTargetView(target, &color.x);
	}
}

void DeviceDefaultRenderTargets::clearDepthStencil(UINT flags, float zValue, UINT8 stencil)
{
	if (device->context)
	{
		device->context->ClearDepthStencilView(depthStencil, flags, zValue, stencil);
	}	
}

void DeviceDefaultRenderTargets::size(int& w, int&h) const
{
	w = viewport.Width;
	h = viewport.Height;
}

void DeviceDefaultRenderTargets::texelSize(float& w, float&h) const
{
	int iw, ih; size(iw, ih);
	w = 1.0f / iw;
	h = 1.0f / ih;
}




DeviceDefaultRenderTargets DefaultRenderTargets::data[Devices::MAX_COUNT];

void DefaultRenderTargets::create()							
{
	for (int i = 0; i < Devices::MAX_COUNT; ++i) 
	{ 
		data[i].device = &(*devices)[i]; 
	} 
}

void DefaultRenderTargets::init(int deviceIndex, ID3D11RenderTargetView* target, ID3D11DepthStencilView* depthStencil, int width, int height)
{
	data[deviceIndex].target = target;
	data[deviceIndex].depthStencil = depthStencil;

	data[deviceIndex].viewport.Width = width;
	data[deviceIndex].viewport.Height = height;
	data[deviceIndex].viewport.MinDepth = 0.0;
	data[deviceIndex].viewport.MaxDepth = 1.0f;
	data[deviceIndex].viewport.TopLeftX = 0;
	data[deviceIndex].viewport.TopLeftY = 0;
	
}

bool DefaultRenderTargets::empty()	{ return data[0].target == 0; }
bool DefaultRenderTargets::good()	{ return !empty(); }

void DefaultRenderTargets::execute(int deviceIndex)	{ data[deviceIndex].set(); }

void DefaultRenderTargets::set() { for (int i = 0; i < devices->count(); ++i) { data[i].set(); } }

void DefaultRenderTargets::clearColor(const D3DXVECTOR4& color)
{
	for (int i = 0; i < devices->count(); ++i)
	{
		data[i].clearColor(color);
	}
}

void DefaultRenderTargets::clearDepthStencil(UINT flags, float zValue, UINT8 stencil)
{
	for (int i = 0; i < devices->count(); ++i)
	{
		data[i].clearDepthStencil(flags, zValue, stencil);
	}
}




void DevicePlainTarget::create(DXGI_FORMAT format, int multisampleCount, int multisampleQuality)
{
	UINT numViewports = 1;
	device->context->RSGetViewports(&numViewports, &viewport);
	create(viewport.Width, viewport.Height, format, multisampleCount, multisampleQuality);
}

void DevicePlainTarget::create(int sX, int sY, DXGI_FORMAT fmt, int multisampleCount, int multisampleQuality)
{
	if (sX > 0 && sY > 0)
	{
		release();

		DevicePlainData::create(sX, sY, fmt, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, multisampleCount, multisampleQuality);

		ENFORCE(SUCCEEDED(device->device->CreateRenderTargetView(ptr, 0, &view)));


		viewport.Width = sX;
		viewport.Height = sY;
		viewport.MinDepth = 0.0;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
	}
}

void DevicePlainTarget::release()
{
	SAFE_RELEASE(ptr);
}

void DevicePlainTarget::set(bool setViewport, DeviceDepthStencil* depthStencil)
{
	set(setViewport, depthStencil ? depthStencil->view : 0);
}

void DevicePlainTarget::set(bool setViewport, DevicePlainTargetBackup* depthStencil)
{
	set(setViewport, depthStencil ? depthStencil->depthStencilView : 0);
}

void DevicePlainTarget::set(bool setViewport, ID3D11DepthStencilView* depthStencil)
{
	ENFORCE(good());

	device->context->OMSetRenderTargets(1, &view, depthStencil);

	if (setViewport)
	{
		device->context->RSSetViewports(1, &viewport);
	}
}

void DevicePlainTarget::clearColor(const D3DXVECTOR4& color)
{
	ENFORCE(good());

	device->context->ClearRenderTargetView(view, &color.x);
}

bool DevicePlainTarget::validateFormat(DXGI_FORMAT format)
{
	return true;
}

bool DevicePlainTarget::validateBindFlag(D3D11_BIND_FLAG bindFlag)
{
	return bindFlag == D3D11_BIND_RENDER_TARGET;	
}				 





bool IsDepthStencilFormat(DXGI_FORMAT fmt)
{
	switch(fmt)
	{
	case DXGI_FORMAT_D24_UNORM_S8_UINT: 
	case DXGI_FORMAT_D16_UNORM:          
	case DXGI_FORMAT_D32_FLOAT:        
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
		return true;
	}

	return false;       
}

bool DeviceDepthStencil::validateFormat(DXGI_FORMAT format)
{
	return IsDepthStencilFormat(format);
}

bool DeviceDepthStencil::validateBindFlag(D3D11_BIND_FLAG bindFlag)
{
	return bindFlag == D3D11_BIND_DEPTH_STENCIL;	
}


void DeviceDepthStencil::create(DXGI_FORMAT format, int multisampleCount, int multisampleQuality)
{
	D3D11_VIEWPORT viewport;

	UINT numViewports = 1;
	device->context->RSGetViewports(&numViewports, &viewport);
	create(viewport.Width, viewport.Height, format, multisampleCount, multisampleQuality);
}

void DeviceDepthStencil::create(int sX, int sY, DXGI_FORMAT fmt, int multisampleCount, int multisampleQuality)
{
	if (sX > 0 && sY > 0)
	{
		release();

		DevicePlainData::create(sX, sY, fmt, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, multisampleCount, multisampleQuality);

		D3D11_DEPTH_STENCIL_VIEW_DESC dssdesc;
		ZeroMemory(&dssdesc, sizeof(dssdesc));
		dssdesc.Flags = 0;
		dssdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dssdesc.ViewDimension = multisampleCount <= 1 ? D3D11_DSV_DIMENSION_TEXTURE2D : D3D11_DSV_DIMENSION_TEXTURE2DMS;
		dssdesc.Texture2D.MipSlice = 0;
		ENFORCE(SUCCEEDED(device->device->CreateDepthStencilView(ptr, &dssdesc, &view)));
	}
}

void DeviceDepthStencil::release()
{
	SAFE_RELEASE(ptr);
}

void DeviceDepthStencil::clearDepthStencil(UINT flags, float zValue, UINT8 stencil)
{
	if (view)
	{
		device->context->ClearDepthStencilView(view, flags, zValue, stencil);
	}
}

void DeviceDepthStencil::set()
{
	ENFORCE(good());

	if (view)
	{
		device->context->OMSetRenderTargets(0, 0, view);
	}
}

void DeviceDepthStencil::set(DevicePlainTargetBackup& renderTarget)
{
	ENFORCE(good());
	
	if (view)
	{		
		device->context->OMSetRenderTargets(renderTarget.count, renderTarget.targets, view);
	}	
}

void DeviceDepthStencil::zero()
{
	device->context->OMSetRenderTargets(0, 0, 0);
}




void DevicePlainTargetBackup::backup(unsigned int cnt)
{
	count = cnt;
										  
	UINT numViewports = 1;
	device->context->RSGetViewports(&numViewports, &viewport);
	device->context->OMGetRenderTargets(count, targets, &depthStencilView);
}

void DevicePlainTargetBackup::restore(bool setViewport)
{
	device->context->OMSetRenderTargets(count, targets, depthStencilView);

	if (setViewport)
	{
		device->context->RSSetViewports(1, &viewport);
	}
}

void DevicePlainTargetBackup::clearColor(const D3DXVECTOR4& color)
{
	device->context->ClearRenderTargetView(targets[0], &color.x);
}

void DevicePlainTargetBackup::clearDepthStencil(UINT flags, float zValue, UINT8 stencil)
{
	device->context->ClearDepthStencilView(depthStencilView, flags, zValue, stencil);
}

void DevicePlainTargetBackup::size(int& w, int&h) const
{
	w = viewport.Width;
	h = viewport.Height;
}

DXGI_FORMAT DevicePlainTargetBackup::format() const
{
	D3D11_RENDER_TARGET_VIEW_DESC desc;
	ptr->GetDesc(&desc);

	return desc.Format;
}

void DevicePlainTargetBackup::texelSize(float& w, float&h) const
{
	int sizeX, sizeY;
	size(sizeX, sizeY);

	w = 1.0f / sizeX;
	h = 1.0f / sizeY;
}





void PlainTargetBackup::backup(unsigned int count)
{
	for (int i = 0; i < devices->count(); ++i)	{ data[i].backup(count); }
}

void PlainTargetBackup::restore(bool setViewport)
{
	for (int i = 0; i < devices->count(); ++i)	{ data[i].restore(setViewport); }
}

}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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