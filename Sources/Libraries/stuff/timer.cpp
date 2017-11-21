#include "timer.h"

namespace Base 
{

	Timer::Timer()
	{
		start();
	}

	void Timer::start()
	{
		prevCounter = std::chrono::high_resolution_clock::now();
	}

	float Timer::dt() const
	{
		auto result = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - prevCounter).count() / 1000.0f;
		prevCounter = std::chrono::high_resolution_clock::now();
		return result;
	}

	//
	IntervalTimer::IntervalTimer(float value, Function func)
	:interval(value), function(func), accum(0.0f)
	{		
	}

	void IntervalTimer::update()
	{
		accum += dt();
		if (accum >= interval)
		{
			accum -= interval;
			function(interval);
		}
	}
}