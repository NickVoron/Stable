// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../types.h"

namespace imgui
{

	void panel_draw(const Rect& rect, bool dark);
	void panel_draw(float x, float y, float w, float h, bool dark);
	Rect panel_draw(const char* caption, const Rect& rect, bool dark);
	Rect panel_draw(const char* caption, float x, float y, float w, float h, bool dark);
	
	void panel_begin(const Rect& rect, bool visible);
	void panel_begin(float x, float y, float w, float h, bool visible);
	void panel_end();

	void vertical_panel_begin(const Rect& rect, bool visible);
	void vertical_panel_begin(float x, float y, float w, float h, bool visible);
	void vertical_panel_end();

	void scrollable_panel_begin(const Rect& rect);
	void scrollable_panel_end();

	void dragable_begin(const Rect& rect);
	void dragable_end();

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