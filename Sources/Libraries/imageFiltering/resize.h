// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "algorithm/mitchellNetravali.h"
#include "newmath/library.include.h" 
#include "image/library.include.h"

namespace image 
{
	namespace filter 
	{

		template<image::Format format>
		image::Color<format> interpolate(float x, const image::Color<format>& y0, const image::Color<format>& y1, const image::Color<format>& y2, const image::Color<format>& y3)
		{
			float c0 = mitchellNetravali(x - 1);
			float c1 = mitchellNetravali(x);
			float c2 = mitchellNetravali(x + 1);
			float c3 = mitchellNetravali(x + 2);

			return c0*y0 + c1*y1 + c2*y2 + c3*y3;
		}

		template<image::Format format0, image::Format format1>
		void resize_n(const image::Plane<image::Color<format0>>& src_img, image::Plane<image::Color<format1>>& dst_img)
		{
			using namespace nm;

			int src_width = src_img.sizeX;
			int src_height = src_img.sizeY;
			int dst_width = dst_img.sizeX;
			int dst_height = dst_img.sizeY;

			if ((src_width == dst_width) && (src_height == dst_height))
			{
				dst_img.copy(src_img);
			}

			float idw = 1.0f / (float) dst_width;
			float idh = 1.0f / (float) dst_height;
			float xscale = src_width * idw;
			float yscale = src_height * idh;

			for (int j = 0; j < dst_height; ++j)
			{
				for (int i = 0; i < dst_width; ++i)
				{
					int src_i = (int) floor(i * xscale);
					int src_j = (int) floor(j * yscale);
					int i0 = min(max(0, src_i - 1), src_width - 1);
					int i1 = min(max(0, src_i), src_width - 1);
					int i2 = min(max(0, src_i + 1), src_width - 1);
					int i3 = min(max(0, src_i + 2), src_width - 1);
					int j0 = min(max(0, src_j - 1), src_height - 1);
					int j1 = min(max(0, src_j), src_height - 1);
					int j2 = min(max(0, src_j + 1), src_height - 1);
					int j3 = min(max(0, src_j + 2), src_height - 1);

					image::Color<format1> t0 = interpolate(i * idw, src_img(i0, j0), src_img(i1, j0), src_img(i2, j0), src_img(i3, j0));
					image::Color<format1> t1 = interpolate(i * idw, src_img(i0, j1), src_img(i1, j1), src_img(i2, j1), src_img(i3, j1));
					image::Color<format1> t2 = interpolate(i * idw, src_img(i0, j2), src_img(i1, j2), src_img(i2, j2), src_img(i3, j2));
					image::Color<format1> t3 = interpolate(i * idw, src_img(i0, j3), src_img(i1, j3), src_img(i2, j3), src_img(i3, j3));
					dst_img(i, j) = interpolate(j * idh, t0, t1, t2, t3);
				}
			}
		}
	}
}



// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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