// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "list.h"
#include "panel.h"

#include "../controlTypes.h"
#include "../focus.h"
#include "../control.h"
#include "../globals.h"
#include "../imgui.h"

namespace imgui
{

	class ListElementsCache			: public ControlStateCache<ListElement>{};
	class ListScrollCache			: public ControlStateCache<float>{};
	class ListScrollTweenerCache	: public ControlStateCache< VerticalScrollTweener::Param >{};

	namespace
	{
		ListElementsCache selectionCache;
		ListElementsCache underMouseCache;
		ListScrollCache scrollCache;
		ListScrollTweenerCache scrollTweenerCache;
	}

	ListResult::ListResult()
	{
		selectedIndex = -1;
		underMouseIndex = -1;
		selectionChange = false;
		underMouseChange = false;
	}

	bool ListResult::operator==(const ListResult& r) const
	{
		return	r.selectionChange == selectionChange &&
				r.underMouseChange == underMouseChange &&
				r.selectedIndex == selectedIndex &&
				r.underMouseIndex == underMouseIndex;
	}

	bool ListResult::operator!=(const ListResult& r) const
	{
		return !operator==(r);
	}

	
	bool ListElement::operator==(const ListElement& lel) const { return text == lel.text && index == lel.index; }
	bool ListElement::operator!=(const ListElement& lel) const { return !operator==(lel); }

	void drawElement(const ListElement& elem, const Rect& rect, bool underMouse, bool selected)
	{
		Color normalColor = unigui::color(0.0f, 0.0f, 0.0f, 1.0f);
		Color underMouseColor = unigui::color(1.0f, 1.0f, 0.0f, 1.0f);

		Color color = underMouse ? underMouseColor : normalColor;

		Rect textRect = rect; 
		textRect.cutLeft(2);
		textRect.cutRight(2);
		textRect.cutTop(1);
		textRect.cutBottom(1);

		if(selected)
		{
			panel_draw(textRect, false);
		}
		
		textRect.cutLeft(1);
		unigui::draw::print(elem.text.c_str(), textRect, detail::font, color, true);
	}

	ListResult list(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount, int elementHeight)
	{
		Control ctrl(controls::LIST, Rect(pos.x, pos.y, (float)width, (float)elementHeight*visibleElementsCount));

		
		

		
		float& scrollValue = scrollCache.state(ctrl.id);
		if(underMouse(ctrl.rect))
		{
			int scrlElLim = elementsCount-visibleElementsCount;
			if(scrlElLim < 0) scrlElLim = 0;
			float scrlLim = -(float)scrlElLim * elementHeight;
		
			scrollValue -= detail::mouse.wheelDelta * elementHeight;
			scrollValue = nm::clamp(scrollValue, scrlLim, 0.0f);
		}	   		

		
		ListResult result;
		ListElement oldUnderMouse = underMouseCache.state(ctrl.id);
		ListElement oldSelection = selectionCache.state(ctrl.id);
		Rect elemRect = ctrl.rect; 
		elemRect.pos.y = ctrl.rect.pos.y + scrollValue;
		elemRect.size.y = (float)elementHeight;
		for (int i = 0; i < elementsCount; ++i)
		{
			if(!ctrl.rect.contain(elemRect))
			{
				elemRect.pos.y += elementHeight;
				continue;
			}

			bool elemUnderMouse = underMouse(elemRect);
			bool selected = selectionCache.stateIsEqualTo(ctrl.id, elements[i]);

			if(elemUnderMouse)
			{
				result.underMouseIndex = i;
				result.underMouseChange = (oldUnderMouse != elements[i]);

				if(detail::mouse.lmb == Mouse::NOW_UP)
				{
					selectionCache.setState(ctrl.id, elements[i]);
					result.selectionChange = (oldSelection != elements[i]);
					selected = true;
				}				
			}

			drawElement(elements[i], elemRect, elemUnderMouse, selected);

			if(selected)
			{
				result.selectedIndex = i;
			}

			elemRect.pos.y += elementHeight;
		}

		return result;
	}

	ListResult list(const char* caption, ListElement* elements, int elementsCount, const Point2& pos, int width, int visibleElementsCount)
	{
		int height = 7;
		detail::font->resource([&height](auto& f) { height += f.face.height; });
		return list(caption, elements, elementsCount, pos, width, visibleElementsCount, height);
	}
	
	ListResult list(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount)
	{
		int height = 7;
		detail::font->resource([&height](auto& f) { height += f.face.height; });
		return list(caption, elements, elementsCount, visibleElementsCount, height);
	}


	ListResult list(const char* caption, ListElement* elements, int elementsCount, int visibleElementsCount, int elementHeight)
	{
		return list(caption, elements, elementsCount, Point2(0, 0), 100, visibleElementsCount, elementHeight);
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