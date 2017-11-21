#pragma once

#include "stuff/enforce.h"
#include "libfreeimage/library.include.h"
#include "stream/library.include.h"

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
			//retrieve the image data
			BYTE* bits = FreeImage_GetBits(bitmap);
			//get the image width and height
			unsigned int width = FreeImage_GetWidth(bitmap);
			unsigned int height = FreeImage_GetHeight(bitmap);
			//FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(bitmap);
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
							
						//добавление еденичной альфы при загрузки изображений без неё
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