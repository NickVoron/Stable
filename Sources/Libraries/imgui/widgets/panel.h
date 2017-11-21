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
