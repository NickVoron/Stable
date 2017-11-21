#pragma once
#include "../types.h"

namespace imgui
{
namespace drawing
{
	void drawRect(const Rect& rect, const Color& color);
	void drawLine(const Point2& start, const Point2& end, const Color& color);
//	void drawText(const wchar_t* text, const Rect& rect, const Color& color);
//	void drawText(const char* text, const Rect& rect, const Color& color);

	void drawLineRect(const Rect& rect, const Color& color);
}
}