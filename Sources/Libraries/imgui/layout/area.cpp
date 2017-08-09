// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "area.h"
#include "../drawing/drawing.h"

namespace imgui{
namespace area{

	
	class Area : public InternalState<Area>
	{
	public:
		Area(Area* parent);

		void init(const Rect& rect, const Point2& sh);
		bool place(const Rect& rect, const Point2& sh_);

		Point2 neededSize() 
		{ 
			Point2 chs = unigui::math::convexHull(rect, usedRect).size;
			return chs - rect.size + shift; 
		}

		Rect rect;
		Rect usedRect;
		Point2 shift;
	};

	
	class AreaSystem : public InternalStateHolder<AreaSystem, Area>
	{
	public:
		void begin(const Rect& rect, const Point2& sh);		
		void end();
		bool place(const Rect& rect, const Point2& sh_);
	};


void AreaSystem::begin(const Rect& rect, const Point2& sh)
{
	start()->init(rect, sh);
}

void AreaSystem::end()
{



	finish();
}

bool AreaSystem::place(const Rect& rect, const Point2& sh)
{
	ENFORCE(current);
	return current->place(rect, sh);
}



Area::Area(Area* parent_) : InternalState<Area>(parent_)
{
	usedRect.pos = usedRect.size = Point2(-1, -1);
}

void Area::init(const Rect& rect_, const Point2& sh_)
{
	rect = rect_;
	shift = sh_;
}

bool Area::place(const Rect& r, const Point2& sh_)
{
  	usedRect = (usedRect.pos.x < 0) ? r : unigui::math::convexHull(r, usedRect);
	Rect rr = r;
	rr.pos += sh_;
	return rect.intersected(rr);
}


AreaSystem areaSys;

void begin(const Rect& rect, const Point2& sh) 	{ areaSys.begin(rect, sh); }
bool place(const Rect& rect, const Point2& sh) { return (exist() && areaSys.place(rect, sh) || !exist()); }
void end() { areaSys.end(); }
Area* current() { return areaSys.current; }
bool exist(){ return current() != 0; }

const Rect rect() { return current()->rect; }
const Point2 neededSize() { return current()->neededSize(); }

}}



// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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