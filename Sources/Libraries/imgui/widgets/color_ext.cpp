// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "color_ext.h"

namespace imgui
{

#define RGBA_GETALPHA(rgb)      ((rgb) >> 24)
#define RGBA_GETRED(rgb)        (((rgb) >> 16) & 0xff)
#define RGBA_GETGREEN(rgb)      (((rgb) >> 8) & 0xff)
#define RGBA_GETBLUE(rgb)       ((rgb) & 0xff)

	void color(const char* caption, Vector4& c)		{	color_rgba(caption, c.x, c.y, c.z, c.w);	}
	void color(const char* caption, Vector3& c)		{	color_rgb(caption, c.x, c.y, c.z);			}

	void color(Vector4& c)	{	color("", c);	}
	void color(Vector3& c)	{	color("", c);	}

	void color_d3d(const char* caption, D3DXCOLOR& c)
	{
		color_rgba(caption, c.r, c.g, c.b, c.a);
	}

	void color_d3d(const char* caption, D3DCOLOR& c)
	{
		float r = RGBA_GETRED(c)	/ 255.0f;
		float g = RGBA_GETGREEN(c)	/ 255.0f;
		float b = RGBA_GETBLUE(c)	/ 255.0f;
		float a = RGBA_GETALPHA(c)	/ 255.0f;

		color_rgba(caption, r, g, b, a);

		c = D3DCOLOR_COLORVALUE(r, g, b, a); 
	}

	void color_d3d(D3DXCOLOR& c)
	{
		color_d3d("", c);
	}

	void color_d3d(D3DCOLOR& c)
	{
		color_d3d("", c);
	}

}//



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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