// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "multiapiUtlis.h"

#if defined(USE_WINDOWS)

#include "dx11/library.include.h"
#include "texutils/library.include.h"
#include "imageD3D/library.include.h"

namespace multiapi
{



	Texture::Texture()
	{
		defaults();		
	}

	Texture::~Texture()
	{
		clear();
	}

	void Texture::defaults()
	{
		texture9 = 0;
		sizeX = 0;
		sizeY = 0;
	}

	void Texture::save(stream::ostream& os) const
	{
		os << sizeX << sizeY;

		if (gapi::initialized(gapi::DX9))
		{
			texutils::dx9::SaveTextureToStream(os, texture9);
		}
		else if (gapi::initialized(gapi::DX11))
		{
			texture.save(os);
			shaderRes.save(os);
		}

	}

	void Texture::load(stream::istream& is)
	{
		clear();

		is >> sizeX >> sizeY;

		if(gapi::initialized(gapi::DX9))
		{
			texutils::dx9::LoadTextureFromStream(is, &texture9);
		}
		else if (gapi::initialized(gapi::DX11))
		{
			texture.load(is);
			shaderRes.load(is);
			shaderRes.create(texture);
		}	
	}

	void Texture::clear()
	{
		SAFE_RELEASE(texture9);

		defaults();
	}
}

#endif // WIN32



// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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