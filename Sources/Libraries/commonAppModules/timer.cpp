#include "timer.h"
#include "containers/circular_buffer.h"
#include "stuff/average_value.h"

namespace AppModules
{
	Timer::Params::Params()
	{
		timeFactor = 1.0f;
	}

	void Timer::update(Params& params) 
	{ 
		lastTime = timer.dt();
		lastFPS = 1.0f / lastTime;

		params.dt = lastTime * params.timeFactor;
		params.fps = nm::round(lastFPS);


		LOG_REPORT("fps: " << params.fps);
	}

	void Timer::after_init_impl(Params& params)
	{
		timer.start();
		update(params);
	}

	float Timer::fps(bool average)
	{
		return lastFPS;
	}

	float Timer::dt(bool average)
	{
		return lastTime;
	}

}