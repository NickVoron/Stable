#pragma once
#include "destructor.h"
#include "timer.h"

class DestructTimer : public BaseTimer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(DestructTimer);

	void linker();

protected:
	virtual void tick() override;
	Destructor* destructor;
};