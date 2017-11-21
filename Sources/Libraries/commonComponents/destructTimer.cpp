#include "destructTimer.h"

void DestructTimer::tick()
{
	destructor->destruct();
}

void DestructTimer::linker()
{
	link(destructor);
}