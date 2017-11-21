#include "textureSetter.h"
#include "../dx9.h"
#include "../dx11.h"
#include "../internalState.h"

namespace unigui {
namespace draw {

TextureSetter9::TextureSetter9(const UniversalTexture& unitex)
{
// 	if (dx9::effectFX)
// 	{
// 		dx9::effectFX->SetTexture("texRect", unitex.tex9);
// 	}
}

TextureSetter11::TextureSetter11(const UniversalTexture& unitex)
{
	Viewports::Viewport* v = dx11::currentViewport.viewport();
	if (unitex.tex11 && v)
	{
		unitex.tex11->pixel(0);
		dx11::sampler().pixel(0);

		unitex.tex11->execute(v->deviceIndex());
		dx11::sampler().execute(v->deviceIndex());
	}
}

TextureSetterAPI::TextureSetterAPI(const UniversalTexture& unitex)
{
	if (gapi::initialized(gapi::DX9))
	{
		TextureSetter9 setter(unitex);
	}
	else if (gapi::initialized(gapi::DX11))
	{
		TextureSetter11 setter(unitex);
	}
}

}//
}//