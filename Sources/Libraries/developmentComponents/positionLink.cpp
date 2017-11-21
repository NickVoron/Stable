#include "positionLink.h"
#include "debugDrawing/library.include.h"

//
//
//
void PositionLink::Resource::properties(PositionLink& component)
{
	property("target", component.target);
}

void PositionLink::update()
{
	auto& selfPosition = component<Position>();
 	target([&selfPosition](auto& linkPosition)
 	{ 
 		Vector3& src = linkPosition.state.position;
 		Vector3& dst = selfPosition.state.position;
		//LOG_EXPRESSION(src, dst);
		//LOG_EXPRESSION(src);
 		DebugDrawing::line(src, dst);
 	});
}
