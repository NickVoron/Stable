// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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





// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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