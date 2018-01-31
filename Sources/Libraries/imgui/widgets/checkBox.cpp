// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "checkBox.h"
#include "../drawing/drawing.h"
#include "../types.h"
#include "../control.h"
#include "../controlTypes.h"
#include "../globals.h"
#include "../imgui.h"

namespace imgui{

	namespace detail
	{
		void checkboxLogic( Rect& rect, bool& value, bool& underMouse)
		{
			Control ctrl(controls::CHECKBOX, rect);
			rect = ctrl.rect;
			
			underMouse = ctrl.mouseHover;
			
			if(ctrl.mouseLeftClick)
				value = !value;
		}

		void checkboxDraw(const char* caption, const Rect& baseRect, bool value, bool underMouse)
		{
			detail::guiAtlas->resource([caption, &baseRect, value, underMouse](auto& a)
			{
				const RectAtlas& atlas = a.atlas;
				
				int contIndex = atlas.index("CB_Base");
				int selIndex = atlas.index("CB_Select");
				int trueIndex = atlas.index("CB_El_Gal");
				int semiIndex = atlas.index("CB_El_Quad");

				
				Rect br = unigui::draw::texrect(baseRect, guiAtlas, unigui::Align::left() + unigui::Align::center(), contIndex);
				if(underMouse)	unigui::draw::texrect(br, guiAtlas, unigui::Align::center(), selIndex);
				if(value)		unigui::draw::texrect(br, guiAtlas, unigui::Align::center(), trueIndex);

				float sw = unigui::draw::stringWidth(caption, detail::font) + 5.0f;
				Rect textRect(br.right(), br.top(), sw, br.size.y);
				unigui::draw::print(caption, textRect, detail::font, unigui::color(0.7f, 0.7f, 1.0f, 1.0f), true);
			});
		}
	}

	bool checkbox(const char* caption, const Point2& pos, bool& value)
	{
		detail::guiAtlas->resource([caption, &pos, &value](auto& a)
		{
			core::checked_begin();

			const RectAtlas& atlas = a.atlas;

			int contIndex = atlas.index("CB_Base");
			const RectAtlas::Rect& r = atlas.absoluteRect(contIndex);
			Rect rect(pos.x, pos.y, r.sizeX(), r.sizeY());

			bool underMouse;
			detail::checkboxLogic(rect, value, underMouse);
			detail::checkboxDraw(caption, rect, value, underMouse);

			core::checked_end();
		});
		
		return value;
	}

	bool checkbox(const char* caption, bool& value)
	{
		return checkbox(caption, Point2(0, 0), value);
	}

	bool checkbox(const char* caption, const Vector3& pos, bool& value)
	{
		Point2 p;
		bool onScreen = detail::worldToScreen(pos, p);
		return onScreen ? checkbox(caption, p, value) : value;
	}

	bool checkbox(const Vector3& pos, bool& value)
	{
		return checkbox("", pos, value);
	}

}//



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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