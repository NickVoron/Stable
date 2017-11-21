#include "dx9.h"

#ifdef WIN32

namespace image{
namespace dx9{

	void lockTexture(LPDIRECT3DTEXTURE9 tex, int level, ImageInfo& imageInfo)
	{
		D3DLOCKED_RECT lockedRect;
		D3DSURFACE_DESC surfaceDesc;

		tex->LockRect(0, &lockedRect, 0, 0);
		tex->GetLevelDesc(0, &surfaceDesc);

		//imageInfo.init(lockedRect.pBits, false, , surfaceDesc.Width, surfaceDesc.Height, lockedRect.Pitch);
	}

	void lockTexture(LPDIRECT3DTEXTURE9 texture, ImageInfo& imageInfo)
	{
		lockTexture(texture, 0, imageInfo);
	}
}
}

#endif