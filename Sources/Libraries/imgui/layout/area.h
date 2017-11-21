#pragma once
#include "../types.h"
#include "../internalState.h"


namespace imgui
{
namespace area
{
	void begin(const Rect& rect, const Point2& sh);
	bool place(const Rect& rect, const Point2& sh_);
	void end();

	bool exist();
	const Rect rect();
	const Point2 neededSize();

}
}//