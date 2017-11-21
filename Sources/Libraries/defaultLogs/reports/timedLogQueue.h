#pragma once

#include "logQueue.h"

namespace logs{
namespace implementation{

	//
	class TimedLogQueue : public LogQueue
	{
	public:
		TimedLogQueue();
		TimedLogQueue(float popTime) { setPopTime(popTime); }

		// время для выталкивания одного элемента
		void setPopTime(float time);	

		//
		void update(float dt);

		virtual void pop() override;

	private:
		float popTime;
		float currentPopTime;
	};
}
}

