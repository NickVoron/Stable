#pragma once

#include <chrono>
#include <functional>

namespace Base 
{
	class Timer
	{
	public:
		Timer();
		void start();
		float dt() const;
	
	private:
		mutable decltype(std::chrono::high_resolution_clock::now()) prevCounter;
	};
	
	class IntervalTimer : public Timer
	{
	public:
		typedef std::function<void(float)> Function;
	
		IntervalTimer(float interval, Function function);
		void update();
	
	private:
		float interval;
		float accum;
		Function function;
	};	
}