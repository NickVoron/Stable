#include "layout.h"
#include "transform.h"


#include "../drawing/drawing.h"
#include "../focus.h"
#include "../controlTypes.h"

namespace imgui
{
namespace layout
{

//
class Layout : public InternalState<Layout>
{
public:
	Layout(Layout* parent);
	~Layout();

	void init(LayoutType::Type type, const Point2& pos, Point2* size);
	Point2 addWidget(Point2& size);

	Point2 pos() const { return _pos; }
	const Point2& currentIterSize() const { return _currentIterSize; }

private:
	LayoutType::Type type;
	Point2 _pos;
	Point2 _currentIterSize;

	Point2* _size;
};

class LayoutSystem : public InternalStateHolder<LayoutSystem, Layout>
{
public:
	LayoutSystem():paused(false){}
	void begin(LayoutType::Type type, const Point2& pos);		
	void end();
	bool exist(){ return current != 0 && !paused; };
	void pause(bool p){ paused = p; };
	Point2 addWidget(Point2& size);
private:
	std::map<Id, Point2> sizes;
	bool paused;
};

void LayoutSystem::begin(LayoutType::Type type, const Point2& pos)
{
	Id id(pos, controls::LAYOUT);
	start()->init(type, pos, &sizes[id]);
}

void LayoutSystem::end()
{
// 	
// 	// отрисовка при закрытии
// 	Color color = {0.0f, 1.0f, 0.0f, 1.0f};
// 	Point2 pos = current->pos();
// 	const Point2& size = current->currentIterSize();
// 	drawing::drawLineRect( Rect(pos, size), color);

	//
	finish();
}

Point2 LayoutSystem::addWidget(Point2& size)
{
	return current ? current->addWidget(size) : Point2(0, 0);
}


//
Layout::Layout(Layout* parent_) : InternalState<Layout>(parent_)
{
	_currentIterSize.zero();
}

Layout::~Layout()
{
	(*_size) = _currentIterSize;
	if (parent)
		parent->addWidget( *_size );

}

void Layout::init(LayoutType::Type _type, const Point2& pos, Point2* size_)
{
	type = _type;
	_size =  size_;

	if (parent)
	{
		if (parent->type == LayoutType::HORIZONTAL)
		{
			_pos.x = parent->pos().x + parent->currentIterSize().x;
			_pos.y = parent->pos().y;
		}
		else
		{
			_pos.x = parent->pos().x;
			_pos.y = parent->pos().y + parent->currentIterSize().y;
		}
	}
	else
		_pos = pos;

	//currentEnd = _pos;
}

Point2 Layout::addWidget(Point2& wSize)
{
	switch (type)
	{
	case LayoutType::HORIZONTAL:
		{
			Point2 wPos = _pos; // расчет позиции для текущего виджета
			wPos.x += _currentIterSize.x;

			_currentIterSize.x += wSize.x;

			_currentIterSize.y = std::max(_currentIterSize.y, wSize.y);	// маштабирование лайаута
			//wSize.y = std::max(_size->y, wSize.y);				// маштабирование виджета

			return wPos;
		}
	case LayoutType::VERTICAL:
		{
			Point2 wPos = _pos; // расчет позиции для текущего виджета
			wPos.y += _currentIterSize.y;

			_currentIterSize.y += wSize.y + 2;
			_currentIterSize.x = std::max(_currentIterSize.x, wSize.x);	// маштабирование лайаута
			//wSize.x = std::max(_size->x, wSize.x);				// маштабирование виджета
			
			return wPos;
		}
	}

	return Point2(0, 0);
}

LayoutSystem layoutSys;
   
//
void begin(LayoutType::Type type, const Point2& pos) {	layoutSys.begin(type, pos);}
void add(Point2& pos, Point2& size){  if(exist()) pos = layoutSys.addWidget(size);  }
void add(Rect& rect){ add(rect.pos, rect.size); }

void pause(bool paused)
{
	layoutSys.pause(paused);
}

void end(){	layoutSys.end(); }
bool exist(){ return layoutSys.exist();}

void begin_default_vertical()
{
	begin(LayoutType::VERTICAL, Point2(0, 0));
}


}}