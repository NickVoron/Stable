// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/enforce.h"
#include "libfreeimage/library.include.h"

namespace image 
{
namespace freeimage 
{

	inline FIBITMAP* load(const char* filename)
	{
		return FreeImage_Load( FreeImage_GetFileType(filename), filename);
	}

	inline bool save(FIBITMAP* bitmap, const char* filename)
	{
		ENFORCE(bitmap);
		return FreeImage_Save(FIF_PNG, bitmap, filename) != FALSE;
	}

	template<image::Format fmt>
	bool convert(FIBITMAP* bitmap, image::Plane< image::Color<fmt> >& img)
	{
		if(bitmap)
		{
			
			BYTE* bits = FreeImage_GetBits(bitmap);
			
			unsigned int width = FreeImage_GetWidth(bitmap);
			unsigned int height = FreeImage_GetHeight(bitmap);
			
			FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);
			unsigned int bpp = FreeImage_GetBPP(bitmap);

			if(imageType == FIT_BITMAP)
			{
				switch (bpp)
				{
				case 32: 
					{
						image::Plane< image::Color<image::BGRA8> > rgba8;
						rgba8.init(bits, FreeImage_GetPitch(bitmap), width, height);
						img.copy(rgba8);
						return true;
					}
				case 24:
					{
						image::Plane< image::Color<image::BGR8> > rgb8;
						rgb8.init(bits, FreeImage_GetPitch(bitmap), width, height);
							
						
						image::Plane< image::Color<image::BGRA8> > rgba8;
						rgba8.allocate(width, height);
						rgba8.copy(rgb8);
						for (unsigned int x=0; x< width; x++)
						{
							for (unsigned int y=0; y< height; y++)
							{
								rgba8.line_ptr(y)[x].a() = 1.0f;
							}
						}

						img.copy(rgba8);
						rgba8.clear();

						return true;
					}
				case 8:
				{
					image::Plane< image::Color<image::R8> > r8;
					r8.init(bits, FreeImage_GetPitch(bitmap), width, height);
					img.copy(r8);
					return true;
				}
				}
			}
			else if(imageType == FIT_FLOAT)
			{
				INCOMPLETE;
				image::Plane< image::Color<image::R32F> > r32;
				r32.init(bits, FreeImage_GetPitch(bitmap), width, height);
				img.copy(r32);
				
				return true;
			}
			else if (imageType == FIT_UINT16)
			{
				image::Plane< image::Color<image::R16> > r16;
				r16.init(bits, FreeImage_GetPitch(bitmap), width, height);
				img.copy(r16);
				
				return true;
			}
		}

		return bitmap != 0;
	}

	template<image::Format fmt>
	FIBITMAP* convert(const image::Plane< image::Color<fmt> >& img)
	{
		FIBITMAP* bitmap = FreeImage_AllocateT(FIT_BITMAP, img.sizeX, img.sizeY, 32);
		BYTE* bits = FreeImage_GetBits(bitmap);

		image::Plane< image::Color<image::BGRA8> > rgba8;
		rgba8.copy(img);
		mem::memcpy(bits, rgba8.bmp, rgba8.memory());
		rgba8.clear();
		
		return bitmap;
	}


	template<image::Format fmt>
	bool load(const char* filename, image::Plane< image::Color<fmt> >& img)
	{
		FIBITMAP* bitmap = load(filename);

		if(bitmap)
		{
			convert(bitmap, img);

			FreeImage_Unload(bitmap);
		}

		return bitmap != 0;
	}

	template<image::Format fmt>
	bool save(const image::Plane< image::Color<fmt> >& img, const char* filename)
	{
		bool res = false;
		FIBITMAP* bitmap = convert(img);
		if (bitmap)
		{
			res = save(bitmap, filename);
			FreeImage_Unload(bitmap);
		}

		return res;		
	}


}
}



// Copyright (C) 2013-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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