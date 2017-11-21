#include "timerConfigurator.h"

void TimerConfigurator::update(float dt)
{
	//LOG_WARNING(timer1->classId() << " " << timer2->classId());
}

void TimerConfigurator::linker()
{
	link("timer1", timer1);
	link("timer2", timer2);
}

