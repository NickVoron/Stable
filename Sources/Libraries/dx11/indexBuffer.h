// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "geometry/library.include.h"

#include "interfaces.h"


namespace dx11
{
	struct DeviceIndexBuffer : public DeviceResource<ID3D11Buffer, DeviceIndexBuffer>
	{
		template<class IndexType>
		void create(const IndexType* indices, int indicesCount)
		{
			release();

			D3D11_BUFFER_DESC bd;
			mem::memzero(bd);
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(IndexType) * indicesCount;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			size = bd.ByteWidth;

			D3D11_SUBRESOURCE_DATA InitData;
			mem::memzero(InitData);
			InitData.pSysMem = indices;
			HRESULT hr = device->device->CreateBuffer(&bd, &InitData, &ptr);

			format = dxgi::FormatFind<IndexType, 1>::format;
			count = indicesCount;
		}

		template<class IndexType>	void create(const Geometry::IndexStream<IndexType>& vs)	{	create(&vs[0], vs.getSize());	}
		template<class IndexType>	void upload(const Geometry::IndexStream<IndexType>& vs)	{	upload(&vs[0], vs.getSize());	}

		template<class IndexType>
		void upload(const IndexType* vertices, int indicesCount)
		{
			if(ptr && size >= sizeof(IndexType) * indicesCount )
			{
				D3D11_MAPPED_SUBRESOURCE data;
				device->context->Map(ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
				mem::memcpy(data.pData, vertices, sizeof(IndexType) * indicesCount);
				device->context->Unmap(ptr, 0);

				format = dxgi::FormatFind<IndexType, 1>::format;
				count = indicesCount;
			}
			else
			{
				create(vertices, indicesCount);
			}
		}

		void set(unsigned int offsetInIndices) const	{ device->context->IASetIndexBuffer(ptr, format, offsetInIndices); }
		void set() const								{ set(0); }

		unsigned int size = 0;
		unsigned int count = 0;
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
	};

	struct IndexBuffer : public DeviceIndexBuffer::BufferSet
	{
		unsigned int size() const { return data[0].size; }
		unsigned int count() const { return data[0].count; }
	};
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