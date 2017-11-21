#include "timedLogQueue.h"

namespace logs{
namespace implementation{

	TimedLogQueue::TimedLogQueue()
	:popTime(-1.0f), currentPopTime(-1.0f)
	{
		
	}

	void TimedLogQueue::setPopTime(float time)
	{
		popTime = time;
	}

	void TimedLogQueue::update(float dt)
	{
		if(!empty() && popTime > 0)
		{
			currentPopTime -= dt;
			if(currentPopTime <= 0.0f)
			{
				pop();
				
			}
		}	
	}

	void TimedLogQueue::pop()
	{
		LogQueue::pop();
		currentPopTime = popTime;
	}

}
}

