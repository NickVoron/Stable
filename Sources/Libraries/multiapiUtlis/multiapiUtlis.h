#pragma once

#ifdef WIN32

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