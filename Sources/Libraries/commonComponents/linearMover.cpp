#include "linearMover.h"

void LinearMover::Resource::properties(LinearMover& component)
{
	property("velocity", component.velocity);
	property("global", component.global);
}

void LinearMover::update(float dt)
{
	State& s = component<Position>().state;
	Vector3 res;
	if(!global)
	{
		Vector3 y, z;
		s.orientation.GetXAxis(res);
		s.orientation.GetYAxis(y);
		s.orientation.GetZAxis(z);
		
		res *= velocity.x;
		y *= velocity.y;
		z *= velocity.z;
		res += y;
		res += z;
		
	}
	else
	{
		res = velocity;
	}

	res *= dt;
	s.position += res;
}

//
//
//
void AroundRotator::Resource::properties(AroundRotator& component)
{
	property("target", component.target);
	property("velocity", component.velocity);
}

void AroundRotator::update(float dt)
{
	auto& selfPosition = component<Position>();
	target([&selfPosition](auto& linkPosition)
	{ 
		Vector3& src = linkPosition.state.position;
		Vector3& dst = selfPosition.state.position;
				
		dst = src + Vector3(10, 0, 0);
	});
}
