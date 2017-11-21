#include "debugDrawComponents.h"

#ifdef WIN32
#include "debugDrawing/library.include.h"
#include "imgui/library.include.h"
#endif

namespace DebugDrawComponents
{

void BaseDebugDrawer::linker()
{
	link(position);
}

//
//
//
void PositionDrawer::update()
{
#ifdef WIN32
	DebugDrawing::locator(position->state);
//	LOG_MSG(position->state.position);
#endif
}



//
//
//
//
SphereDrawer::SphereDrawer()
{
#ifdef WIN32
	image::rgb(1.0f, 1.0f, 1.0f, color);
#endif
}

void SphereDrawer::Resource::properties(SphereDrawer& component)
{
	property("radius", component.radius);
	property("color", component.color);
	property("text", component.text);
}

void SphereDrawer::update()
{
#ifdef WIN32
	DebugDrawing::color(color);
	DebugDrawing::sphere(position->state.position, radius);
	if (!text.empty())
	{
		DebugDrawing::text(position->state.position, text);
	}	
#endif
}

}

namespace properties_inheritance
{
	void inherite(DebugDrawComponents::SphereDrawer& parent, DebugDrawComponents::SphereDrawer& child)
	{
		//child.radius = parent.radius * 1.1f;
	}
}
