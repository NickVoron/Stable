#include "shift.h"
#include "../drawing/drawing.h"
#include "../focus.h"
#include "../controlTypes.h"
#include "../internalState.h"

#include <map>

namespace imgui{
namespace shift	{

//
class Shift : public InternalState<Shift>
{
public:
	Shift(Shift* parent_) : InternalState<Shift>(parent_){}

	void init(const Rect& rect_, Point2* shift_)
	{
		rect = rect_;
		shift = shift_;
	}

	const Point2& current(){ return *shift; }
	Point2 total()
	{ 
		return *shift + (parent ? parent->total() : Point2()); 
	}

	Point2* shift;
	Rect rect;
};

//
class ShiftSystem: public InternalStateHolder<ShiftSystem, Shift>
{
public:
	void begin(const Rect& rect)
	{
		Id id(rect.pos, controls::PANEL);
		start()->init(rect, &shifts[id]);
	}

	void end()
	{
		finish();
	}

	std::map<Id, Point2> shifts;
};


ShiftSystem shiftSys;

void begin(const Rect& rect) { shiftSys.begin(rect); }
void end() { shiftSys.end(); }

Point2 current()
{
	Shift* s = shiftSys.current;
	return s ? s->current() : Point2(0, 0);
}

Point2 total()
{
	Shift* s = shiftSys.current;
	return s ? s->total() : Point2(0, 0);
}

void apply(Point2& point)
{
	point += total();
}

void set(const Point2& sh)
{
	Shift* s = shiftSys.current;
	if(s) *s->shift = sh;
}
}
}