#pragma once

#include "types.h"

namespace imgui
{

	namespace detail
	{
		extern Mouse mouse;
		extern Keyboard keyboard;
	}

	void Mouse::addProcessed(bool processed)
	{
		processedFlag |= processed;
	}

	bool Mouse::processed() const
	{
		return processedFlag;
	}

	bool underMouse(const Rect& rect)
	{
		return rect.contain(detail::mouse.pos);		
	}

	std::ostream& operator<<(std::ostream& os, const Id& id)
	{
		return os << id.type << ": " << id.pos;
	}


}