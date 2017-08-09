// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "shift.h"
#include "../drawing/drawing.h"
#include "../focus.h"
#include "../controlTypes.h"
#include "../internalState.h"

#include <map>

namespace imgui{
namespace shift	{


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