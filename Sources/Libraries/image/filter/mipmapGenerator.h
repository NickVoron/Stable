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
					//float r = mipColor.r();
					//float g = mipColor.g();
					//float b = mipColor.b();

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


	}//
}//