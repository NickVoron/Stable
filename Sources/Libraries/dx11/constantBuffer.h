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