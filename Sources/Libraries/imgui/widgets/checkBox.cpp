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

				//Rect br = unigui::draw::texrect(baseRect, guiAtlas, unigui::Align::left() + unigui::Align::center(), contIndex);
				Rect br = unigui::draw::texrect(baseRect, guiAtlas, unigui::Align::left() + unigui::Align::center(), contIndex);
				if(underMouse)	unigui::draw::texrect(br, guiAtlas, unigui::Align::center(), selIndex);
				if(value)		unigui::draw::texrect(br, guiAtlas, unigui::Align::center(), trueIndex);

				float sw = unigui::draw::stringWidth(caption, detail::font) + 5.0f;
				Rect textRect(br.right(), br.top(), sw, br.size.y);
				unigui::draw::print(caption, textRect, detail::font, unigui::color(0.7f, 0.7f, 1.0f, 1.0f), true);
			});
		}
	}// detail

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