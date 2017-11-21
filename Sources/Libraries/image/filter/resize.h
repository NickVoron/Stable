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

			//krn.data[1][1] = 3.0f;

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