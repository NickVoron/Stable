// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "color.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "image/library.include.h"

namespace imgui
{

	template<image::Format format>
	void color_rgba(const char* caption, image::Color<format>& c)
	{
		image::Color<image::RGBA32F> oc = c;
		color_rgba(caption, oc.r(), oc.g(), oc.b(), oc.a());
		c = oc;
	}

	template<image::Format format>
	void color_rgba(image::Color<format>& c)
	{
		color_rgba("", c);
	}

	void color_d3d(const char* caption, D3DCOLOR& c);
	void color_d3d(const char* caption, D3DXCOLOR& c);
	void color_d3d(D3DCOLOR& c);
	void color_d3d(D3DXCOLOR& c);

	void color(const char* caption, Vector4& c);
	void color(const char* caption, Vector3& c);
	void color(Vector4& c);
	void color(Vector3& c);

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