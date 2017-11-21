#pragma once
#include <string>
#include "imageFiltering/library.include.h"

namespace distanceProcessing
{

	template<image::Format fmt>
	void processTexture(const image::Plane<image::Color<fmt>>& res, image::Plane<image::Color<image::R32F>>& distMap)
	{
		image::Plane<image::Color<image::R32F>> resCopy;
		resCopy.copy(res);

		double* img = (double*)malloc(resCopy.sizeX*resCopy.sizeY*sizeof(double));

		for (int yy = 0; yy < resCopy.sizeY; ++yy)
		{
			for (int xx = 0; xx < resCopy.sizeX; ++xx)
			{
				double c = resCopy(xx, yy).r();
				img[xx + yy*resCopy.sizeX] = 1.0 - c;
				//LOG_MSG(c);
				//img[xx + yy*resCopy.sizeX] = resCopy(xx, yy).r();
			}
		}

		image::filter::distance_map(img, resCopy.sizeX, resCopy.sizeY);

		for (int yy = 0; yy < resCopy.sizeY; ++yy)
		{
			for (int xx = 0; xx < resCopy.sizeX; ++xx)
			{
				float  c = (float)img[xx + yy*resCopy.sizeX];
				resCopy(xx, yy).r() = c;
			}
		}

		distMap.copy(resCopy);
	}

}