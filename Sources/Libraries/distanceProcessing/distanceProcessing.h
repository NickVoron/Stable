// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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