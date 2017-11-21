#pragma once
#include "spawner.h"
#include "timer.h"

class SpawnTimer : public BaseTimer
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SpawnTimer);

	typedef BaseTimer::Resource Resource;
	
	void linker();

protected:
	virtual void tick() override;
	Spawner* spawner;
};
