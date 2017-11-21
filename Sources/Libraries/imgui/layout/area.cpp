#include "area.h"
#include "../drawing/drawing.h"

namespace imgui{
namespace area{

	//
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

	//
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
// 	// отрисовка при закрытии
// 	Color color = {1.0f, 0.0f, 0.0f, 1.0f};
// 	drawing::drawLineRect(current->rect, color);
	finish();
}

bool AreaSystem::place(const Rect& rect, const Point2& sh)
{
	ENFORCE(current);
	return current->place(rect, sh);
}


//
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

//
AreaSystem areaSys;

void begin(const Rect& rect, const Point2& sh) 	{ areaSys.begin(rect, sh); }
bool place(const Rect& rect, const Point2& sh) { return (exist() && areaSys.place(rect, sh) || !exist()); }
void end() { areaSys.end(); }
Area* current() { return areaSys.current; }
bool exist(){ return current() != 0; }

const Rect rect() { return current()->rect; }
const Point2 neededSize() { return current()->neededSize(); }

}}