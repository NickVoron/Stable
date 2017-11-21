#pragma once

#include "formats.h"
#include "swizzles.h"

namespace image
{
	struct ImageInfo 
	{
		ImageInfo();
		ImageInfo(Format f, unsigned int sX, unsigned int sY);
		ImageInfo(void* data, bool dataOwner, Format f, unsigned int sX, unsigned int sY, unsigned int pitch);
		~ImageInfo();


		void init(Format f, unsigned int sX, unsigned int sY, bool allocateInternalBuffer);
		void init(void* data, bool dataOwner, Format f, unsigned int sX, unsigned int sY, unsigned int pitch);

		void clear();
		void freemem();
		bool valid() const;

		Format format;
		unsigned int sizeX;
		unsigned int sizeY;
		unsigned int pitchInBytes;
		void* data;
		bool dataOwner;
	};

	bool alpha_channel_used(const ImageInfo& ii);

	swizzle_desc swizzle(Format f);

	void allocate_plane_image(ImageInfo& imgInfo);

	void convert_plane_image(const ImageInfo& srcInfo, ImageInfo& dstInfo);
}

