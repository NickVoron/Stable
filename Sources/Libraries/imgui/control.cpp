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
					//mouse.processed = !cs.mouseLeftDrag;
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