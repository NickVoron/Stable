// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "layout.h"
#include "transform.h"


#include "../drawing/drawing.h"
#include "../focus.h"
#include "../controlTypes.h"

namespace imgui
{
namespace layout
{


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







	
	finish();
}

Point2 LayoutSystem::addWidget(Point2& size)
{
	return current ? current->addWidget(size) : Point2(0, 0);
}



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

	
}

Point2 Layout::addWidget(Point2& wSize)
{
	switch (type)
	{
	case LayoutType::HORIZONTAL:
		{
			Point2 wPos = _pos; 
			wPos.x += _currentIterSize.x;

			_currentIterSize.x += wSize.x;

			_currentIterSize.y = std::max(_currentIterSize.y, wSize.y);	
			

			return wPos;
		}
	case LayoutType::VERTICAL:
		{
			Point2 wPos = _pos; 
			wPos.y += _currentIterSize.y;

			_currentIterSize.y += wSize.y + 2;
			_currentIterSize.x = std::max(_currentIterSize.x, wSize.x);	
			
			
			return wPos;
		}
	}

	return Point2(0, 0);
}

LayoutSystem layoutSys;
   

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



// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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