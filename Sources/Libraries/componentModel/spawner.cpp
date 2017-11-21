#include "spawner.h"
#include "entityList.h"


void SystemSpawner::prototype(stream::dmemstream* is_)
{
	ENFORCE_POINTER(is_);
	is = is_;
}

void SystemSpawner::spawn(EntitiesList& parent)
{
	spawn(parent, [](auto&){});
}

void SystemSpawner::resetCounter()
{
	remaining = limit;
	afterResetCounter = 0;
}