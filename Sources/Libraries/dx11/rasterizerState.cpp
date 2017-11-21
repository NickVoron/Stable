#include "rasterizerState.h"
#include "device.h"
#include "samplers.h"

namespace dx11
{
	D3D11_FILL_MODE fillMode(gapi::FillMode mode)
	{
		return mode == gapi::SOLID ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;
	}

	D3D11_CULL_MODE cullMode(gapi::CullMode mode)
	{
		return mode == gapi::NONE ? D3D11_CULL_NONE : (mode == gapi::BACK ? D3D11_CULL_BACK : D3D11_CULL_FRONT);
	}

	D3D11_RASTERIZER_DESC rasterizerDesc(const gapi::RasterizerState& state)
	{
		D3D11_RASTERIZER_DESC res;
		res.FillMode = fillMode(state.fillMode);
		res.CullMode = cullMode(state.cullMode);
		
		res.FrontCounterClockwise = state.frontCounterClockwise;
		res.DepthBias = state.depthBias;
		res.DepthBiasClamp = state.depthBiasClamp;
		res.SlopeScaledDepthBias = state.slopeScaledDepthBias;
		res.DepthClipEnable = state.depthClipEnable;
		res.ScissorEnable = state.scissorEnable;
		res.MultisampleEnable = state.multisampleEnable;
		res.AntialiasedLineEnable = state.antialiasedLineEnable;

		return res;
	}

	//
	//
	//
	void DeviceRasterizerState::create(const gapi::RasterizerState& state)
	{
		D3D11_RASTERIZER_DESC desc = rasterizerDesc(state);
		device->device->CreateRasterizerState(&desc, &ptr);
	}


	void DeviceRasterizerState::set() const
	{
		device->context->RSSetState(ptr);
	}

	//
	//
	//

	
	
	D3D11_DEPTH_WRITE_MASK depthWriteMask(gapi::DepthWriteMask mask)
	{
		return mask == gapi::MASK_ALL ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	D3D11_STENCIL_OP stencilOp(gapi::StencilOp op)
	{
		switch (op)
		{
		case gapi::STENCIL_OP_KEEP:		return D3D11_STENCIL_OP_KEEP;
		case gapi::STENCIL_OP_ZERO:		return D3D11_STENCIL_OP_ZERO;
		case gapi::STENCIL_OP_REPLACE:	return D3D11_STENCIL_OP_REPLACE;
		case gapi::STENCIL_OP_INCR_SAT:	return D3D11_STENCIL_OP_INCR_SAT;
		case gapi::STENCIL_OP_DECR_SAT:	return D3D11_STENCIL_OP_DECR_SAT;
		case gapi::STENCIL_OP_INVERT:	return D3D11_STENCIL_OP_INVERT;
		case gapi::STENCIL_OP_INCR:		return D3D11_STENCIL_OP_INCR;
		case gapi::STENCIL_OP_DECR:		return D3D11_STENCIL_OP_DECR;
		}

		return D3D11_STENCIL_OP_KEEP;
	}

	D3D11_DEPTH_STENCILOP_DESC depthStencilOpDesc(const gapi::DepthStencilOpDesc& state)
	{
		D3D11_DEPTH_STENCILOP_DESC res;

		res.StencilPassOp		= stencilOp(state.stencilPassOp);
		res.StencilFailOp		= stencilOp(state.stencilFailOp);
		res.StencilDepthFailOp	= stencilOp(state.stencilDepthFailOp);
		res.StencilFunc			= comparsion(state.stencilFunc);

		return res;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc(const gapi::DepthStencilState& state)
	{
		D3D11_DEPTH_STENCIL_DESC res;

		res.DepthEnable = state.depthEnable;
		res.DepthWriteMask = depthWriteMask(state.depthWriteMask);
		res.DepthFunc = comparsion(state.depthFunc);

		res.StencilEnable = state.stencilEnable;
		res.StencilReadMask = state.stencilReadMask;
		res.StencilWriteMask = state.stencilWriteMask;

		res.BackFace = depthStencilOpDesc(state.backFace);
		res.FrontFace = depthStencilOpDesc(state.frontFace);
				
		return res;
	}

	//
	//
	//

	void DeviceDepthStencilState::create(const D3D11_DEPTH_STENCIL_DESC& desc)
	{
		ENFORCE(SUCCEEDED(device->device->CreateDepthStencilState(&desc, &ptr)));
	}

	void DeviceDepthStencilState::set(UINT8 stencilRef) const
	{
		device->context->OMSetDepthStencilState(ptr, stencilRef);
	}

	//
	//
	//
	void DepthStencilState::create()
	{
		const D3D11_DEPTH_STENCIL_DESC desc = depthStencilDesc(state);
		for (int i = 0; i < devices->count(); ++i)	
		{ 
			data[i].create(desc);
		} 
	}

	void DepthStencilState::execute(int deviceIndex)
	{
		data[deviceIndex].set(state.stencilRef);
	}
}