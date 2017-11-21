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
		//Rect screenRect()  { return Rect(screenPos(), _rect.size); }

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

	//
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

	//Point2 pos()	{ return current() ? current()->screenPos() : Point2(); }
	Rect rect()		{ return current() ? current()->rect() : Rect(); }

}
}