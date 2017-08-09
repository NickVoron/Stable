// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "transform.h"
#include "../drawing/drawing.h"
#include "../internalState.h"


namespace imgui
{
namespace transform
{

	class Transform : public InternalState<Transform>
	{
	public:
		Transform(Transform* parent) : InternalState<Transform>(parent){}

		void init (const Rect& r){ _rect = r; }
		const Rect& rect() { return _rect; }
		

		void apply(Point2& point) const 
		{ 
			const Transform* p = this;
			while (p)
			{
				point += _rect.pos;
				p = p->parent;
			}
		}

	private:
		Rect _rect;
	};

	
	class TransformGraph : public InternalStateHolder<TransformGraph, Transform>
	{
	public:
		void begin(const Rect& r) { start()->init(r); }
		void end() { finish(); }
		void pause(bool p){paused = p;}
		bool paused;
	};


	
	TransformGraph graph;

	void begin(const Rect& rect) { graph.begin(rect); }
	void end() { graph.end(); }
	Transform* current() { return graph.current; }

	void pause(bool paused){graph.paused = paused;}
	
	void apply(Point2& point)
	{
		if(current() && !graph.paused)
		{
			current()->apply(point);
		}
	}

	
	Rect rect()		{ return current() ? current()->rect() : Rect(); }

}
}



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