// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "kernel2.h"


namespace image
{
	namespace filter
	{
		template<unsigned int windowSize>
		void downsample_krnl(kernel2<windowSize>& krn)
		{
			for (int i = 0; i < windowSize; ++i)
			{
				for (int j = 0; j < windowSize; ++j)
				{
					krn.data[i][j] = 1.0f;
				}
			}

			

			krn.normalize();
		}

		template<class CT0, class CT1>
		void downsample(const Plane<CT0>& src, Plane<CT1>& dst, int dstWidth, int dstHeight, WrapMode wm)
		{
			kernel2<3> krnl; downsample_krnl(krnl);
			apply_kernel2(src, dst, dstWidth, dstHeight, krnl, wm);
		}

		template<class CT0, class CT1>
		void downsample1(const Plane<CT0>& src, Plane<CT1>& dst, int dstWidth, int dstHeight, WrapMode wm)
		{
			kernel2<1> krnl; downsample_krnl(krnl);
			apply_kernel2(src, dst, dstWidth, dstHeight, krnl, wm);
		}
	}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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