// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#include "resourceTexture/library.include.h"
#include "resourceTexture11/library.include.h"
#include "resourceAtlas/library.include.h"
#include "resourceFont/library.include.h"

#include "extractors.h"

namespace unigui {
	namespace draw {

		UniversalTexture VoidTexture::extract(void* data) { return UniversalTexture(0); }
		Vector2 VoidTexture::invsize(void* data) { return Vector2(1.0f, 1.0f); }

		
		UniversalTexture MultiapiTexture::extract(void* data)
		{
			if (gapi::initialized(gapi::DX9))
			{
				return UniversalTexture(((multiapi::Texture*)data)->texture9);
			}
			else if (gapi::initialized(gapi::DX11))
			{
				return UniversalTexture(&((multiapi::Texture*)data)->shaderRes);
			}
		}
		
		Vector2 MultiapiTexture::invsize(void* data)
		{
			int szX = ((multiapi::Texture*)data)->sizeX;
			int szY = ((multiapi::Texture*)data)->sizeY;
			return Vector2(1.0f / szX, 1.0f / szY);
		}
	


		UniversalTexture NativeTexture::extract(void* data) { return UniversalTexture(data); }
		Vector2 NativeTexture::invsize(void* data)
		{
			if (gapi::initialized(gapi::DX9))
			{
				nm::index2 size = texutils::dx9::GetTextureSize(extract(data).tex9);
				return Vector2(1.0f / size.x, 1.0f / size.y);
			}
			else if (gapi::initialized(gapi::DX11))
			{
				return Vector2(1.0f, 1.0f);
			}

			ENFORCE(false);
			return Vector2(0,0);
		}

		UniversalTexture TextureExtrator::extract(void* data)
		{
			UniversalTexture unitex(nullptr);

			if (gapi::initialized(gapi::DX9))
			{
				auto tex = (Resources::Texture*)data;
				tex->resource([&unitex](auto& t) 
				{
					unitex = UniversalTexture(t);
				});
			}
			else if (gapi::initialized(gapi::DX11))
			{
				auto tex = (Resources::Texture11*)data;
				tex->resource([&unitex](auto& t)
				{
					unitex = UniversalTexture(&t.texture);
				});
			}

			return unitex;
		}

		Vector2 TextureExtrator::invsize(void* data)
		{
			if (gapi::initialized(gapi::DX9))
			{
				int szX = ((Resources::Texture*)data)->sizeX;
				int szY = ((Resources::Texture*)data)->sizeY;
				return Vector2(1.0f / szX, 1.0f / szY);
			}
			else if (gapi::initialized(gapi::DX11))
			{
				int szX, szY;
				Resources::Texture11* tex = (Resources::Texture11*)data;
				tex->Size(szX, szY);
				return Vector2(1.0f / szX, 1.0f / szY);
			}

			ENFORCE(false);
			return Vector2(0, 0);
		}

		UniversalTexture RenderTargetExtrator::extract(void* data)
		{
			INCOMPLETE;
			return UniversalTexture(0);


		}

		Vector2 RenderTargetExtrator::invsize(void* data)
		{
			INCOMPLETE;	   			
			return Vector2(0, 0);




		}

		UniversalTexture DepthStencilExtrator::extract(void* data)
		{
			INCOMPLETE;
			return UniversalTexture(0);


		}

		Vector2 DepthStencilExtrator::invsize(void* data)
		{
			INCOMPLETE;
			return Vector2(0, 0);




		}

		UniversalTexture AtlasExtrator::extract(void* data)
		{
			UniversalTexture unitex(nullptr);

			((Resources::Atlas*)data)->resource([&unitex](auto& res) 
			{
				unitex = gapi::initialized(gapi::DX9) ? UniversalTexture(res.texture.texture9) : UniversalTexture(&res.texture.shaderRes);
			});

			return unitex;			
		}

		Vector2 AtlasExtrator::invsize(void* data)
		{
			Vector2 result(1.0f, 1.0f);

			((Resources::Atlas*)data)->resource([&result](auto& res) 
			{
				result = Vector2(1.0f / res.texture.sizeX, 1.0f / res.texture.sizeY);
			});
			
			return result;
		}

		UniversalTexture NativeAtlasExtrator::extract(void* data)
		{
			RectAtlasGAPI* res = (RectAtlasGAPI*) data;
			return gapi::initialized(gapi::DX9) ? UniversalTexture(res->texture.texture9) : UniversalTexture(&res->texture.shaderRes);
		}

		Vector2 NativeAtlasExtrator::invsize(void* data)
		{
			RectAtlasGAPI* res = (RectAtlasGAPI*) data;
			return Vector2(1.0f / res->texture.sizeX, 1.0f / res->texture.sizeY);
		}

		UniversalTexture FontExtrator::extract(void* data)
		{
			UniversalTexture unitex(nullptr);

			((Resources::Font*)data)->resource([&unitex](auto& res)
			{
				unitex = gapi::initialized(gapi::DX9) ? UniversalTexture(res.texture.texture9) : UniversalTexture(&res.texture.shaderRes);
			});

			return unitex;
		}

		Vector2 FontExtrator::invsize(void* data)
		{
			Vector2 result(1.0f, 1.0f);

			((Resources::Font*)data)->resource([&result](auto& res)
			{
				result = Vector2(1.0f / res.texture.sizeX, 1.0f / res.texture.sizeY);
			});

			return result;
		}


	}
}



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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