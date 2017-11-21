#include "swapTimer.h"

void SwapTimer::tick()
{
	swaper->swap();
}

void SwapTimer::linker()
{
	link(swaper);
}