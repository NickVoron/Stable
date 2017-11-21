#pragma once
#include "dx11/library.include.h"

#include "types.h"

namespace unigui {
namespace draw {

	struct UniversalTexture
	{
		UniversalTexture(void* data) :tex9((LPDIRECT3DTEXTURE9) data){}

		operator bool() const { return tex9 != 0; }
		union
		{
			LPDIRECT3DTEXTURE9 tex9;
			::dx11::ShaderResource* tex11;
		};
	};

	struct VoidTexture
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct MultiapiTexture
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct NativeTexture
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct TextureExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct RenderTargetExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct DepthStencilExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct AtlasExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct NativeAtlasExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

	struct FontExtrator
	{
		static UniversalTexture extract(void* data);
		static Vector2 invsize(void* data);
	};

}
}