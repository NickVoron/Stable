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
}//
