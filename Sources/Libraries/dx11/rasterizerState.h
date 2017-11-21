#include "gapi/library.include.h"

#include "interfaces.h"

namespace dx11
{
	D3D11_RASTERIZER_DESC rasterizerDesc(const gapi::RasterizerState& state);
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc(const gapi::DepthStencilState& state);

	class DeviceRasterizerState : public DeviceResource<ID3D11RasterizerState, DeviceRasterizerState>
	{
	public:
		void create(const gapi::RasterizerState& state);
		void set() const;
	};

	struct RasterizerState : public DeviceRasterizerState::StateSet	
	{ 
	};

	//
	//
	//
	class DeviceDepthStencilState : public DeviceResource<ID3D11DepthStencilState, DeviceDepthStencilState>
	{
	public:
		void create(const D3D11_DEPTH_STENCIL_DESC& desc);
		void set(UINT8 stencilRef) const;
	};

	struct DepthStencilState : public DeviceDepthStencilState::ResultBase
	{
		gapi::DepthStencilState state;

		void create();
		
		virtual void execute(int deviceIndex);
	};
}