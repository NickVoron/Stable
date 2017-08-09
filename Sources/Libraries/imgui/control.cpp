// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "control.h"
#include "layout/layout.h"
#include "layout/transform.h"
#include "layout/area.h"
#include "layout/shift.h"

#include "resourceAtlas/library.include.h"


namespace imgui
{

	ControlState::ControlState()
	{
		reset();
	}

	void ControlState::reset()
	{
		mem::memzero(*this);
	}

	inline bool screen_pos(Rect& rect, Id* id, const Id* eid)
	{
		layout::add(rect);
		transform::apply(rect.pos);

		if(id) 
		{
			if(!eid)
			{
				id->pos = rect.pos;
			}
			else
			{
				*id = *eid;
			}			
		}

		bool res = area::place(rect, shift::total());
		shift::apply(rect.pos);

		return res;
	}

	bool screen_pos(Rect& rect)	{ return screen_pos(rect, 0, 0); }
	bool screen_pos(Rect& rect, Id& id) { return screen_pos(rect, &id, 0); }
	bool screen_pos(Rect& rect, Id& id, const Id* eid) { return screen_pos(rect, &id, eid); }

	namespace capture {

		extern Id current;
	}

	namespace detail{

		extern Mouse mouse;
		extern Keyboard keyboard;
		extern Resources::Atlas* guiAtlas;



		bool control(ControlState& cs, int controlType, const Rect& inRect, Rect& screenRect, Id& id, const Id* eid, bool inputCapture)
		{
			screenRect = inRect;
			bool res = screen_pos(screenRect, id, eid);
			id.type = controlType;

			bool containMouse = screenRect.contain(mouse.pos);
			mouse.addProcessed(containMouse);

			cs.reset();

			if(res && inputCapture)
			{
				cs.mouseHoverImmediate = containMouse;
				cs.mouseHover = cs.mouseHoverImmediate && (capture::check(id)  || capture::empty());
	
				if(cs.mouseHoverImmediate && mouse.lmb == Mouse::NOW_UP)
				{
					cs.mouseLeftClick = capture::check(id);
				}

				if(cs.mouseHoverImmediate && capture::empty())
				{
					if(mouse.lmb == Mouse::NOW_DOWN || mouse.lmb == Mouse::DOWN)
					{
						capture::set(id);
						cs.mouseLeftDown = true;
					}
				}
				
				if(mouse.lmb == Mouse::DOWN)
				{
					cs.mouseLeftDrag = capture::check(id);
					
				}					
			}

			return false;			
		}
	}

Control::Control(int controlType, const Rect& inRect, bool inputCapture)
{
	invalid = !detail::control(*this, controlType, inRect, rect, id, 0, inputCapture);
}

Control::Control(int controlType, const Rect& inRect, const Id& eid, bool inputCapture)
{
	invalid = !detail::control(*this, controlType, inRect, rect, id, &eid, inputCapture);
}

}//



// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.