// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "color.h"
#include "plane.h"


namespace image
{
	namespace filter
	{
		template<int windowSize>
		struct kernel2
		{
			void transpose()
			{
				for(int i = 0; i < windowSize - 1; ++i)
				{
					for(int j = i + 1; j < windowSize; ++j)
					{
						std::swap(data[i][j], data[j][i]);
					}
				}				
			}

			void normalize()
			{
				float total = 0.0f;
				for(int i = 0; i < windowSize; i++) 
				{
					for(int j = 0; j < windowSize; j++) 
					{
						total += fabs(data[i][j]);
					}
				}

				float inv = 1.0f / total;
				for(int i = 0; i < windowSize; i++) 
				{
					for(int j = 0; j < windowSize; j++) 
					{
						data[i][j] *= inv;
					}
				}
			}

			float data[windowSize][windowSize];
		};

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

		template<class ColorType, int kernelWindowSize>
		ColorType evaluate_kernel2(const Plane<ColorType>& src, const kernel2<kernelWindowSize>& krnl, int x, int y, WrapMode wm)
		{
			const int kernelOffset = (kernelWindowSize / 2) - 1;

			ColorType res; res.zero();

			for (int i = 0; i < kernelWindowSize; i++)
			{
				const int src_y = int(y + i) - kernelOffset;

				for (int e = 0; e < kernelWindowSize; e++)
				{
					const int src_x = int(x + e) - kernelOffset;

					res += src.index(src_x, src_y, wm) * krnl.data[e][i];
				}
			}

			return res;
		}

		template<class ColorType, int kernelWindowSize>
		void apply_kernel2(const Plane<ColorType>& src, Plane<ColorType>& dst, const kernel2<kernelWindowSize>& krnl, WrapMode wm)
		{
			dst.clear();
			allocate_plane_image(dst, src.sizeX, src.sizeY);
			
			for (int j = 0; j < dst.sizeY; ++j)
			{
				for (int i = 0; i < dst.sizeX; ++i)
				{
					dst(i, j) = evaluate_kernel2(src, krnl, i, j, wm);	
				}
			}
		}




		template<int kernelWindowSize, class CT0, class CT1>
		void sobel(const Plane<CT0>& src, Plane<CT1>& dst, WrapMode wm)
		{
			Plane< NewColor<ARGB32F> >* srcFloat = 0;
			Plane< NewColor<ARGB32F> > srct;

			allocate_plane_image(srct, src.sizeX, src.sizeY);
			srcFloat = &srct;
			srcFloat->copy(src);
	
			ENFORCE(srcFloat);

			Plane< NewColor<ARGB32F> > t0;
			Plane< NewColor<ARGB32F> > t1;
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
					const NewColor<ARGB32F>& dc = t1(i, j);
					const NewColor<ARGB32F>& tc = t0(i, j);

					NewColor<ARGB32F> rc = (dc*dc + tc*tc);
					
					rc *= 1.5f;
					rc.a() = 1.0f;

					CT1 dest = rc;

					dst(i, j) = dest;
				}
			}
		}


		template<int kernelWindowSize, class CT0, class CT1>
		void sobel_gradient(const Plane<CT0>& src, Plane<CT1>& dst, WrapMode wm)
		{
			Plane< NewColor<ARGB32F> >* srcFloat = 0;
			Plane< NewColor<ARGB32F> > srct;

			allocate_plane_image(srct, src.sizeX, src.sizeY);
			srcFloat = &srct;
			srcFloat->copy(src);

			ENFORCE(srcFloat);

			Plane< NewColor<ARGB32F> > t0;
			Plane< NewColor<ARGB32F> > t1;
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
					const NewColor<ARGB32F>& dc = t1(i, j);
					const NewColor<ARGB32F>& tc = t0(i, j);

					float nx = tc.r();
					float ny = dc.r();
					float nz = sqrt( 1.0f - nx * nx - ny * ny );

					nm::Vector3 n(nx, nz, ny);
					nm::normalize(n);
					nm::Vector3 l(1.0, 2.0f, 1.0);
					nm::normalize(l);

					float diff = nm::clamp((nm::dot(n, l))+0.1f, 0.0f, 0.99f);

					CT1 dest = rgba<ARGB32F>(diff, diff, diff, 1.0f);
					
					
					dst(i, j) = dest;	
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