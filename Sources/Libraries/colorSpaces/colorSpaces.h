// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

namespace image
{
	void hsv_to_rgb(float hue, float saturation, float value, float& red, float& green, float& blue);
	void rgb_to_hsv(float red, float green, float blue, float& hue, float& saturation, float& value);

	void hsl_to_rgb(float hue, float saturation, float lightness, float& red, float& green, float& blue);
	void rgb_to_hsl(float red, float green, float blue, float& hue, float& saturation, float& lightness);

	void cmyk_to_rgb(float cyan, float magenta, float yellow, float key, float& red, float& green, float& blue);
	void rgb_to_cmyk(float red, float green, float blue, float& cyan, float& magenta, float& yellow, float& key);

	void yuv_to_rgb(float y, float u, float v, float& red, float& green, float& blue);
	void rgb_to_yuv(float red, float green, float blue, float& y, float& u, float& v);
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