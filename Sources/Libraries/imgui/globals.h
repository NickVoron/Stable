#pragma once

#include "resourceFont/library.include.h"
#include "resourceAtlas/library.include.h"

#include "types.h"

namespace imgui
{
	namespace detail
	{
		extern Mouse mouse;
		extern Keyboard keyboard;
		extern Resources::Font* font;
		extern Resources::Atlas* guiAtlas;

		bool worldToScreen(const Vector3& pos, Point2& point);
		Vector3 billboardPosition(const Vector3& base, const Vector2& delta);
	}
}