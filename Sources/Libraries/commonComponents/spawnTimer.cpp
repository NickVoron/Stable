#include "spawnTimer.h"

void SpawnTimer::tick()
{
	spawner->spawn();
}

void SpawnTimer::linker()
{
	link(spawner);
}