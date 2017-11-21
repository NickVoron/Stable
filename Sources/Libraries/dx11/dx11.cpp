#include "dx11.h"

#include "common/clearPtrContainer.h"

#include <vector>

namespace dx11
{
	void DeviceCreate()
	{
		gapi::initialize(gapi::DX11);
		devices = std::make_unique<Devices>(Devices::FIRST_ADAPTER);
		DefaultRenderTargets::create();
	}
/*
	ID3D11RenderTargetView* CreateRenderTargetView(DXGI_FORMAT format, UINT width, UINT height, bool isShaderResource)
	{
		ID3D11RenderTargetView* result = 0;
		ID3D11Texture2D* buffer = 0;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof(descDepth) );
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = format;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_RENDER_TARGET;
		descDepth.BindFlags |= isShaderResource ? D3D11_BIND_SHADER_RESOURCE : 0;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		ENFORCE( SUCCEEDED( device.device->CreateTexture2D( &descDepth, NULL, &buffer ) ) );
		ENFORCE( SUCCEEDED( device.device->CreateRenderTargetView(buffer, NULL, &result) ));
		buffer->Release();
		return result;
	}
*/

// 	void SetRenderTarget(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView)
// 	{
// 		device.context->OMSetRenderTargets(1, &renderTargetView, depthStencilView );
// 	}																		 
// 
// 
// 	void SetViewport(float x, float y, float w, float h, float minZ, float maxZ)
// 	{
// 		// Set up the viewport.
// 		D3D11_VIEWPORT vp;
// 		vp.Width = w;
// 		vp.Height = h;
// 		vp.MinDepth = 0.0f;
// 		vp.MaxDepth = 1.0f;
// 		vp.TopLeftX = x;
// 		vp.TopLeftY = y;
// 		device.context->RSSetViewports(1, &vp);
// 	}

	void DeviceRelease()
	{
		devices = nullptr;
		//device->release();	
	}
}