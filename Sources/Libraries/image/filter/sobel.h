// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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
		template<int windowSize>
		void sobelX(kernel2<windowSize>& krn)
		{
			ENFORCE(windowSize >= 3 && windowSize % 2 == 1);

			int halfSize = windowSize / 2;

			for (int i = 0; i < windowSize; ++i)
			{
				krn.data[halfSize][i] = 0.0f;
			}

			for (int i = 0; i < halfSize; ++i)
			{
				int a = 1;
				for (int j = 0; j < windowSize; ++j)
				{		  
					krn.data[i][j] = (float)-(a + i);

					a += (j < halfSize) ? 1 : -1;
				}		
			}	

			for (int i = halfSize + 1; i < windowSize; ++i)
			{
				int a = 1;
				for (int j = 0; j < windowSize; ++j)
				{		  
					krn.data[i][j] = (float)( halfSize + a + (halfSize - i));

					a += (j < halfSize) ? 1 : -1;
				}		
			}
		}

		template<int windowSize>
		void sobelY(kernel2<windowSize>& krn)
		{
			sobelX(krn);
			krn.transpose();
		}


		template<int kernelWindowSize, class CT0, class CT1>
		void sobel(const Plane<CT0>& src, Plane<CT1>& dst, WrapMode wm)
		{
			Plane< Color<ARGB32F> >* srcFloat = 0;
			Plane< Color<ARGB32F> > srct;

			allocate_plane_image(srct, src.sizeX, src.sizeY);
			srcFloat = &srct;
			srcFloat->copy(src);
	
			ENFORCE(srcFloat);

			Plane< Color<ARGB32F> > t0;
			Plane< Color<ARGB32F> > t1;
			kernel2<kernelWindowSize> sk;

			sobelX(sk);	
			apply_kernel2(*srcFloat, t0, sk, wm);

			sk.transpose(); 
			apply_kernel2(*srcFloat, t1, sk, wm);

			dst.clear();
			allocate_plane_image(dst, src.sizeX, src.sizeY); 			

			for (int j = 0; j < dst.sizeY; ++j)
			{
				for (int i = 0; i < dst.sizeX; ++i)
				{
					const Color<ARGB32F>& dc = t1(i, j);
					const Color<ARGB32F>& tc = t0(i, j);

					Color<ARGB32F> rc = (dc*dc + tc*tc);
					rc.sqrtc();
					rc.a() = 1.0f;
					dst(i, j) = rc;
				}
			}
		}


		template<int kernelWindowSize, class CT0, class CT1>
		void sobel_gradient(const Plane<CT0>& src, Plane<CT1>& dst, WrapMode wm)
		{
			Plane< Color<ARGB32F> > srct;

			srct.copy(src);

			Plane< Color<ARGB32F> > t0;
			Plane< Color<ARGB32F> > t1;
			kernel2<kernelWindowSize> sk;

			sobelX(sk);	
			
			apply_kernel2(srct, t0, sk, wm);

			sk.transpose(); 
			apply_kernel2(srct, t1, sk, wm);

			dst.clear();
			dst.allocate(src.sizeX, src.sizeY);
			
			for (int j = 0; j < dst.sizeY; ++j)
			{
				for (int i = 0; i < dst.sizeX; ++i)
				{
					const Color<ARGB32F>& dc = t1(i, j);
					const Color<ARGB32F>& tc = t0(i, j);

					float nx = tc.r();
					float ny = dc.r();
					float nz = 2.0f * sqrt( 1.0f - nx * nx - ny * ny );

					nm::Vector3 n(nx, nz, ny);
					nm::normalize(n);
					nm::Vector3 l(1.0, 1.0f, 1.0);
					nm::normalize(l);

					float diff = nm::clamp((nm::dot(n, l)), 0.0f, 1.0f) + 0.1f;
					diff = nm::clamp(diff, 0.0f, 1.0f);

					
					
					dst(i, j) = rgba<ARGB8>(diff, diff, diff, 1.0f);	
				}
			}
		}

	}
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