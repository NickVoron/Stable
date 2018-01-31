// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "dx9.h"

#if defined(USE_WINDOWS)

#include "image/library.include.h"
#include "imageD3D/library.include.h"
#include "dx9/library.include.h"

namespace texutils
{
	namespace dx9
	{
		void FakeFixTextureMapBorder(LPDIRECT3DTEXTURE9 nm)
		{
			typedef image::Color<image::ARGB8> ColorType;
			typedef image::Plane<ColorType> ImagePlane;

			nm::index2 size = GetTextureSize(nm);

			D3DLOCKED_RECT rect;
			nm->LockRect(0, &rect, 0, 0);
			ImagePlane image(rect.pBits, rect.Pitch, size.x, size.y);

			nm::index2 i;
			nm::index2 srcIdx;

			
			
			

			
			i = nm::index2(0, 0);
			srcIdx = nm::index2(0, 1);
			for(i.x = 0; i.x < size.x; ++i.x, ++srcIdx.x)
			{
				image(i.x, i.y) = image(srcIdx.x, srcIdx.y);
			}

			
			i = nm::index2(0, 0);
			srcIdx = nm::index2(1, 0);
			for(i.y = 0; i.y < size.y; ++i.y, ++srcIdx.y)
			{
				image(i.x, i.y) = image(1, i.y);
			}

			
			i = nm::index2(0, size.y - 1);
			srcIdx = nm::index2(0, size.y - 2);
			for(i.x = 0; i.x < size.x; ++i.x, ++srcIdx.x)
			{
				image(i.x, i.y) = image(srcIdx.x, srcIdx.y);
			}

			
			i = nm::index2(size.x - 1, 0);
			srcIdx = nm::index2(size.x - 2, 0);
			for(i.y = 0; i.y < size.y; ++i.y, ++srcIdx.y)
			{
				image(i.x, i.y) = image(srcIdx.x, srcIdx.y);
			}

			nm->UnlockRect(0);
		}

		void SaveTextureToStream(stream::ostream& s, const std::string& fileName)
		{
			SaveTextureToStream(s, fileName.c_str());
		}

		void SaveTextureToStream(stream::ostream& s, const char* fileName)
		{
			LPDIRECT3DTEXTURE9 tex = 0;
			D3DXCreateTextureFromFileA(::dx9::device, fileName, &tex);

			SaveTextureToStream(s, tex);

			SAFE_RELEASE(tex);
		}

		void SaveTextureToStream(stream::ostream& s, LPDIRECT3DTEXTURE9 tex)
		{			  
			int mipsCount = tex->GetLevelCount();
			D3DFORMAT format = GetTextureFormat(tex);

			D3DSURFACE_DESC topLevelDesc;
			tex->GetLevelDesc(0, &topLevelDesc);

			nm::index2 topLevelSize(topLevelDesc.Width, topLevelDesc.Height);

			int sizes[32];
			int pitches[32];
			mem::memzero(sizes);
			mem::memzero(pitches);

			for(int level = mipsCount - 1; level >=0 ; --level)
			{
				D3DSURFACE_DESC surfaceDesc;
				tex->GetLevelDesc(level, &surfaceDesc);

				D3DLOCKED_RECT rect;
				tex->LockRect(level, &rect, 0, 0);
				ENFORCE(rect.pBits && rect.Pitch);
				int d = isDXT(format) ? 4 : 1;
				if(surfaceDesc.Height < 4)	d = 1;

				int size = rect.Pitch * surfaceDesc.Height / d;

				pitches[level] = rect.Pitch;
				sizes[level] = size;
				tex->UnlockRect(level);
			}

			s << mipsCount << format << topLevelSize;
			s.write(sizes, mipsCount * sizeof(sizes[0]));
			s.write(pitches, mipsCount * sizeof(pitches[0]));

			for(int level = mipsCount - 1; level >=0; --level)
			{
				D3DLOCKED_RECT rect;
				tex->LockRect(level, &rect, 0, 0);
				s.write(rect.pBits, sizes[level]); 
				tex->UnlockRect(level);
			}
		}

		void LoadTextureFromStream(stream::istream& s, LPDIRECT3DTEXTURE9* texture, int* sizeX, int* sizeY)
		{
			LoadTextureFromStream(s, 0, texture, sizeX, sizeY);
		}

		void LoadTextureFromStream(stream::istream& s, int topLevel, LPDIRECT3DTEXTURE9* texture, int* sizeX, int* sizeY)
		{
			int mipsCount;
			D3DFORMAT format;

			nm::index2 topLevelSize;
			int sizes[32]; mem::memzero(sizes);
			int pitches[32]; mem::memzero(pitches);
			s >> mipsCount >> format >> topLevelSize;
			s.read(sizes, mipsCount * sizeof(sizes[0]));
			s.read(pitches, mipsCount * sizeof(pitches[0]));

			int maxLevel = mipsCount - 1;
			if(topLevel > maxLevel) topLevel = maxLevel;
			int levelsCount = mipsCount - topLevel;

			if(sizeX) *sizeX = topLevelSize.x;
			if(sizeY) *sizeY = topLevelSize.y;

			LPDIRECT3DTEXTURE9 tex, resTex;
			::dx9::device->CreateTexture(topLevelSize.x, topLevelSize.y, levelsCount, 0, format, D3DPOOL_SYSTEMMEM, &tex, 0);
			::dx9::device->CreateTexture(topLevelSize.x, topLevelSize.y, levelsCount, 0, format, D3DPOOL_DEFAULT, &resTex, 0);

			for(int level = levelsCount - 1; level >= 0; --level)
			{
				D3DLOCKED_RECT rect;
				tex->LockRect(level, &rect, 0, 0);
				ENFORCE(pitches[level] == rect.Pitch);
				s.read(rect.pBits, sizes[level]); 
				tex->UnlockRect(level);
			}

			::dx9::device->UpdateTexture(tex, resTex);
			SAFE_RELEASE(tex);

			*texture = resTex;
		}

		void SaveCubeTextureToStream(stream::ostream& s, LPDIRECT3DCUBETEXTURE9 tex)
		{
			D3DSURFACE_DESC surfaceDesc;
			tex->GetLevelDesc(0, &surfaceDesc);
			DWORD mipsCount = tex->GetLevelCount();

			ENFORCE(surfaceDesc.Width == surfaceDesc.Height);

			s << surfaceDesc.Width << mipsCount << surfaceDesc.Format;

			for(DWORD level = 0; level < mipsCount; ++level)
			{
				D3DSURFACE_DESC surfaceDesc;
				tex->GetLevelDesc(level, &surfaceDesc);
				for(int i = D3DCUBEMAP_FACE_POSITIVE_X; i <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++i)
				{
					D3DLOCKED_RECT lockedRect;
					tex->LockRect((D3DCUBEMAP_FACES)i, level, &lockedRect, 0, 0);
					int d = isDXT(surfaceDesc.Format) ? 4 : 1;
					if(surfaceDesc.Height < 4)	d = 1;

					int size = lockedRect.Pitch * surfaceDesc.Height / d;
					s.write(lockedRect.pBits, size); 

					tex->UnlockRect((D3DCUBEMAP_FACES)i, level);
				}	
			}
		}

		void LoadCubeTextureFromStream(stream::istream& s, LPDIRECT3DCUBETEXTURE9* texture)
		{
			UINT Width;
			UINT Levels;
			D3DFORMAT Format;

			s >> Width >> Levels >> Format;

			LPDIRECT3DCUBETEXTURE9 tex;
			::dx9::device->CreateCubeTexture(Width, Levels, 0, Format, D3DPOOL_SYSTEMMEM, &tex, 0);

			for(UINT level = 0; level < Levels; ++level)
			{
				D3DSURFACE_DESC surfaceDesc;
				tex->GetLevelDesc(level, &surfaceDesc);
				for(int i = D3DCUBEMAP_FACE_POSITIVE_X; i <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++i)
				{
					D3DLOCKED_RECT rect;
					tex->LockRect((D3DCUBEMAP_FACES)i, level, &rect, 0, 0);
					int d = isDXT(Format) ? 4 : 1;
					if(surfaceDesc.Height < 4)	d = 1;

					int size = rect.Pitch * surfaceDesc.Height / d;
					s.read(rect.pBits, size); 
					tex->UnlockRect((D3DCUBEMAP_FACES)i, level);
				}	
			}

			LPDIRECT3DCUBETEXTURE9 resTex;
			::dx9::device->CreateCubeTexture(Width, Levels, 0, Format, D3DPOOL_DEFAULT, &resTex, 0);
			::dx9::device->UpdateTexture(tex, resTex);
			SAFE_RELEASE(tex);

			*texture = resTex;
		}



		bool TexturesSizeIsEqual(LPDIRECT3DTEXTURE9 t1, LPDIRECT3DTEXTURE9 t2)
		{
			return GetTextureSize(t1) == GetTextureSize(t2);
		}

		bool TexturesFormatIsEqual(LPDIRECT3DTEXTURE9 t1, LPDIRECT3DTEXTURE9 t2)
		{
			return GetTextureFormat(t1) == GetTextureFormat(t2);
		}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		

		LPDIRECT3DTEXTURE9 LoadTexture( const char* texFileName,  bool dynamic, bool systemMem )
		{	
			if(Base::FileUtils::FileExists(texFileName))
			{
				LPDIRECT3DTEXTURE9 tex = 0;

				D3DXCreateTextureFromFileExA(
					::dx9::device,
					texFileName,
					D3DX_DEFAULT_NONPOW2,
					D3DX_DEFAULT_NONPOW2,
					D3DX_DEFAULT,
					dynamic ? D3DUSAGE_DYNAMIC : 0,
					D3DFMT_A8R8G8B8,
					systemMem ? D3DPOOL_SYSTEMMEM : D3DPOOL_DEFAULT,
					D3DX_FILTER_NONE,
					D3DX_FILTER_NONE,
					0,
					NULL,
					NULL,
					&tex);
				return tex ? tex : image::dx9::loadTexture<image::ARGB8>(texFileName);
			}

			return 0;
		}

		LPDIRECT3DCUBETEXTURE9 
		LoadCubeTexture( const char* texFileName,  bool dynamic )
		{	
			if(Base::FileUtils::FileExists(texFileName))
			{
				LPDIRECT3DCUBETEXTURE9 tex;
				IDirect3DDevice9* drv = ::dx9::device;
				HRESULT r = D3DXCreateCubeTextureFromFileA(	drv, texFileName, &tex);

				if ( r != S_OK )
				{
					throw Base::Errors::Simple(str::stringize("Failed to load cube texture from", std::quoted(texFileName)));
				}

				return tex;
			}
			else
			{
				throw Base::Errors::Simple(str::stringize(__FUNCTION__": file not exist ", std::quoted(texFileName)));
			}
			return 0;
		}


		bool SaveTextureDDS( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture )
		{
			Base::FileUtils::create_dir_for_file(fileName);
			return S_OK == D3DXSaveTextureToFileA( fileName, D3DXIFF_DDS, pTexture, 0 );
		}

		bool SaveTextureJPG( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture )
		{
			Base::FileUtils::create_dir_for_file(fileName);
			return S_OK == D3DXSaveTextureToFileA( fileName, D3DXIFF_JPG, pTexture, 0 );
		}

		bool SaveTexturePNG( const char* fileName,  LPDIRECT3DBASETEXTURE9 pTexture )
		{
			Base::FileUtils::create_dir_for_file(fileName);
			return S_OK == D3DXSaveTextureToFileA( fileName, D3DXIFF_PNG, pTexture, 0 );
		}

		VOID WINAPI D3dColorFill(D3DXVECTOR4* pOut, const D3DXVECTOR2* , const D3DXVECTOR2* , LPVOID pData)
		{
			D3DCOLORVALUE* pColor = (D3DCOLORVALUE*)pData;
			*pOut = D3DXVECTOR4(pColor->r, pColor->g, pColor->b, pColor->a);
		}

		void CreateSolidCubeMap( LPDIRECT3DCUBETEXTURE9 *ppTexture, int size, unsigned int color[6])
		{
			IDirect3DDevice9* device = ::dx9::device;
			D3DXCreateCubeTexture(device, size, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, ppTexture);

			for(int i = D3DCUBEMAP_FACE_POSITIVE_X; i <= D3DCUBEMAP_FACE_NEGATIVE_Z; ++i)
			{
				D3DLOCKED_RECT lockedRect;
				(*ppTexture)->LockRect((D3DCUBEMAP_FACES)i, 0, &lockedRect, 0, 0);
				int count = lockedRect.Pitch * size / sizeof(int);
				int* data = (int*)lockedRect.pBits;

				int c = color[i];
				for (int j = 0; j < count; ++j)
				{
					data[j] = c;
				}
				(*ppTexture)->UnlockRect((D3DCUBEMAP_FACES)i, 0);
			}					
		}

		void FlipTextureY(LPDIRECT3DTEXTURE9 tex)
		{
			nm::index2 size = GetTextureSize(tex);

			D3DLOCKED_RECT rect; 
			tex->LockRect(0, &rect, 0, 0);

			std::vector< std::vector<char> > data;
			data.resize(size.y);

			char* texData = (char*)rect.pBits;
			for(int i = size.y - 1; i >= 0; --i)
			{
				data[i].resize(rect.Pitch);
				mem::memcpy(&data[i][0], texData, rect.Pitch);
				texData += rect.Pitch;
			}

			texData = (char*)rect.pBits;
			for(int i = 0; i < size.y; ++i)
			{
				mem::memcpy(texData, &data[i][0], rect.Pitch);
				texData += rect.Pitch;
			}

			tex->UnlockRect(0);	
		}


		LPDIRECT3DTEXTURE9 CreateTextureARGB( const nm::index2& size)
		{
			LPDIRECT3DTEXTURE9 texture;
			D3DXCreateTexture(::dx9::device, size.x, size.y, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture);
			return texture;
		}

		LPDIRECT3DTEXTURE9 CreateTextureABGR32F(const nm::index2& size, int mipsCount)
		{
			LPDIRECT3DTEXTURE9 texture;
			D3DXCreateTexture(::dx9::device, size.x, size.y, 0, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &texture);
			return texture;
		}

		void CreateSolidTexture( LPDIRECT3DTEXTURE9 *ppTexture, const nm::index2& size, unsigned int color)
		{
			IDirect3DDevice9* device = ::dx9::device;
			D3DCOLORVALUE d3dColor;
			d3dColor.r = float((color & 0xff000000) >> 24) / 255.0f;
			d3dColor.g = float((color & 0x00ff0000) >> 16) / 255.0f;
			d3dColor.b = float((color & 0x0000ff00) >> 8) / 255.0f;
			d3dColor.a = float((color & 0x000000ff) >> 0) / 255.0f;

			bool bSuccess = true;
			bSuccess &= D3DXCreateTexture(device, size.x, size.y, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture) == S_OK;
			bSuccess &= D3DXFillTexture(*ppTexture, D3dColorFill, &d3dColor) == S_OK;
		}	

		void FillSolidTexture( LPDIRECT3DTEXTURE9 pTexture, unsigned int color)
		{
			D3DCOLORVALUE d3dColor;
			d3dColor.r = float((color & 0xff000000) >> 24) / 255.0f;
			d3dColor.g = float((color & 0x00ff0000) >> 16) / 255.0f;
			d3dColor.b = float((color & 0x0000ff00) >> 8) / 255.0f;
			d3dColor.a = float((color & 0x000000ff) >> 0) / 255.0f;
			D3DXFillTexture(pTexture, D3dColorFill, &d3dColor);
		}

		LPDIRECT3DTEXTURE9 CreateDummyTexture(D3DFORMAT fmt)
		{
			LPDIRECT3DTEXTURE9 texture;
			D3DXCreateTexture(::dx9::device, 1, 1, 0, 0, fmt, D3DPOOL_DEFAULT, &texture);
			return texture;
		}

		nm::index2 GetTextureSize(LPDIRECT3DTEXTURE9 tex)
		{
			D3DSURFACE_DESC surfaceDesc;
			tex->GetLevelDesc(0, &surfaceDesc);
			return nm::index2(surfaceDesc.Width, surfaceDesc.Height);
		}

		void GetTexelSize(LPDIRECT3DTEXTURE9 tex, float& w, float& h)
		{
			D3DSURFACE_DESC surfaceDesc;
			tex->GetLevelDesc(0, &surfaceDesc);
			w = 1.0f / surfaceDesc.Width;
			h = 1.0f / surfaceDesc.Height;
		}

		bool IsPow2Texture(LPDIRECT3DTEXTURE9 tex, nm::index2* size)
		{
			nm::index2 sz = GetTextureSize(tex);
			if(size) *size = sz;
			return Base::MathUtils::LogInt2(sz.x) > 0 && Base::MathUtils::LogInt2(sz.y) > 0;
		}

		D3DFORMAT GetTextureFormat(LPDIRECT3DTEXTURE9 tex)
		{
			D3DSURFACE_DESC surfaceDesc;
			tex->GetLevelDesc(0, &surfaceDesc);
			return surfaceDesc.Format;
		}

		void SaveD3DBufferToStream(stream::ostream& s, LPD3DXBUFFER buf)
		{
			s << buf->GetBufferSize();
			s.write( buf->GetBufferPointer(), buf->GetBufferSize() );
		}

		void LoadD3DBufferFromStream(stream::istream& s, LPD3DXBUFFER* buf)
		{
			DWORD size;	s >> size;
			D3DXCreateBuffer(size, buf);
			LPVOID pdata = (*buf)->GetBufferPointer();
			s.read(pdata, size);
		}

		void SaveD3DBufferToFile(const std::string& fileName, LPD3DXBUFFER buf)
		{
			stream::ofstream<> os( fileName.c_str() );
			SaveD3DBufferToStream(os, buf);
		}

		void LoadD3DBufferFromFile(const std::string& fileName, LPD3DXBUFFER* buf)
		{
			stream::ifstream<> is(fileName.c_str());
			LoadD3DBufferFromStream(is, buf);
		}


	}
}

#endif



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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