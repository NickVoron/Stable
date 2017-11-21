#pragma once
#include "../types.h"

namespace imgui
{
	void color_rgb(const char* caption, const Point2& pos, float& r, float& g, float& b);
	void color_rgba(const char* caption, const Point2& pos, float& r, float& g, float& b, float& a);
	void color_hsv(const char* caption, const Point2& pos, float& h, float& s, float& v);
	void color_hsva(const char* caption, const Point2& pos, float& h, float& s, float& v, float& a);
	void color_rgb_as_hsv(const char* caption, const Point2& pos, float& r, float& g, float& b);

	void color_rgb(const char* caption, float& r, float& g, float& b);
	void color_rgba(const char* caption, float& r, float& g, float& b, float& a);
	void color_hsv(const char* caption, float& h, float& s, float& v);
	void color_hsva(const char* caption, float& h, float& s, float& v, float& a);
	void color_rgb_as_hsv(const char* caption, float& r, float& g, float& b);

	void color_rgb(const Point2& pos, float& r, float& g, float& b);
	void color_rgba(const Point2& pos, float& r, float& g, float& b, float& a);
	void color_hsv(const Point2& pos, float& h, float& s, float& v);
	void color_hsva(const Point2& pos, float& h, float& s, float& v, float& a);
	void color_rgb_as_hsv(const Point2& pos, float& r, float& g, float& b);

	void color_rgb(float& r, float& g, float& b);
	void color_rgba(float& r, float& g, float& b, float& a);
	void color_hsv(float& h, float& s, float& v);
	void color_hsva(float& h, float& s, float& v, float& a);
	void color_rgb_as_hsv(float& r, float& g, float& b);


}//