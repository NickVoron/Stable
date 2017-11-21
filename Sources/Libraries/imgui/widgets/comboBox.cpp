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

		// рисуем задник
		int btWidth = 25;
		Rect fieldRect = ctrl.rect;
		fieldRect.size.x -= 25;
		panel_draw(fieldRect, true);

		CacheEntry& cache = comboListCache.state(ctrl.id);
		
 		layout::pause(true);
 		transform::pause(true);
		Point2 btPos = fieldRect.rightTop();
		//Point2 btPos(0,0);
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