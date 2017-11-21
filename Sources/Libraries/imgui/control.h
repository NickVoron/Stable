#pragma once

#include "types.h"

namespace imgui
{
	struct ControlState
	{
		bool mouseLeftDrag;
		bool mouseLeftDown;
		bool mouseLeftClick;
		bool mouseHover;
		bool mouseHoverImmediate;
		bool mouseMove;

		ControlState();
		void reset();
	};

	bool screen_pos(Rect& rect);
	bool screen_pos(Rect& rect, Id& id);

	struct Control : public ControlState
	{
		Control(int controlType, const Rect& inRect, bool inputCapture = true);
		Control(int controlType, const Rect& inRect, const Id& id, bool inputCapture = true);
		
		Rect rect;
		Id id;
		bool invalid;
	};
}
