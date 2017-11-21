#pragma once
#include "../types.h"
#include "../focus.h"
#include "../internalState.h"



namespace imgui
{
	namespace LayoutType
	{
		enum Type {HORIZONTAL, VERTICAL};
	}

	namespace layout{

		void begin(LayoutType::Type type, const Point2& pos);
		bool exist();
		void pause(bool paused);
		void add(Point2& pos, Point2& size);
		void add(Rect& rect);
		void end();

		void begin_default_vertical();
		
	}//
}//