#include "blendState.h"
#include "device.h"

namespace dx11
{
	inline void renderTargetBlend(D3D11_RENDER_TARGET_BLEND_DESC& dst, const gapi::RenderTargetBlend& src) 
	{
		dst.BlendEnable				= src.blendEnable;
		dst.BlendOp					= (D3D11_BLEND_OP)src.blendOp;
		dst.BlendOpAlpha			= (D3D11_BLEND_OP)src.blendOpAlpha;
		dst.DestBlend				= (D3D11_BLEND)src.destBlend;
		dst.DestBlendAlpha			= (D3D11_BLEND)src.destBlendAlpha;
		dst.SrcBlend				= (D3D11_BLEND)src.srcBlend;
		dst.SrcBlendAlpha			= (D3D11_BLEND)src.srcBlendAlpha;
		dst.RenderTargetWriteMask	= src.renderTargetWriteMask;
	}

	void blendDesc(D3D11_BLEND_DESC& res, const gapi::BlendState& state)
	{
		res.AlphaToCoverageEnable = state.alphaToCoverageEnable;
		res.IndependentBlendEnable = state.independentBlendEnable;

		for (int i = 0; i < 8; ++i)
		{
			renderTargetBlend(res.RenderTarget[i], state.renderTarget[0]);
		}
	}

	DeviceBlendState::DeviceBlendState()
	{

	}

	DeviceBlendState::DeviceBlendState(const gapi::BlendState& state)
	{
		create(state);
	}

	DeviceBlendState::~DeviceBlendState()
	{
		clear();
	}
	
	void DeviceBlendState::create(const gapi::BlendState& state)
	{
		clear();
		D3D11_BLEND_DESC desc;
		blendDesc(desc, state);
		device->device->CreateBlendState(&desc, &ptr);
	}

	void DeviceBlendState::clear()
	{
		SAFE_RELEASE(ptr);
	}

	void DeviceBlendState::set() const
	{
		float factor[4] = {1.0f, 1.0f, 1.0f, 1.0};
		device->context->OMSetBlendState(ptr, factor, 0xFFFFFFFF);
	}

}