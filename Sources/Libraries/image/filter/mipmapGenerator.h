// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "resize.h"
#include <vector>
#include <limits>
#include "colorMapping.h"



namespace image
{
	namespace filter
	{
		namespace p
		{
			template<class CT0, class CT1, int kernelWindowSize>
			int generateMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst, int count, const kernel2<kernelWindowSize>* krnl)
			{
				int l2X = (int)log2(src.sizeX);
				int l2Y = (int)log2(src.sizeY);
				if (count > l2X)
				{
					count = l2X - 1;
				}

				if (count > l2Y)
				{
					count = l2Y - 1;
				}

				int curSizeX = src.sizeX;
				int curSizeY = src.sizeY;

				dst.resize(count + 1);
				dst[0].copy(src);



				for (int i = 0; i < count; i++)
				{
					curSizeX = curSizeX / 2;
					curSizeY = curSizeY / 2;

					Plane<CT1>& resultMip = dst[i + 1];

					if (krnl)
					{
						Plane<CT1> mipLevel;
						mipLevel.allocate(curSizeX, curSizeY);
						downsample1(src, mipLevel, curSizeX, curSizeY, CLAMP);

						apply_kernel2(mipLevel, resultMip, curSizeX, curSizeY, *krnl, CLAMP);
					}
					else
					{
						downsample1(src, resultMip, curSizeX, curSizeY, CLAMP);
					}
				}

				return count + 1;
			}
		}


		template<class CT0, class CT1>
		int generateRainbowMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst, int count)
		{
			int l2X = (int)log2(src.sizeX);
			int l2Y = (int)log2(src.sizeY);
			if (count > l2X)
			{
				count = l2X - 1;
			}

			if (count > l2Y)
			{
				count = l2Y - 1;
			}

			int curSizeX = src.sizeX;
			int curSizeY = src.sizeY;

			dst.resize(count + 1);
			dst[0].copy(src);

			if (count)
			{
				mapping::RainbowGenerator generator;
				mapping::Map<int, CT1> colorMap = mapping::generate<CT1::FMT>(mapping::range(0, count), generator);

				for (int i = 0; i < count; i++)
				{
					curSizeX = curSizeX / 2;
					curSizeY = curSizeY / 2;

					Plane<CT1>& resultMip = dst[i + 1];
					resultMip.allocate(curSizeX, curSizeY);

					CT1 mipColor = colorMap[i];
					
					
					

					resultMip.fill(mipColor);
				}
			}
			return count + 1;
		}



		template<class CT0, class CT1>
		int generateMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst, int count)
		{
			return p::generateMipLevels<CT0, CT1, 1>(src, dst, count, nullptr);
		}

		template<class CT0, class CT1, int kernelWindowSize>
		int generateMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst, int count, const kernel2<kernelWindowSize>& krnl)
		{
			return p::generateMipLevels(src, dst, count, &krnl);
		}

		template<class CT0, class CT1>
		int generateMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst)
		{
			return p::generateMipLevels<CT0, CT1, 1>(src, dst, INT_MAX, nullptr);
		}

		template<class CT0, class CT1, int kernelWindowSize>
		int generateMipLevels(const Plane<CT0>& src, std::vector<Plane<CT1>>& dst, const kernel2<kernelWindowSize>& krnl)
		{
			return p::generateMipLevels(src, dst, INT_MAX, &krnl);
		}


	}
}//



// Copyright (C) 2015-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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