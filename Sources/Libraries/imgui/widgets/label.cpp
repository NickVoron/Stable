// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "label.h"
#include "../drawing/drawing.h"
#include "../controlTypes.h"
#include "../layout/layout.h"
#include "../layout/area.h"
#include "../layout/shift.h"
#include "../control.h"
#include "../globals.h"
#include "../imgui.h"

namespace imgui					
{
	void label(const Point2& pos, const char* text)
	{
		core::checked_begin();
		float h = 30.0f;
		detail::font->resource([&h](auto& f) { h = static_cast<float>(f.face.height); });
		Control ctrl(controls::LABEL, Rect(pos.x, pos.y, 100.0f, h));
		unigui::draw::print(text, ctrl.rect, detail::font, unigui::color(1.0f, 1.0f, 1.0f, 1.0f), true);
		core::checked_end();
	}

	void label (const Point2& pos, const std::string& text)
	{
		label(pos, text.c_str());
	}

	void label(const char* text)
	{
		label(Point2(0, 0), text);
	}

	void label(const std::string& text)
	{
		return label(text.c_str());
	}
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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