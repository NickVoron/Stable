#pragma once

#include "geometry/library.include.h"

#include "interfaces.h"


namespace dx11
{
	struct DeviceVertexBuffer : public DeviceResource<ID3D11Buffer, DeviceVertexBuffer>
	{
		template<class VertexType>
		void create(const VertexType* vertices, int verticesCount)
		{
			release();

			D3D11_BUFFER_DESC bd;
			mem::memzero(bd);
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VertexType) * verticesCount;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			size = bd.ByteWidth;

			D3D11_SUBRESOURCE_DATA InitData;
			mem::memzero(InitData);
			InitData.pSysMem = vertices;
			HRESULT hr = device->device->CreateBuffer(&bd, &InitData, &ptr);

			stride = sizeof(VertexType);
			count = verticesCount;
		}

		template<class VertexType>
		void upload(const VertexType* vertices, int verticesCount)
		{
			if(ptr && size >= sizeof(VertexType) * verticesCount )
			{
				D3D11_MAPPED_SUBRESOURCE data;
				device->context->Map(ptr, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
				mem::memcpy(data.pData, vertices, sizeof(VertexType) * verticesCount);
				device->context->Unmap(ptr, 0);

				stride = sizeof(VertexType);
				count = verticesCount;
			}
			else
			{
				create(vertices, verticesCount);
			}			
		}

		template<class VertexType> void create(const Geometry::VertexStream<VertexType>& vs) { create(&vs[0], vs.getSize()); }
		template<class VertexType> void upload(const Geometry::VertexStream<VertexType>& vs) { upload(&vs[0], vs.getSize()); }


		void set() const
		{
			device->context->IASetVertexBuffers(slot, 1, &ptr, &stride, &offset);
		}

		int slot = 0;
		unsigned int offset = 0;

		unsigned int size = 0;
		unsigned int stride = 0;
		unsigned int count = 0;
	};

	struct VertexBuffer : public DeviceVertexBuffer::BufferSet
	{
		void setup(int streamSlot, unsigned int offsetInVertices)
		{
			for (int i = 0; i < devices->count(); ++i)
			{
				data[i].slot = streamSlot;
				data[i].offset = offsetInVertices;
			}			
		}

		void setup(int streamSlot) { setup(streamSlot, 0); }

		unsigned int size() const { return data[0].size; }
		unsigned int count() const { return data[0].count; }
	};

}




