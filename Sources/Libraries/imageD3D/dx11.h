// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#ifdef WIN32

#include <d3d11.h>

#include "image/library.include.h"


#include "freeimage.h"

namespace image
{
namespace dx11
{

	template<Format fmt>		struct image_to_d3d;
	template<DXGI_FORMAT fmt>	struct d3d_to_image;

	template<> struct image_to_d3d<RGBA8>						{ static const Format imageFmt = RGBA8;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R8G8B8A8_UNORM; };
	template<> struct image_to_d3d<R32F>						{ static const Format imageFmt = R32F;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R32_FLOAT; };
	template<> struct image_to_d3d<RGBA32F>						{ static const Format imageFmt = RGBA32F;	static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R32G32B32A32_FLOAT; };
	template<> struct image_to_d3d<R8>							{ static const Format imageFmt = R8;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R8_UNORM; };
	template<> struct image_to_d3d<R16>							{ static const Format imageFmt = R16;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R16_UNORM; };
	template<> struct image_to_d3d<R16F>						{ static const Format imageFmt = R16F;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R16_FLOAT; };

	template<> struct d3d_to_image<DXGI_FORMAT_R8G8B8A8_UNORM>		{ static const Format imageFmt = RGBA8;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R8G8B8A8_UNORM; };
	template<> struct d3d_to_image<DXGI_FORMAT_R32_FLOAT>			{ static const Format imageFmt = R32F;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R32_FLOAT; };
	template<> struct d3d_to_image<DXGI_FORMAT_R32G32B32A32_FLOAT>	{ static const Format imageFmt = RGBA32F;	static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R32G32B32A32_FLOAT; };
	template<> struct d3d_to_image<DXGI_FORMAT_R8_UNORM>			{ static const Format imageFmt = R8;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R8_UNORM; };
	template<> struct d3d_to_image<DXGI_FORMAT_R16_UNORM>			{ static const Format imageFmt = R16;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R16_UNORM; };
	template<> struct d3d_to_image<DXGI_FORMAT_R16_FLOAT>			{ static const Format imageFmt = R16F;		static const DXGI_FORMAT d3dFmt = DXGI_FORMAT_R16_FLOAT; };
	
	
	template<D3DFORMAT fmt> struct d3d_pixel_size {	static const unsigned int value = pixel_info< d3d_to_image<fmt>::imageFmt >::value;	};
	template<D3DFORMAT fmt> struct d3d_to_image_color {	typedef Color< d3d_to_image<fmt>::imageFmt > Color; };

	template<D3DFORMAT fmt> struct d3d_to_image_line	{ typedef Line		<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_plane	{ typedef Plane		<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_volume	{ typedef Volume	<typename d3d_to_image_color<fmt>::Color> Result; };
	template<D3DFORMAT fmt> struct d3d_to_image_cubemap	{ typedef Cubemap	<typename d3d_to_image_color<fmt>::Color> Result; };

	
	
	

	template<Format fmt>
	void createTexture(ID3D11Device* device, const Plane< Color<fmt> >& plane, ID3D11Texture2D** texture)
	{
		ENFORCE(device);
		ENFORCE(texture);

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = plane.sizeX;
		desc.Height = plane.sizeY;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = image_to_d3d<fmt>::d3dFmt;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = plane.bmp;
		data.SysMemPitch = plane.pitchInBytes;
		data.SysMemSlicePitch = 0;

		
		device->CreateTexture2D(&desc, &data, texture);
	}


	template<Format fmt>
	int createTextureWithMip(ID3D11Device* device, const Plane< Color<fmt> >& plane, ID3D11Texture2D** texture, int mipsCount)
	{
		ENFORCE(device);
		ENFORCE(texture);


		std::vector<Plane<Color<fmt>>> mips;
		
		
		
		int countMips = filter::generateMipLevels(plane, mips, mipsCount);

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = plane.sizeX;
		desc.Height = plane.sizeY;
		desc.MipLevels = countMips;
		desc.ArraySize = 1;
		desc.Format = image_to_d3d<fmt>::d3dFmt;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA* data = new D3D11_SUBRESOURCE_DATA[countMips];
		for (int i = 0; i < countMips; i++)
		{
			Plane<Color<fmt>>& p = mips[i];
			data[i].pSysMem = p.bmp;
			data[i].SysMemPitch = p.pitchInBytes;
			data[i].SysMemSlicePitch = 0;

		}
		
		device->CreateTexture2D( &desc, &data[0], texture);

		return countMips;
	}


	  

	
	
	
	
	
	
	template<Format fmt>
	int createTexture(ID3D11Device* device, const Cubemap< Color<fmt> >& cubemap, ID3D11Texture2D** texture)
	{
		ENFORCE(device);
		ENFORCE(texture);

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = cubemap.size;
		desc.Height = cubemap.size;
		desc.MipLevels = 1;
		desc.ArraySize = 6;
		desc.Format = image_to_d3d<fmt>::d3dFmt;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		D3D11_SUBRESOURCE_DATA data[6];
		for (int i = POSITIVE_X; i <= NEGATIVE_Z; ++i)
		{
			data[i].pSysMem = cubemap.planes[i];
			data[i].SysMemPitch = cubemap.pitchInBytes;
			data[i].SysMemSlicePitch = 0;
		}

		device->CreateTexture2D(&desc, &data[0], texture);

		return 1;
	}



	template<Format fmt>
	void loadTexture(ID3D11Device* device, const char* fileName, ID3D11Texture2D** texture)
	{
		Plane< Color<fmt> > plane;
		image::freeimage::load(fileName, plane);
		createTexture(device, plane, texture);
		plane.clear();
	}

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