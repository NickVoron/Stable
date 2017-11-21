#pragma once
#include "../types.h"

namespace imgui
{
namespace transform
{

	void begin(const Rect& rect);
	void end();
	void apply(Point2& point);
	void pause(bool paused);
	Rect rect();
	
}
}

