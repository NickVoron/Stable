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

		// Compute outside = edtaa3(bitmap); % Transform background (0's)
		computegradient(data, width, height, gx, gy);
		edtaa3(data, gx, gy, width, height, xdist, ydist, outside);
		for (unsigned int i = 0; i < width*height; ++i)
		{
			if (outside[i] < 0.0)
			{
				outside[i] = 0.0;
			}
		}

		// Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
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

		// distmap = outside - inside; % Bipolar distance field
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
