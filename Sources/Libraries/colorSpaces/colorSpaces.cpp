// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "colorSpaces.h"
#include <math.h>

namespace image
{

	void hsv_to_rgb(float hue, float saturation, float value, float& r, float& g, float& b)
	{
		int d0;
		float e0, a0, b0, c0;
		hue = fmodf(hue, 360);
		if (hue < 0) hue += 360;
		d0 = hue / 60;
		e0 = hue / 60 - d0;
		a0 = value * (1.0f - saturation);
		b0 = value * (1.0f - e0 * saturation);
		c0 = value * (1.0f - (1.0f - e0) * saturation);
		switch (d0) 
		{
		default:
		case 0: r = value, g = c0,     b = a0;     return;
		case 1: r = b0,    g = value,  b = a0;     return;
		case 2: r = a0,    g = value,  b = c0;     return;
		case 3: r = a0,    g = b0,     b = value;  return;
		case 4: r = c0,    g = a0,     b = value;  return;
		case 5: r = value, g = a0,     b = b0;     return;
		}
	}

	void rgb_to_hsv(float r, float g, float b, float& hue, float& saturation, float& value)
	{
		float a0, b0, c0, d0;
		if (r > g)
		{
			if (r > b)
			{
				a0 = r;	b0 = g - b;	d0 = 0;	c0 = (g > b) ? b : g;
			}
			else
			{
				a0 = b; b0 = r - g; c0 = g; d0 = 4;
			}
		}
		else if (r > b)
		{
			a0 = g;	b0 = b - r;	c0 = b;	d0 = 2;
		}
		else if (g > b)
		{
			a0 = g;	b0 = b - r;c0 = r; d0 = 2;
		}
		else
		{
			a0 = b; b0 = r - g; c0 = r; d0 = 4;
		}

		if (a0 == c0)
		{
			hue = 0;
		}
		else
		{
			hue = 60 * (d0 + b0 / (a0 - c0));
			if (hue < 0)	hue += 360;
			if (hue > 360)	hue -= 360;
		}

		saturation = (a0 == 0) ? 0 : (a0 - c0) / a0;
		value = a0;
	}

	
	
	
	inline float helper(float x, float a0, float b0)
	{
		if (x < 0) x += 1;
		if (x > 1) x -= 1;

		if (x * 6 < 1) return b0 + (a0 - b0) * 6 * x;
		if (x * 6 < 3) return a0;
		if (x * 6 < 4) return b0 + (a0 - b0) * (4.0f - 6.0f * x);
		return b0;
	}

	void hsl_to_rgb(float hue, float saturation, float lightness, float& r, float& g, float& b)
	{
		float a0, b0, h;
		hue = fmod(hue, 360);
		if (hue < 0) hue += 360;
		h = hue / 360;
		if (lightness < 0.5f)
			a0 = lightness + lightness * saturation;
		else
			a0 = lightness + saturation - lightness * saturation;
		b0 = lightness * 2 - a0;
		r	= helper(h + 1.0f / 3.0f, a0, b0);
		g	= helper(h, a0, b0);
		b	= helper(h - 1.0f / 3.0f, a0, b0);
	}

	void rgb_to_hsl(float r, float g, float b, float& hue, float& saturation, float& lightness)
	{
		float a0, b0, c0, d0;
		if (r > g)
		{
			if (r > b)
				if (g > b)
					a0 = r, b0 = g - b, c0 = b, d0 = 0;
				else
					a0 = r, b0 = g - b, c0 = g, d0 = 0;
			else
				a0 = b, b0 = r - g, c0 = g, d0 = 4;
		}
		else
		{
			if (r > b)
				a0 = g, b0 = b - r, c0 = b, d0 = 2;
			else
				if (g > b)
					a0 = g, b0 = b - r, c0 = r, d0 = 2;
				else
					a0 = b, b0 = r - g, c0 = r, d0 = 4;
		}

		hue = (a0 == c0) ? 0 : 60 * (d0 + b0 / (a0 - c0));

		if (hue < 0) hue += 360;

		saturation = (a0 == c0) ? 0 : ( (a0 + c0 < 1) ? ((a0 - c0) / (a0 + c0)) : ((a0 - c0) / (2 - a0 - c0)) );
		lightness = (a0 + c0) / 2;
	}


	
	
	
	void cmyk_to_rgb(float cyan, float magenta, float yellow, float key, float& r, float& g, float& b)
	{
		float max = 1 - key;
		r	= max - cyan	* max;
		g	= max - magenta * max;
		b	= max - yellow	* max;
	}

	void rgb_to_cmyk(float r, float g, float b, float& cyan, float& magenta, float& yellow, float& key)
	{
		float max = r;
		if (g > max) max = g;
		if (b > max) max = b;
		key = 1 - max;
		if (max > 0) {
			cyan = (max - r) / max;
			magenta = (max - g) / max;
			yellow = (max - b) / max;
		}
		else {
			cyan = magenta = yellow = 0;
		}
	}

	
	
	
	void yuv_to_rgb(float y, float u, float v, float& r, float& g, float& b)
	{
		
		u = 0.436f * (u * 2 - 1);
		v = 0.615f * (v * 2 - 1);
		r		= y + v * 1.13983f;
		g	= y + u * -0.39465f + v * -0.58060f;
		b		= y + u * 2.03211f;
	}

	void rgb_to_yuv(float r, float g, float b, float& y, float& u, float& v)
	{
		y = r * 0.299f		+ g * 0.587f	+ b * 0.114f;
		u = r * -0.14713f	+ g * -0.28886f	+ b * 0.436f;
		v = r * 0.615f		+ g * -0.51499f	+ b * -0.10001f;
		
		u = (u / 0.436f + 1) * 0.5f;
		v = (v / 0.615f + 1) * 0.5f;
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