#pragma once

#include "timer.h"

class TimerConfigurator : public UpdatableComponent<TimerConfigurator>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(TimerConfigurator);

	void linker();

	void update(float dt);

protected:
	BaseTimer* timer1;
	BaseTimer* timer2;
};
