
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