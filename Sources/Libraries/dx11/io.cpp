// Copyright (C) 2013-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "io.h"
#include "formatInfo.h"
#include "shaderResource.h"
#include "device.h"

#include "common/delete.h"

namespace dx11
{
	unsigned int calculateTextureMemSize(const D3D11_TEXTURE2D_DESC& desc)
	{
		unsigned int bpp = dx11::bpp(desc.Format);
		unsigned int size = 0;

		for (unsigned int i = 0; i < desc.MipLevels; ++i)
		{
			int levelW = std::max((int)1, (int)(desc.Width >> i));
			int levelH = std::max((int)1, (int)(desc.Height >> i));

			size += levelW * levelH * bpp / 8;
		}

		return size * desc.ArraySize;
	}

	unsigned int calculateTextureMemSize(ID3D11Texture2D* texture)
	{
		ENFORCE(texture);

		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		return calculateTextureMemSize(desc);
	}

	void saveTexture(Device* device, ID3D11Texture2D* texture, stream::ostream& os)
	{
		ENFORCE(texture);

		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);

		os << desc;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		ID3D11Texture2D* staging = 0;
		device->device->CreateTexture2D(&desc, 0, &staging);
		device->context->CopyResource(staging, texture);

		unsigned int size = 0;
		unsigned int subidx = 0;
		unsigned int subcount = desc.ArraySize * desc.MipLevels;

		os << subcount;
		for (unsigned int a = 0; a < desc.ArraySize; ++a)
		{
			for (unsigned int i = 0; i < desc.MipLevels; ++i)
			{
				int levelW = std::max((int) 1, (int) (desc.Width >> i));
				int levelH = std::max((int) 1, (int) (desc.Height >> i));

				D3D11_MAPPED_SUBRESOURCE subresource;
				device->context->Map(staging, subidx++, D3D11_MAP_READ, 0, &subresource);
				os << subresource.RowPitch << levelH;
				os.write(subresource.pData, subresource.RowPitch * levelH);

				device->context->Unmap(staging, subidx);

				size += subresource.RowPitch * levelH;
			}
		}



		SAFE_RELEASE(staging);	
	}

	void loadTexture(ID3D11Device* device, ID3D11Texture2D*& texture, stream::istream& is)
	{
		SAFE_RELEASE(texture);

		D3D11_TEXTURE2D_DESC desc;
		unsigned int subcount;
		is >> desc >> subcount;

		std::vector<D3D11_SUBRESOURCE_DATA> subresource(subcount);
		for (unsigned int i = 0; i < subcount; ++i)
		{
			unsigned int rowPitch;
			unsigned int levelH;
			is >> rowPitch >> levelH;

			subresource[i].pSysMem = malloc(rowPitch*levelH);
			subresource[i].SysMemPitch = rowPitch;
			subresource[i].SysMemSlicePitch = 0;

			is.read((void*) subresource[i].pSysMem, rowPitch*levelH);
		}

		device->CreateTexture2D(&desc, &subresource[0], &texture);


		for (unsigned int i = 0; i < subcount; ++i)
		{
			free((void*) subresource[i].pSysMem);
		}
	}

}



// Copyright (C) 2013-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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