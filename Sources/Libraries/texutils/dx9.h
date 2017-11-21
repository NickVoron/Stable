#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "stream/library.include.h"
#include "image/library.include.h"
#include "common/delete.h"


namespace texutils
{
	namespace dx9
	{
		void FakeFixTextureMapBorder(LPDIRECT3DTEXTURE9 nm);

		void SaveTextureToStream(stream::ostream& s, const std::string& fileName);
		void SaveTextureToStream(stream::ostream& s, const char* fileName);
		void SaveTextureToStream(stream::ostream& s, LPDIRECT3DTEXTURE9 texture);

		void SaveCubeTextureToStream(stream::ostream& s, LPDIRECT3DCUBETEXTURE9 texture);

		inline bool isDXT(D3DFORMAT f) { return f == D3DFMT_DXT1 || f == D3DFMT_DXT2 || f == D3DFMT_DXT3 || f == D3DFMT_DXT4 || f == D3DFMT_DXT5; }

		void LoadTextureFromStream(stream::istream& s, LPDIRECT3DTEXTURE9* texture, int* sizeX = 0, int* sizeY = 0);
		void LoadTextureFromStream(stream::istream& s, int topLevel, LPDIRECT3DTEXTURE9* texture, int* sizeX = 0, int* sizeY = 0);
		void LoadCubeTextureFromStream(stream::istream& s, LPDIRECT3DCUBETEXTURE9* texture);

		void SaveD3DBufferToStream(stream::ostream& s, LPD3DXBUFFER buf);
		void LoadD3DBufferFromStream(stream::istream& s, LPD3DXBUFFER* buf);
		void SaveD3DBufferToFile(const std::string& fileName, LPD3DXBUFFER buf);
		void LoadD3DBufferFromFile(const std::string& fileName, LPD3DXBUFFER* buf);

/*
		template<class ColorType>
		void CopyImageRect(image::Plane<ColorType>& srcImage, image::Plane<ColorType>& dstImage, const Base::IndexRect& srcRect, const Base::IndexRect& dstRect)
		{
			//validation
			if(srcRect.size != dstRect.size) throw Base::Errors::Simple(__FUNCTION__": rects sizes must be equal");

			nm::index2 si, di;
			for(si.x = srcRect.minCorner.x, di.x = dstRect.minCorner.x; si.x < srcRect.minCorner.x + srcRect.size.x; ++si.x, ++di.x)
			{
				for(si.y = srcRect.minCorner.y, di.y = dstRect.minCorner.y; si.y < srcRect.minCorner.y + srcRect.size.y; ++si.y, ++di.y)
				{
					ColorType& dst = dstImage(di.x, di.y);
					ColorType& src = srcImage(si.x, si.y);

					dst = src;
				}
			}
		}

		template<D3DFORMAT format>
		void CopyTextureRect(LPDIRECT3DTEXTURE9 src, LPDIRECT3DTEXTURE9 dst, const Base::IndexRect& srcRect, const Base::IndexRect& dstRect)
		{
			//validation
			ENFORCE(srcRect.size == dstRect.size);
			ENFORCE(GetTextureFormat(src) == GetTextureFormat(dst));

			D3DLOCKED_RECT dst_lockedRect;
			D3DLOCKED_RECT src_lockedRect;

			dst->LockRect(0, &dst_lockedRect, 0, 0);
			src->LockRect(0, &src_lockedRect, 0, 0);

			typedef image::d3d::d3d_to_image_plane<format>::Result ImagePlane;

			ImagePlane srcImage(src_lockedRect.pBits, src_lockedRect.Pitch, srcRect.size.x, srcRect.size.y);
			ImagePlane dstImage(dst_lockedRect.pBits, dst_lockedRect.Pitch, dstRect.size.x, dstRect.size.y);

			CopyImageRect(srcImage, dstImage, srcRect, dstRect);

			dst->UnlockRect(0);
			src->UnlockRect(0);
		}*/

		template<D3DFORMAT format>
		void SliceTexture(LPDIRECT3DTEXTURE9 src, LPDIRECT3DTEXTURE9* dst, const nm::index2& slicesCount)
		{
			nm::index2 srcSize = GetTextureSize(src);
			nm::index2 sliceSize = srcSize / slicesCount;


			int j = 0;
			nm::index2 i;
			nm::index2 srcPos;
			for(i.y = 0; i.y < slicesCount.y; ++i.y)
			{
				srcPos.y = sliceSize.y * i.y;
				for(i.x = 0; i.x < slicesCount.x; ++i.x)
				{
					dst[j] = CreateTexture<format>(sliceSize);
					srcPos.x = sliceSize.x * i.x;

					CopyTextureRect<format>(src, dst[j], Base::IndexRect(srcPos, sliceSize), Base::IndexRect(nm::index2(0, 0), sliceSize) );

					++j;
				}		
			}
		}

		template<D3DFORMAT format>
		void SliceTexture(const char* fileName, const char* outDir, const nm::index2& slicesCount)
		{
			boost::filesystem::path od = outDir;
			Base::FileUtils::create_dir(od);

			LPDIRECT3DTEXTURE9 src = LoadTexture(fileName, true);
			int count = slicesCount.Square();
			LPDIRECT3DTEXTURE9* dst = new LPDIRECT3DTEXTURE9[ count ];

			SliceTexture<format>(src, dst, slicesCount);

			std::string fileNameClear = Base::StrUtils::GetFileNameOnly(fileName);

			for(int i = 0; i < count; ++i)
			{
				char ss[64];
				sprintf(ss, "_%.2d.", i);
				std::string name = outDir + Base::StrUtils::GetFileNameMinusLastExtention(fileNameClear)+ ss + "dds";// Base::FileUtils::GetLastExtention(fileName);
				SaveTextureDDS(name.c_str(), dst[i]);
				SAFE_RELEASE(dst[i]);
			}

			delete [] dst;
		}

		template<class ColorType>
		void LockTexture(LPDIRECT3DTEXTURE9 tex, image::Plane<ColorType>& image)
		{
			// 	ENFORCE( image::d3d::image_to_d3d<ColorType>::format == GetTextureFormat(tex) ); 

			D3DLOCKED_RECT lockedRect;
			tex->LockRect(0, &lockedRect, 0, 0);

			nm::index2 size = GetTextureSize(tex);
			image.init(lockedRect.pBits, lockedRect.Pitch, size.x, size.y);
		}

		inline void UnlockTexture(LPDIRECT3DTEXTURE9 tex)
		{
			tex->UnlockRect(0);
		}


		template<class ColorType>
		struct TextureAccessor
		{
			typedef image::Plane<ColorType> ImagePlane;

			void lock() {LockTexture(tex, image);}
			void unlock(){UnlockTexture(tex);}

			LPDIRECT3DTEXTURE9 tex;
			ImagePlane image;
		};



		LPDIRECT3DTEXTURE9 LoadTexture( const char* fileName, bool dynamic, bool systemMem);
		LPDIRECT3DCUBETEXTURE9 LoadCubeTexture( const char* fileName,  bool dynamic);

		bool SaveTextureDDS( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture );
		bool SaveTextureJPG( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture );
		bool SaveTexturePNG( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture );

		nm::index2 GetTextureSize(LPDIRECT3DTEXTURE9 tex);
		void GetTexelSize(LPDIRECT3DTEXTURE9 tex, float& w, float& h);
		D3DFORMAT GetTextureFormat(LPDIRECT3DTEXTURE9 tex);
		bool IsPow2Texture(LPDIRECT3DTEXTURE9 tex, nm::index2* size = 0);
		void FlipTextureY(LPDIRECT3DTEXTURE9 tex);
		bool TexturesSizeIsEqual(LPDIRECT3DTEXTURE9 t1, LPDIRECT3DTEXTURE9 t2);
		bool TexturesFormatIsEqual(LPDIRECT3DTEXTURE9 t1, LPDIRECT3DTEXTURE9 t2);

		void FillSolidTexture( LPDIRECT3DTEXTURE9 pTexture, unsigned int color);
		LPDIRECT3DTEXTURE9 CreateDummyTexture(D3DFORMAT fmt); 
		LPDIRECT3DTEXTURE9 CreateTextureARGB( const nm::index2& size);
		LPDIRECT3DTEXTURE9 CreateTextureABGR32F( const nm::index2& size, int mipsCount = 0);
		void CreateSolidTexture( LPDIRECT3DTEXTURE9 *ppTexture, const nm::index2& size, unsigned int color);
		void CreateSolidCubeMap( LPDIRECT3DCUBETEXTURE9 *ppTexture, int size, unsigned int color[6]);
		// void ComputeBoundBox(dx9::Mesh &xmesh, Geometry::AABB &aabb);
		// Vector3 getXMeshBoudingBoxSize(dx9::Mesh& xmesh);

		template<D3DFORMAT fmt>
		LPDIRECT3DTEXTURE9 CreateTexture(const nm::index2& size)
		{
			LPDIRECT3DTEXTURE9 texture;
			D3DXCreateTexture(dx9::device, size.x, size.y, 0, 0, fmt, D3DPOOL_MANAGED, &texture);
			return texture;
		}
		template<typename T>
		void LoadImageFromDDSFile(image::Plane<T>& tex, const wchar_t* fname)
		{
			IDirect3DTexture9* d3dtex;
			HRESULT hr = D3DXCreateTextureFromFile(dx9::device, fname, &d3dtex);

			D3DSURFACE_DESC Desc;
			d3dtex->GetLevelDesc(0,&Desc);

			D3DLOCKED_RECT LockedRect;
			d3dtex->LockRect(0, &LockedRect, 0, D3DLOCK_READONLY);

			T* bmp = new T[ Desc.Height * Desc.Width ];
			mem::memcpy(bmp, LockedRect.pBits, Desc.Height * Desc.Width * sizeof(T));
			d3dtex->UnlockRect(0);

			tex.init(bmp, Desc.Width * sizeof(T), nm::index2(Desc.Width, Desc.Height));

			d3dtex->Release();
		}

		template<typename T>
		void SaveImageToDDSFile( image::Plane<T>& tex, const wchar_t* fname)
		{
			IDirect3DTexture9* d3dtex;
			dx9::device->CreateTexture(tex.size.x, tex.size.y, 1, D3DUSAGE_DYNAMIC, D3DFMT_A32B32G32R32F, D3DPOOL_SYSTEMMEM, &d3dtex, 0);
			D3DLOCKED_RECT LockedRect;
			d3dtex->LockRect(0, &LockedRect, 0, D3DLOCK_READONLY);
			mem::memcpy(LockedRect.pBits, tex.bmp, tex.size.Square() * sizeof(T) );
			d3dtex->UnlockRect(0);

			D3DXSaveTextureToFile(fname, D3DXIFF_DDS, d3dtex, 0);
			d3dtex->Release();
		}
	}
}
