#pragma once

#ifdef WIN32

#include "image/library.include.h"
#include "dx9/library.include.h"

#include "freeimage.h"

namespace image
{
namespace dx9
{
	template<Format fmt> struct image_to_d3d;
	template<D3DFORMAT fmt> struct d3d_to_image;

	template<> struct image_to_d3d<R8>						{ static const Format imageFmt = R8;		static const D3DFORMAT d3dFmt = D3DFMT_L8;				};
	template<> struct image_to_d3d<ARGB8>					{ static const Format imageFmt = ARGB8;		static const D3DFORMAT d3dFmt = D3DFMT_A8R8G8B8;		};
	template<> struct image_to_d3d<ABGR8>					{ static const Format imageFmt = ABGR8;		static const D3DFORMAT d3dFmt = D3DFMT_A8B8G8R8;		};
	template<> struct image_to_d3d<ABGR16>					{ static const Format imageFmt = ABGR16;	static const D3DFORMAT d3dFmt = D3DFMT_A16B16G16R16;	};
	template<> struct image_to_d3d<ABGR16F> 				{ static const Format imageFmt = ABGR16F;	static const D3DFORMAT d3dFmt = D3DFMT_A16B16G16R16F;	};
	template<> struct image_to_d3d<ABGR32F> 				{ static const Format imageFmt = ABGR32F;	static const D3DFORMAT d3dFmt = D3DFMT_A32B32G32R32F;	};

	template<> struct d3d_to_image<D3DFMT_L8>				{ static const Format imageFmt = R8;		static const D3DFORMAT d3dFmt = D3DFMT_L8;				};
	template<> struct d3d_to_image<D3DFMT_A8R8G8B8>			{ static const Format imageFmt = ARGB8;		static const D3DFORMAT d3dFmt = D3DFMT_A8R8G8B8;		};
	template<> struct d3d_to_image<D3DFMT_A8B8G8R8>			{ static const Format imageFmt = ABGR8;		static const D3DFORMAT d3dFmt = D3DFMT_A8B8G8R8;		};
	template<> struct d3d_to_image<D3DFMT_A16B16G16R16>		{ static const Format imageFmt = ABGR16;	static const D3DFORMAT d3dFmt = D3DFMT_A16B16G16R16;	};
	template<> struct d3d_to_image<D3DFMT_A16B16G16R16F>	{ static const Format imageFmt = ABGR16F;	static const D3DFORMAT d3dFmt = D3DFMT_A16B16G16R16F;	};
	template<> struct d3d_to_image<D3DFMT_A32B32G32R32F>	{ static const Format imageFmt = ABGR32F;	static const D3DFORMAT d3dFmt = D3DFMT_A32B32G32R32F;	};

	template<D3DFORMAT fmt> struct d3d_pixel_size {	static const unsigned int value = pixel_info< d3d_to_image<fmt>::imageFmt >::value;	};

	template<D3DFORMAT fmt> struct d3d_to_image_color {	typedef Color< d3d_to_image<fmt>::imageFmt > Color; };

	template<D3DFORMAT fmt> struct d3d_to_image_line	{ typedef Line		<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_plane	{ typedef Plane		<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_volume	{ typedef Volume	<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_cubemap	{ typedef Cubemap	<typename d3d_to_image_color<fmt>::Color> Result; };

	template<Format fmt>
	LPDIRECT3DTEXTURE9 createTexture(const Plane< Color<fmt> >& plane)
	{
		INCOMPLETE;
		LPDIRECT3DTEXTURE9 texture = nullptr;
// 		D3DXCreateTexture(::dx9::device, plane.sizeX, plane.sizeY, 1, D3DUSAGE_DYNAMIC, image_to_d3d<fmt>::d3dFmt, D3DPOOL_DEFAULT, &texture);
// 
// 		updateTexture(texture, 0, plane);

		return texture;
	}

	template<Format fmt>
	LPDIRECT3DTEXTURE9 loadTexture(const char* fileName)
	{
		Plane< Color<fmt> > plane;
		image::freeimage::load(fileName, plane);

		LPDIRECT3DTEXTURE9 texture = createTexture(plane);
		plane.clear();

		return texture;
	}

	template<Format fmt>
	LPDIRECT3DTEXTURE9 createTexture(const Plane< Color<fmt> >* levels, int levelsCount)
	{
		LPDIRECT3DTEXTURE9 texture;
		int w = levels[0].sizeX;
		int h = levels[0].sizeY;
		D3DXCreateTexture(::dx9::device, w, h, levelsCount, 0, image_to_d3d<fmt>::d3dFmt, D3DPOOL_SYSTEMMEM, &texture);

		for(int l = 0; l < levelsCount; ++l)
		{
			updateTexture(texture, l, levels[l]);
		}		

		LPDIRECT3DTEXTURE9 res;
		D3DXCreateTexture(::dx9::device, w, h, levelsCount, 0, image_to_d3d<fmt>::d3dFmt, D3DPOOL_DEFAULT, &res);
		::dx9::device->UpdateTexture(texture, res);
		SAFE_RELEASE(texture);

		return res;
	}

	template<Format fmt>
	void lockTexture(LPDIRECT3DTEXTURE9 texture, unsigned int level, Plane< Color<fmt> >& plane)
	{
		D3DSURFACE_DESC surfaceDesc;
		texture->GetLevelDesc(level, &surfaceDesc);
		D3DLOCKED_RECT rect;
		texture->LockRect(level, &rect, 0, 0);
		plane.init(rect.pBits, rect.Pitch, surfaceDesc.Width, surfaceDesc.Height);
	}

	inline void unlockTexture(LPDIRECT3DTEXTURE9 texture, unsigned int level)
	{
		texture->UnlockRect(level);
	}

	template<Format fmt>
	void updateTexture(LPDIRECT3DTEXTURE9 texture, unsigned int level, const Plane< Color<fmt> >& plane)
	{
		ENFORCE(texture);

		D3DSURFACE_DESC surfaceDesc;
		texture->GetLevelDesc(level, &surfaceDesc);
		ENFORCE(plane.sizeX == surfaceDesc.Width && plane.sizeY == surfaceDesc.Height);

		Plane< Color<fmt> > target;
		lockTexture(texture, level, target);
		
		if(target.pitchInBytes == plane.pitchInBytes)
		{
			mem::memcpy(target.bmp, plane.bmp, plane.memory() );
		}
		else
		{
			for (int y = 0; y < plane.sizeY; ++y)
			{
				for (int x = 0; x < plane.sizeX; ++x)
				{
					target(x, y) = plane(x, y);
				}
			}
		}

		unlockTexture(texture, level);
	}

	template<Format fmt>
	void saveTexture(const char* fileName, const Plane< Color<fmt> >& plane)
	{
		LPDIRECT3DTEXTURE9 texture = createTexture(plane);
		ENFORCE(texture);
		D3DXSaveTextureToFileA(fileName, D3DXIFF_DDS, texture, 0);
		texture->Release();
	}

	template<Format fmt>
	void saveTexture(const char* fileName, const Plane< Color<fmt> >* levels, int levelsCount)
	{
		LPDIRECT3DTEXTURE9 texture = createTexture(levels, levelsCount);
		ENFORCE(texture);
		D3DXSaveTextureToFileA(fileName, D3DXIFF_DDS, texture, 0);
		texture->Release();
	}

	void lockTexture(LPDIRECT3DTEXTURE9 texture, int level, ImageInfo& imageInfo);
	void lockTexture(LPDIRECT3DTEXTURE9 texture, ImageInfo& imageInfo);


}
}

#endif