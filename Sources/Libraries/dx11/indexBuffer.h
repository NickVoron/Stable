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




