#include "multiapiUtlis.h"

#ifdef WIN32


#include "dx11/library.include.h"
#include "texutils/library.include.h"
#include "imageD3D/library.include.h"

namespace multiapi
{
//
//
//
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