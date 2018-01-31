// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "samplers.h"
#include "device.h"
		   
namespace dx11
{  
	D3D11_TEXTURE_ADDRESS_MODE addressMode(gapi::TextureAddressMode mode)
	{
		switch (mode)
		{
		case gapi::WRAP:		return D3D11_TEXTURE_ADDRESS_WRAP;
		case gapi::MIRROR:		return D3D11_TEXTURE_ADDRESS_MIRROR;
		case gapi::CLAMP:		return D3D11_TEXTURE_ADDRESS_CLAMP;
		case gapi::BORDER:		return D3D11_TEXTURE_ADDRESS_BORDER;
		case gapi::MIRROR_ONCE:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		}

		return D3D11_TEXTURE_ADDRESS_WRAP;
	}
		
	D3D11_COMPARISON_FUNC comparsion(gapi::ComparsionFunc func)
	{
		switch (func)
		{
		case gapi::COMPARISON_NEVER:		   return D3D11_COMPARISON_NEVER;
		case gapi::COMPARISON_LESS:			   return D3D11_COMPARISON_LESS;
		case gapi::COMPARISON_EQUAL:		   return D3D11_COMPARISON_EQUAL;
		case gapi::COMPARISON_LESS_EQUAL:	   return D3D11_COMPARISON_LESS_EQUAL;
		case gapi::COMPARISON_GREATER:		   return D3D11_COMPARISON_GREATER;
		case gapi::COMPARISON_NOT_EQUAL:	   return D3D11_COMPARISON_NOT_EQUAL;
		case gapi::COMPARISON_GREATER_EQUAL:   return D3D11_COMPARISON_GREATER_EQUAL;
		case gapi::COMPARISON_ALWAYS:		   return D3D11_COMPARISON_ALWAYS;
		}

		return D3D11_COMPARISON_NEVER;
	}

	D3D11_FILTER filter(gapi::TextureFilter f)
	{ 
		switch (f)
		{
		case gapi::MIN_MAG_MIP_POINT:							   return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case gapi::MIN_MAG_POINT_MIP_LINEAR:					   return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case gapi::MIN_POINT_MAG_LINEAR_MIP_POINT:				   return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		case gapi::MIN_POINT_MAG_MIP_LINEAR:					   return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		case gapi::MIN_LINEAR_MAG_MIP_POINT:					   return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		case gapi::MIN_LINEAR_MAG_POINT_MIP_LINEAR:				   return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		case gapi::MIN_MAG_LINEAR_MIP_POINT:					   return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case gapi::MIN_MAG_MIP_LINEAR:							   return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case gapi::ANISOTROPIC:									   return D3D11_FILTER_ANISOTROPIC;
		case gapi::COMPARISON_MIN_MAG_MIP_POINT:				   return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		case gapi::COMPARISON_MIN_MAG_POINT_MIP_LINEAR:			   return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
		case gapi::COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT:	   return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
		case gapi::COMPARISON_MIN_POINT_MAG_MIP_LINEAR:			   return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
		case gapi::COMPARISON_MIN_LINEAR_MAG_MIP_POINT:			   return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
		case gapi::COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR:	   return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		case gapi::COMPARISON_MIN_MAG_LINEAR_MIP_POINT:			   return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		case gapi::COMPARISON_MIN_MAG_MIP_LINEAR:				   return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		case gapi::COMPARISON_ANISOTROPIC:						   return D3D11_FILTER_COMPARISON_ANISOTROPIC;
		}

		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}

	D3D11_SAMPLER_DESC samplerDesc(const gapi::SamplerDesc& desc)
	{
		D3D11_SAMPLER_DESC result;

		result.Filter = filter(desc.filter);
		result.AddressU = addressMode(desc.addressU);
		result.AddressV = addressMode(desc.addressV);
		result.AddressW = addressMode(desc.addressW);
		result.ComparisonFunc = comparsion(desc.comparisonFunc);

		result.MipLODBias = desc.mipLODBias;
		result.MaxAnisotropy = desc.maxAnisotropy;
		result.BorderColor[0] = desc.BorderColor[0];
		result.BorderColor[1] = desc.BorderColor[1];
		result.BorderColor[2] = desc.BorderColor[2];
		result.BorderColor[3] = desc.BorderColor[3];
		result.MinLOD = desc.minLod;
		result.MaxLOD = desc.maxLod;

		return result;
	}



		void DeviceSampler::vertex(int slot) const		{ device->context->VSSetSamplers(slot, 1, &ptr); }
		void DeviceSampler::pixel(int slot) const		{ device->context->PSSetSamplers(slot, 1, &ptr); }
		void DeviceSampler::geometry(int slot) const	{ device->context->GSSetSamplers(slot, 1, &ptr); }
		void DeviceSampler::hull(int slot) const		{ device->context->HSSetSamplers(slot, 1, &ptr); }
		void DeviceSampler::domain(int slot) const		{ device->context->DSSetSamplers(slot, 1, &ptr); }
		void DeviceSampler::compute(int slot) const	{ device->context->CSSetSamplers(slot, 1, &ptr); }

		void DeviceSampler::create(const gapi::SamplerDesc& dsc)
		{
			D3D11_SAMPLER_DESC desc = samplerDesc(dsc);
			device->device->CreateSamplerState(&desc, &ptr);
		}

		
		
		
		void Sampler::save(stream::ostream& os) const
		{
			os << samplerDesc;

		}

		void Sampler::load(stream::istream& is)
		{
			is >> samplerDesc;
			create();
		}

		void Sampler::create()
		{
			DeviceSampler::ShaderSet::create(samplerDesc);
		}
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