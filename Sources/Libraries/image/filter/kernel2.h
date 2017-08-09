// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../color.h"
#include "../plane.h"


namespace image
{
	namespace filter
	{
		template<unsigned int windowSize>
		struct kernel2
		{
			void transpose()
			{
				for(unsigned int i = 0; i < windowSize - 1; ++i)
				{
					for(unsigned int j = i + 1; j < windowSize; ++j)
					{
						std::swap(data[i][j], data[j][i]);
					}
				}
			}

			void normalize()
			{
				float total = 0.0f;
				for(unsigned int i = 0; i < windowSize; i++)
				{
					for(unsigned int j = 0; j < windowSize; j++)
					{
						total += data[i][j];
					}
				}

				float inv = 1.0f / total;
				for(unsigned int i = 0; i < windowSize; i++)
				{
					for(unsigned int j = 0; j < windowSize; j++)
					{
						data[i][j] *= inv;
					}
				}
			}

			float data[windowSize][windowSize];

			void load(const float data_[windowSize][windowSize]) 
			{
				for (unsigned int i = 0; i < windowSize; i++)
				{
					for (unsigned int j = 0; j < windowSize; j++)
					{
						data[i][j] = data_[i][j];
					}
				}

			}
		};

		template<class kernelHelper>
		struct kernel2Ext: public kernel2<kernelHelper::size>
		{
			kernel2Ext()
			{
				load(kernelHelper::data());
			}
		};



		template<class ColorType, unsigned int kernelWindowSize>
		ColorType evaluate_kernel2(const Plane<ColorType>& src, const kernel2<kernelWindowSize>& krnl, int x, int y, WrapMode wm)
		{
			const int kernelOffset = (kernelWindowSize / 2) - 1;

			Color<ARGB32F> res; res.zero();

			for (unsigned int i = 0; i < kernelWindowSize; i++)
			{
				const int src_y = int(y + i) - kernelOffset;

				for (unsigned int e = 0; e < kernelWindowSize; e++)
				{
					const int src_x = int(x + e) - kernelOffset;

					res += src.index(src_x, src_y, wm) * krnl.data[e][i];
				}
			}

			ColorType r = res;
			return r;
		}

		template<class ColorType, unsigned int kernelWindowSize>
		void apply_kernel2(const Plane<ColorType>& src, Plane<ColorType>& dst, const kernel2<kernelWindowSize>& krnl, WrapMode wm)
		{
			dst.clear();
			dst.allocate(src.sizeX, src.sizeY);

			for (unsigned int j = 0; j < dst.sizeY; ++j)
			{
				for (unsigned int i = 0; i < dst.sizeX; ++i)
				{
					dst(i, j) = evaluate_kernel2(src, krnl, i, j, wm);
					dst(i, j).a() = 1.0f;
				}
			}
		}

		template<class ColorType, unsigned int kernelWindowSize>
		void apply_kernel2(const Plane<ColorType>& src, Plane<ColorType>& dst, unsigned int dstWidth, unsigned int dstHeight, const kernel2<kernelWindowSize>& krnl, WrapMode wm)
		{
			dst.clear();
			dst.allocate(dstWidth, dstHeight);

			float rateX = (float)src.sizeX / dst.sizeX;
			float rateY = (float)src.sizeY / dst.sizeY;











			for (unsigned int j = 0; j < dstHeight; ++j)
			{
				for (unsigned int i = 0; i < dstWidth; ++i)
				{
					int k = static_cast<int>( i * rateX );
					int m = static_cast<int>( j * rateY );

					dst(i, j) = evaluate_kernel2(src, krnl, k, m, wm);
				}
			}
		}

	}
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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