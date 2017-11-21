#include "gapi/library.include.h"
#include "device.h"
#include "interfaces.h"

namespace dx11
{
	D3D11_BLEND_DESC blendDesc(const gapi::BlendState& state);

	class DeviceBlendState : public DeviceResource<ID3D11BlendState, DeviceBlendState>
	{
	public:
		DeviceBlendState();
		DeviceBlendState(const gapi::BlendState& state);
		~DeviceBlendState();

		void create(const gapi::BlendState& state);
		void clear();
		void set() const;
	};

	struct BlendState : public DeviceBlendState::StateSet { };
}