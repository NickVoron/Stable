// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "defaultLogs/library.include.h"
#include "imageD3D/library.include.h"
#include "strings/library.include.h"

#include "interfaces.h"


namespace dx11
{
	struct CubemapDesc
	{
		str::string256 planes[image::FACE_PLANES_COUNT];
	};

	struct DeviceTexture : public DeviceResource<ID3D11Texture2D, DeviceTexture>
	{
		template<template<class> class ImageType, image::Format fmt>
		int createWithMips(const ImageType< image::Color<fmt> >& plane, int mipsCount)
		{
			return image::dx11::createTextureWithMip(device->device, plane, &ptr, mipsCount);
		}

		template<template<class> class ImageType, image::Format fmt>
		void create(const ImageType< image::Color<fmt> >& plane)
		{
			image::dx11::createTexture(device->device, plane, &ptr);
		}

		void size(int& width, int& height) const;

		void save(stream::ostream& os) const;
		void load(stream::istream& is);
	};

	struct Texture : public DeviceTexture::DeviceData
	{
		void save(stream::ostream& os) const;
		void load(stream::istream& is);

		void size(int& width, int& height) const;

		template<template<class> class ImageType, image::Format fmt>
		void create(const ImageType< image::Color<fmt> >& plane)
		{
			for (int i = 0; i < devices->count(); ++i)
			{
				data[i].create(plane);
			}
		}

		template<image::Format fmt>
		void create(const char* fileName)
		{
			image::Plane< image::Color<fmt> > pln;
			image::freeimage::load(fileName, pln);
			create(pln);
		}

		template<template<class> class ImageType, image::Format fmt>
		int createWithMips(const ImageType< image::Color<fmt> >& plane, int mipsCount)
		{
			int countMips = 1;
			for (int i = 0; i < devices->count(); ++i)
			{
				countMips = data[i].createWithMips(plane, mipsCount);
			}

			return countMips;
		}

		template<image::Format fmt>
		int createWithMips(const char* fileName, int mipsCount)
		{
			image::Plane< image::Color<fmt> > pln;
			image::freeimage::load(fileName, pln);
			return createWithMips(pln, mipsCount);
		}

		template<image::Format fmt>
		void create(const CubemapDesc& cubeDesc)
		{
			image::Cubemap< image::Color<fmt> > cubemap;
			for (int i = image::POSITIVE_X; i < image::FACE_PLANES_COUNT; ++i)
			{
				image::Plane< image::Color<fmt> > plane;
				image::freeimage::load(cubeDesc.planes[i].c_str(), plane);
				ENFORCE(plane.sizeX == plane.sizeY);
				if (cubemap.size == 0)
				{
					cubemap.allocate(plane.sizeX);
				}
				ENFORCE(cubemap.size == plane.sizeX);
				cubemap.copy((image::CUBE_FACE)i, plane);
			}

			create(cubemap);
		}
	};

	struct DevicePlainTarget;
	struct DeviceDepthStencil;
	struct DeviceShaderResource : public DeviceResource<ID3D11ShaderResourceView, DeviceShaderResource>
	{
		void vertex(int slot) const { device->context->VSSetShaderResources(slot, 1, &ptr); }
		void pixel(int slot) const { device->context->PSSetShaderResources(slot, 1, &ptr); }
		void geometry(int slot) const { device->context->GSSetShaderResources(slot, 1, &ptr); }
		void hull(int slot) const { device->context->HSSetShaderResources(slot, 1, &ptr); }
		void domain(int slot) const { device->context->DSSetShaderResources(slot, 1, &ptr); }
		void compute(int slot) const { device->context->CSSetShaderResources(slot, 1, &ptr); }

		void create(DeviceTexture& texture, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);
		void create(DevicePlainTarget& target, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);
		void create(DeviceDepthStencil& depthStencil, const D3D11_SHADER_RESOURCE_VIEW_DESC& desc);
	};

	struct PlainTarget;
	struct DepthStencil;
	struct ShaderResource : public DeviceShaderResource::ShaderSet
	{			   
		void create(Texture& texture);
		void create(PlainTarget& target);
		void create(DepthStencil& depthStencil);

		void save(stream::ostream& os) const;
		void load(stream::istream& is);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	};

	void resetShaderResourceVS(int slot);
	void resetShaderResourcePS(int slot);
	void resetShaderResourceGS(int slot);
	void resetShaderResourceHS(int slot);
	void resetShaderResourceDS(int slot);
	void resetShaderResourceCS(int slot);
}



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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