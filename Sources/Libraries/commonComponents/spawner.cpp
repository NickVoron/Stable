#include "spawner.h"

//
//
//
void SpawnerImplementation::spawn(EntitiesList& entities)
{
	spawner.spawn(entities);
}

void SpawnerImplementation::finalize(Entity& entity)
{
	if(prototype.streamId)
	{
		ENFORCE(entity.getClass().classes && prototype.streamId);
		auto& prototypeHoder = entity.getClass().classes->prototypes;

		if (auto is = prototypeHoder.find(prototype.streamId))
		{
			spawner.prototype(&is->ios);
			spawner.resetCounter();
		}
	}
}

//
//
//
void Spawner::Resource::properties(Spawner& component)
{
	property("prototype", component.impl.prototype);
	property("limit", component.impl.spawner.limit);
	property("remaining", component.impl.spawner.remaining);
}