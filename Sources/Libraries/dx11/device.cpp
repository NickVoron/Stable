// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "device.h"
#include "common/clearPtrContainer.h"
#include "common/delete.h"
#include "defaultLogs/library.include.h"
#include "stuff/library.include.h"

#include "renderTargets.h"

#include <string>

namespace dx11
{
	std::unique_ptr<Devices> devices;

	std::wstring DeviceDescription(int deviceIndex)
	{
		return devices ? (*devices)[deviceIndex].adapterDesc : L"";
	}

	void GetWindowSize(HWND hwnd, unsigned int& width, unsigned int& height)
	{
		RECT rc;
		GetClientRect( hwnd, &rc );
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
	}

	
	Adapter::Adapter() : adapter(0)	
	{
	}

	Adapter::~Adapter()
	{
		release();
	}

	void Adapter::release()
	{
		SAFE_RELEASE(adapter);
	}

	
	
	
	Adapters::Adapters() : count(0)
	{
		IDXGIFactory1* factory = 0; 
		if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1) ,(void**)&factory)))
		{
			return;
		}

		IDXGIAdapter1* adapter = 0; 
		for (UINT i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			++count;
			ENFORCE(count < MAX_COUNT);
			adapters[i].adapter = adapter;
		} 

		SAFE_RELEASE(factory);
	}
	

	
	
	
	Device::Device() : device(0), context(0)
	{

	}

	Device::~Device()
	{
		release();
	}

	void Device::create(Adapter& adapter)
	{
		D3D_FEATURE_LEVEL flOut;
		const D3D_FEATURE_LEVEL flvl[] = { D3D_FEATURE_LEVEL_11_0 };
		
		ENFORCE(SUCCEEDED(D3D11CreateDevice(adapter.adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, flvl, sizeof(flvl) / sizeof(*flvl), D3D11_SDK_VERSION, &device, &flOut, &context))); 
		
		
		
		

		DXGI_ADAPTER_DESC1 desc;
		adapter.adapter->GetDesc1(&desc);
		adapterDesc = desc.Description;
	}

	void Device::release()
	{
		SAFE_RELEASE(context);
		SAFE_RELEASE(device);
	}

	
	
	
	Devices::Devices(CreationMode mode)
	{
		if(mode == FOR_EACH_ADAPTER)
		{
			Adapters adapters;
			create(adapters);
		}
		else if (mode == FIRST_ADAPTER)
		{
			cnt = 1;
			Adapters adapters;
			devices_[0].create(adapters[0]);
		}
	}
	
	void Devices::create(Adapters& adapters)
	{
		cnt = adapters.count;

		for(int i = 0; i < cnt; ++i)
		{
			devices_[i].create(adapters[i]);
		}
	}

	
	
	

	ID3D11Texture2D* CreateDepthStencilTexture(ID3D11Device* dev, DXGI_FORMAT format, UINT width, UINT height, bool isShaderResource)
	{
		ENFORCE(dev);

		ID3D11Texture2D* result = 0;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = format;
		descDepth.SampleDesc.Count = 1;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.BindFlags |= isShaderResource ? D3D11_BIND_SHADER_RESOURCE : 0;
		ENFORCE( SUCCEEDED( dev->CreateTexture2D( &descDepth, NULL, &result ) ) );		

		return result;
	}

	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* dev, DXGI_FORMAT format, UINT width, UINT height, bool isShaderResource)
	{
		ENFORCE(dev);

		ID3D11DepthStencilView* result = 0;
		ID3D11Texture2D* buffer = CreateDepthStencilTexture(dev, format, width, height, isShaderResource);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(descDSV) );
		descDSV.Format = format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		ENFORCE( SUCCEEDED( dev->CreateDepthStencilView( buffer, &descDSV, &result) ) );
		buffer->Release();

		return result;
	}

	
	
	
	OutputSurfaceBase::OutputSurfaceBase() : renderTargetView(0), depthStencilView(0), parent(0), width(0), height(0)
	{

	}

	OutputSurfaceBase::~OutputSurfaceBase()
	{

	}

	void OutputSurfaceBase::set()
	{
		

		if (parent)
		{
			parent->context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

			D3D11_VIEWPORT vp;
			vp.Width = (float) width;
			vp.Height = (float) height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			parent->context->RSSetViewports(1, &vp);
		}
	}

	void OutputSurfaceBase::clearColor(const Vector4& color)
	{
		

		if (parent)
		{
			parent->context->ClearRenderTargetView(renderTargetView, &color.x);
		}		
	}

	void OutputSurfaceBase::clearDepthStencil(UINT flags, float zValue, UINT8 stencil)
	{
		

		if (parent && depthStencilView)
		{
			parent->context->ClearDepthStencilView(depthStencilView, flags, zValue, stencil);
		}
	}

	void OutputSurfaceBase::release()
	{
		SAFE_RELEASE(depthStencilView);
		SAFE_RELEASE(renderTargetView);

		parent = 0;
		width = 0;
		height = 0;
	}

	bool OutputSurfaceBase::good() const
	{
		return depthStencilView && renderTargetView && parent;
	}

	
	
	
	OutputSurfaceClone::OutputSurfaceClone() :buffer(0), shaderResViewRT(0)
	{

	}

	void OutputSurfaceClone::release()
	{
		SAFE_RELEASE(shaderResViewRT);
	}

	void OutputSurfaceClone::init(OutputSurfaceBase& source)
	{
		if (width > 0 && height > 0)
		{
			release();

			parent = source.parent;
			width = source.width;
			height = source.height;

			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				source.depthStencilView->GetDesc(&desc);

				ID3D11Texture2D* result = 0;
				D3D11_TEXTURE2D_DESC descDepth;
				ZeroMemory(&descDepth, sizeof(descDepth));
				descDepth.Width = width;
				descDepth.Height = height;
				descDepth.MipLevels = 1;
				descDepth.ArraySize = 1;
				descDepth.Format = desc.Format;
				descDepth.SampleDesc.Count = 1;
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				ENFORCE(SUCCEEDED(parent->device->CreateTexture2D(&descDepth, NULL, &result)));
				ENFORCE(SUCCEEDED(parent->device->CreateDepthStencilView(result, 0, &depthStencilView)));
			}


			{
				D3D11_RENDER_TARGET_VIEW_DESC desc;
				source.renderTargetView->GetDesc(&desc);

				D3D11_TEXTURE2D_DESC texDesc;
				ZeroMemory(&texDesc, sizeof(texDesc));
				texDesc.Width = width;
				texDesc.Height = height;
				texDesc.MipLevels = 1;
				texDesc.ArraySize = 1;
				texDesc.Format = desc.Format;
				texDesc.SampleDesc.Count = 1;
				texDesc.Usage = D3D11_USAGE_DEFAULT;
				texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				ENFORCE(SUCCEEDED(parent->device->CreateTexture2D(&texDesc, 0, &buffer)));
				ENFORCE(SUCCEEDED(parent->device->CreateRenderTargetView(buffer, 0, &renderTargetView)));
			}		

			parent->device->CreateShaderResourceView(buffer, 0, &shaderResViewRT);
		}
	}

	
	
	
	OutputSurface::OutputSurface() : swapChain(0)
	{	

	}

	OutputSurface::~OutputSurface()
	{

	}

	void OutputSurface::init(int devIndex, HWND hwnd, bool hasDepthStencil, bool fullscreen)
	{
		release();

		deviceIndex = devIndex;
		parent = &(*devices)[deviceIndex];

		GetWindowSize(hwnd, width, height);

		IDXGIDevice* pDXGIDevice;
		IDXGIAdapter* pDXGIAdapter;
		IDXGIFactory* pIDXGIFactory;

		ENFORCE( SUCCEEDED( parent->device->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice) ) );
		ENFORCE( SUCCEEDED( pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter) ) );
		ENFORCE( SUCCEEDED( pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory) ) );

		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferCount = 1;                                    
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      
		scd.OutputWindow = hwnd;                                
		scd.SampleDesc.Count = 1;                               
		scd.Windowed = !fullscreen;                             

		ID3D11Texture2D* pBuffer = 0;
		ENFORCE( SUCCEEDED( pIDXGIFactory->CreateSwapChain(parent->device, &scd, &swapChain) ) );				
		ENFORCE( SUCCEEDED( swapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**) &pBuffer ) ));
		ENFORCE( SUCCEEDED( parent->device->CreateRenderTargetView(pBuffer, NULL, &renderTargetView) ));
		pBuffer->Release();

		if(width > 0 && height > 0)
		if(hasDepthStencil)
		{
			depthStencilView = CreateDepthStencilView(parent->device, DXGI_FORMAT_D24_UNORM_S8_UINT, width, height, false);

			ID3D11DepthStencilState* depthStencilState = 0;

			D3D11_DEPTH_STENCIL_DESC dsd;
			dsd.DepthEnable = TRUE;
			dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
			dsd.StencilEnable = FALSE;
			dsd.StencilReadMask = 0;
			dsd.StencilWriteMask = 0;
			dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsd.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
			dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
			dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsd.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			parent->device->CreateDepthStencilState(&dsd, &depthStencilState);
			parent->context->OMSetDepthStencilState(depthStencilState, 0);

			depthStencilState->Release();
		}

		DefaultRenderTargets::init(devIndex, renderTargetView, depthStencilView, width, height);
	}

	void OutputSurface::release()
	{
		OutputSurfaceBase::release();
		SAFE_RELEASE(swapChain);
	}

	void OutputSurface::present()
	{
		swapChain->Present(0, 0);
	}

	
	
	


	
	
	
	Viewport::Viewport() : surface(0)
	{
	
	}

	void Viewport::init(const std::wstring& n, OutputSurface& outputSurface)
	{
		surface = &outputSurface;
		Viewports::Viewport::init(n, width(), height());
	}

	int Viewport::width() const
	{
		return surface ? surface->width : 0;
	}

	int Viewport::height() const
	{
		return surface ? surface->height : 0;
	}

	void Viewport::set() const
	{
		if(surface)
		{
			surface->set();
		}
	}

	int Viewport::deviceIndex() const
	{
		return surface ? surface->deviceIndex : 0;
	}

	void Viewport::setPrimaryOutputSurface()
	{
		surface->set();
	}

	void Viewport::setSecondaryOutputSurface()
	{
		if (!secondary.good() || secondary.width != surface->width || secondary.height != surface->height)
		{
			ENFORCE(surface);
			secondary.init(*surface);
		}
		
		secondary.set();
	}

	void Viewport::clearSecondary()
	{
		if (!secondary.good())
		{
			ENFORCE(surface);
			secondary.init(*surface);
		}

		secondary.clearColor(Vector4(0, 0, 0, 0));
		secondary.clearDepthStencil(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	ID3D11DeviceContext* Viewport::context()
	{
		return surface->parent->context;
	}

	ID3D11Device* Viewport::device()
	{
		return surface->parent->device;
	}

	ID3D11ShaderResourceView* Viewport::renderTarget()
	{
		return secondary.shaderResViewRT;
	}
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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