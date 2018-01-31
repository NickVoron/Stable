// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once
#include "gapi/library.include.h"

#include "interfaces.h"

namespace dx11
{
	D3D11_COMPARISON_FUNC comparsion(gapi::ComparsionFunc func);
	D3D11_SAMPLER_DESC samplerDesc(const gapi::SamplerDesc& desc);

	struct DeviceSampler : public DeviceResource<ID3D11SamplerState, DeviceSampler>
	{
		void vertex(int slot) const;
		void pixel(int slot) const;
		void geometry(int slot) const;
		void hull(int slot) const;
		void domain(int slot) const;
		void compute(int slot) const;

		void create(const gapi::SamplerDesc& samplerDesc);
	};

	struct Sampler : public DeviceSampler::ShaderSet
	{
		void save(stream::ostream& os) const;
		void load(stream::istream& is);
		void create();

		gapi::SamplerDesc samplerDesc;
	};
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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