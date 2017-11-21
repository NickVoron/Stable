#include <d3d11.h>

#include "device.h"

#include "stream/library.include.h"

namespace dx11
{
	unsigned int calculateTextureMemSize(const D3D11_TEXTURE2D_DESC& desc);
	unsigned int calculateTextureMemSize(ID3D11Texture2D* texture);
	void saveTexture(Device* device, ID3D11Texture2D* texture,stream::ostream& os);
	void loadTexture(ID3D11Device* device, ID3D11Texture2D*& texture, stream::istream& is);
}