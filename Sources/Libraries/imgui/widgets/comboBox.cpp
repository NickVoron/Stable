// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "comboBox.h"
#include "button.h"
#include "panel.h"

#include "../controlTypes.h"
#include "../focus.h"
#include "../control.h"
#include "../globals.h"
#include "../imgui.h"

#include "../layout/layout.h"
#include "../layout/transform.h"

namespace imgui
{
	namespace
	{
		struct CacheEntry
		{
			CacheEntry():selectedIndex(-1), opened(false){}
			int selectedIndex;
			bool opened;
			void toggle() { opened = !opened; }
		};

		class ComboListCache : public ControlStateCache<CacheEntry>{};
		
		ComboListCache comboListCache;

	}

	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount, int elementHeight)
	{
		Control ctrl(controls::COMBOBOX, Rect(pos.x, pos.y, (float)width, (float)elementHeight), false);

		
		int btWidth = 25;
		Rect fieldRect = ctrl.rect;
		fieldRect.size.x -= 25;
		panel_draw(fieldRect, true);

		CacheEntry& cache = comboListCache.state(ctrl.id);
		
 		layout::pause(true);
 		transform::pause(true);
		Point2 btPos = fieldRect.rightTop();
		
		if(button("open", btPos, btWidth))
		{
			cache.toggle();
		}
		layout::pause(false);
		transform::pause(false);
		
		ListResult res;
		if(cache.opened)
		{
			res = list(caption, elements, elementsCount, pos, width, visibleElementsCount, elementHeight);
			cache.selectedIndex =  res.selectedIndex;
		}

		if(cache.selectedIndex >= 0)
		{
			Color normalColor = unigui::color(1.0f, 1.0f, 1.0f, 1.0f);
			const ListElement& elem = elements[cache.selectedIndex];
			unigui::draw::print(elem.text.c_str(), ctrl.rect, detail::font, normalColor, true);
		}

		return res;
	}


	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount)
	{
		int height = 7;
		detail::font->resource([&height](auto& f) { height += f.face.height; });
		return combobox(caption, elements, elementsCount, pos, width, visibleElementsCount, height);
	}

	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount)
	{
		int height = 7;
		detail::font->resource([&height](auto& f) { height += f.face.height; });
		return combobox(caption, elements, elementsCount, visibleElementsCount, height);
	}


	ListResult combobox(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount, int elementHeight)
	{
		return combobox(caption, elements, elementsCount, Point2(0, 0), 100, visibleElementsCount, elementHeight);
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