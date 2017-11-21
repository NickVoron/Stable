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
			//sk.normalize();
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

					//CT1 dest = rgba<ARGB32F>((nx+1.0f)*0.5f, (nz+1.0f)*0.5f, (ny+1.0f)*0.5f, 1.0f);
					
					dst(i, j) = rgba<ARGB8>(diff, diff, diff, 1.0f);	
				}
			}
		}

	}
}