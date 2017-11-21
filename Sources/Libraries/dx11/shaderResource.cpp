#include "shaderResource.h"
#include "device.h"
#include "io.h"
#include "renderTargets.h"

		   
namespace dx11
{  
	//
	//
	//
	/*
	ID3D11ShaderResourceView* createShaderRes2D(ID3D11Device* device, ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		ID3D11ShaderResourceView* srv = 0;	  
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
				
		device->CreateShaderResourceView(texture, &srvDesc, &srv);		

		return srv;
	}	

	ID3D11ShaderResourceView* createShaderResCUBE(ID3D11Device* device, ID3D11Texture2D* texture)
	{
		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		ID3D11ShaderResourceView* srv = 0;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		
		device->CreateShaderResourceView(texture, &srvDesc, &srv);

		return srv;
	}
	*/

	//
	//
	//
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


	//
	//
	//
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

	//
	//
	//
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

	//
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

	//
	//
	//

	//
	//
	//
	void resetShaderResourceVS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->VSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourcePS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->PSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceGS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->GSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceHS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->HSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceDS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->DSSetShaderResources(slot, 1, &empty); }
	void resetShaderResourceCS(int slot) { ID3D11ShaderResourceView* empty = 0; for (int i = 0; i < devices->count(); ++i)	(*devices)[i].context->CSSetShaderResources(slot, 1, &empty); }
}

