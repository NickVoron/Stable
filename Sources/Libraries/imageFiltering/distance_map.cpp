// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "distance_map.h"
#include "edtaa3func.h"

#pragma once

namespace image 
{
namespace filter 
{
	void distance_map(double *data, unsigned int width, unsigned int height)
	{
		short * xdist = (short *) malloc(width * height * sizeof(short));
		short * ydist = (short *) malloc(width * height * sizeof(short));
		double * gx = (double *) calloc(width * height, sizeof(double));
		double * gy = (double *) calloc(width * height, sizeof(double));
		double * outside = (double *) calloc(width * height, sizeof(double));
		double * inside = (double *) calloc(width * height, sizeof(double));

		
		computegradient(data, width, height, gx, gy);
		edtaa3(data, gx, gy, width, height, xdist, ydist, outside);
		for (unsigned int i = 0; i < width*height; ++i)
		{
			if (outside[i] < 0.0)
			{
				outside[i] = 0.0;
			}
		}

		
		memset(gx, 0, sizeof(double)*width*height);
		memset(gy, 0, sizeof(double)*width*height);
		for (unsigned int i = 0; i < width*height; ++i)
			data[i] = 1 - data[i];
		computegradient(data, width, height, gx, gy);
		edtaa3(data, gx, gy, width, height, xdist, ydist, inside);
		for (unsigned int i = 0; i < width*height; ++i)
		{
			if (inside[i] < 0)
			{
				inside[i] = 0.0;
			}
		}

		
		double vmin = +INFINITY;
		for (unsigned int i = 0; i < width*height; ++i)
		{
			outside[i] -= inside[i];
			if (outside[i] < vmin)
			{
				vmin = outside[i];
			}
		}
		vmin = fabs(vmin);
		for (unsigned int i = 0; i < width*height; ++i)
		{
			double v = outside[i];
			if (v < -vmin) outside[i] = -vmin;
			else if (v > +vmin) outside[i] = +vmin;
			data[i] = (outside[i] + vmin) / (2 * vmin);
		}

		free(xdist);
		free(ydist);
		free(gx);
		free(gy);
		free(outside);
		free(inside);
	}
}
}




// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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