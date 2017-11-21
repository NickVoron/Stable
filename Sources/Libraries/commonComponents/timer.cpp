#include "timer.h"

void BaseTimer::Resource::properties(BaseTimer& component)
{
	property("interval", component.interval);
}

void BaseTimer::update(float dt)
{
	accumulatedTime += dt;
	if(accumulatedTime >= interval)
	{
		accumulatedTime = 0.0f;
		tick();
	}
}
