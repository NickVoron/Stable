#pragma once
#include "../types.h"


namespace imgui
{
namespace shift
{

	void begin(const Rect& rect);
	void end();
	Point2 current();
	Point2 total();
	void apply(Point2& point);
	void set(const Point2& s);

}
}//