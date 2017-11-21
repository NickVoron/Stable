#pragma once
#include "swaper.h"
#include "timer.h"

class SwapTimer : public BaseTimer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SwapTimer);
	
	void linker();

protected:
	virtual void tick() override;
	Swaper* swaper;
};
