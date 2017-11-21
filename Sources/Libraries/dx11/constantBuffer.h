#pragma once

#include "geometry/library.include.h"

#include "interfaces.h"

namespace dx11
{
	struct DeviceConstantBuffer : public DeviceResource<ID3D11Buffer, DeviceConstantBuffer>
	{
		template<class DataSet>
		void create(const DataSet& data)
		{
			release();

			D3D11_BUFFER_DESC bd;
			mem::memzero(bd);
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(DataSet);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			mem::memzero(InitData);
			InitData.pSysMem = &data;
			device->device->CreateBuffer(&bd, &InitData, &ptr);
		}

		template<class DataSet>
		void upload(const DataSet& data)
		{
			if(ptr)
			{
				device->context->UpdateSubresource( ptr, 0, NULL, &data, 0, 0 );	
			}
			else
			{
				create(data);
			}
		}

		void vertex(int slot) const		{ device->context->VSSetConstantBuffers(slot, 1, &ptr); }
		void pixel(int slot) const		{ device->context->PSSetConstantBuffers(slot, 1, &ptr); }
		void geometry(int slot) const	{ device->context->GSSetConstantBuffers(slot, 1, &ptr); }
		void hull(int slot) const		{ device->context->HSSetConstantBuffers(slot, 1, &ptr); }
		void domain(int slot) const		{ device->context->DSSetConstantBuffers(slot, 1, &ptr); }
		void compute(int slot) const	{ device->context->CSSetConstantBuffers(slot, 1, &ptr); }

	};

	struct ConstantBuffer : public DeviceConstantBuffer::ShaderSet { };

}




