// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "shaderResource.h"
#include "device.h"
#include "io.h"
#include "renderTargets.h"

		   
namespace dx11
{  
	
	
	
	

	
	
	
 	void DeviceTexture::save(stream::ostream& os) const
 	{
		dx11::saveTexture(device, ptr, os);
 	}
 
 	void DeviceTexture::load(stream::istream& is)
 	{
 		dx11::loadTexture(device->device, ptr, is);
 	}

	void DeviceTexture::size(int& width, int& height) const
	{
		D3D11_TEXTURE2D_DESC desc;
		ptr->GetDesc(&desc);
		width = desc.Width;
		height = desc.Height;
 	}


	
	
	
	void Texture::save(stream::ostream& os) const 
	{
		for (int i = 0; i < devices->count(); ++i)
		{
			data[i].save(os);
		}
	}

	void Texture::load(stream::istream& is) 
	{
 		for (int i = 0; i < devices->count(); ++i)
 		{	
			data[i].load(is);
 		}		
	}

	void Texture::size(int& width, int& height) const
	{
		data[0].size(width, height);
	}

	
	
	
	void DeviceShaderResource::create(DeviceTexture& texture, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		HRESULT hr = device->device->CreateShaderResourceView(texture.ptr, &desc, &ptr);
		if (!SUCCEEDED(hr))
		{
			LOG_ERROR(__FUNCTION__ << "Can't create hader resource, error code:" << hr);
		}
	}

	void DeviceShaderResource::create(DevicePlainTarget& target, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		device->device->CreateShaderResourceView(target.ptr, &desc, &ptr);
	}

	void DeviceShaderResource::create(DeviceDepthStencil& depthStencil, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc)
	{
		device->device->CreateShaderResourceView(depthStencil.ptr, &desc, &ptr);
	}

	
	void ShaderResource::create(Texture& texture)
	{
		for (int i = 0; i < devices->count(); ++i)
		{
			data[i].create(texture.data[i], desc);
		}
	}

	void ShaderResource::create(PlainTarget& target)
	{
		desc.Format = target.format();
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;

		for (int i = 0; i < devices->count(); ++i)
		{
			data[i].create(target.data[i], desc);
		}
	}

	void ShaderResource::create(DepthStencil& depthStencil)
	{
		desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;

		for (int i = 0; i < devices->count(); ++i)
		{
			data[i].create(depthStencil.data[i], desc);
		}
	}

	void ShaderResource::save(stream::ostream& os) const
	{
		os << desc;
	}

	void ShaderResource::load(stream::istream& is)
	{
		is >> desc;
	}

	
	
	

	
	
	
	void resetShaderResourceVS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->VSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourcePS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->PSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceGS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->GSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceHS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->HSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceDS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->DSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceCS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->CSSetShaderResources(slot, 1, &empty); }
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