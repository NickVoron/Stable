// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#if defined(USE_WINDOWS)

#include "dx9/library.include.h"
#include "dx11/library.include.h"
#include "stream/library.include.h"
#include "image/library.include.h"

namespace multiapi
{
	struct Texture
	{
	public:
		Texture();
		~Texture();

		template<image::Format format>
		void create(const image::Plane< image::Color<format> >& img);
		void save(stream::ostream& os) const;
		void load(stream::istream& is);
		void clear();

		int sizeX;
		int sizeY;

		LPDIRECT3DTEXTURE9 texture9;

		dx11::Texture texture;
		dx11::ShaderResource shaderRes;

	private:
		void defaults();
	};

	template<image::Format format>
	void Texture::create(const image::Plane< image::Color<format> >& img)
	{
		clear();

		if (gapi::initialized(gapi::DX9))
		{
			texture9 = image::dx9::createTexture(img);
		}
		else if (gapi::initialized(gapi::DX11))
		{
			texture.create(img);

			ZeroMemory(&shaderRes.desc, sizeof(shaderRes.desc));
			shaderRes.desc.Format = image::dx11::image_to_d3d<format>::d3dFmt;
			shaderRes.desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderRes.desc.Texture2D.MostDetailedMip = 0;
			shaderRes.desc.Texture2D.MipLevels = 1;

			shaderRes.create(texture);
		}

		sizeX = img.sizeX;
		sizeY = img.sizeY;
	}
}

#endif



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