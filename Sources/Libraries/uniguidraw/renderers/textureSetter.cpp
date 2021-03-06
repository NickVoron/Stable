// Copyright (C) 2015-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "textureSetter.h"
#ifdef USE_WINDOWS
#include "../dx9.h"
#include "../dx11.h"
#include "../internalState.h"

namespace unigui {
namespace draw {

TextureSetter9::TextureSetter9(const UniversalTexture& unitex)
{




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

}
}
#endif




// Copyright (C) 2015-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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