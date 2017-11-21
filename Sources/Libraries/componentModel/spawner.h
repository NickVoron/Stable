#pragma once

#include "prototypesHolder.h"

class SystemSpawner
{
public:
	template<class Configurator, class... ConfiguratorFunctions>
	void spawn(EntitiesList& parent, Configurator&& cfg, ConfiguratorFunctions&&... functions);

	void spawn(EntitiesList& parent);

	void prototype(stream::dmemstream* is);

	void resetCounter();

	std::size_t totalCounter = 0;
	std::size_t afterResetCounter = 0;
	int limit = -1;
	int remaining = -1;

private:
	stream::dmemstream* is = nullptr;
};

template<class Configurator, class... ConfiguratorFunctions>
void SystemSpawner::spawn(EntitiesList& entities, Configurator&& configurator, ConfiguratorFunctions&&... functions)
{
	if ((remaining != 0) && is)
	{
		stream::cycle_idmemstream cis(*is);

		std::vector<ComponentExternalLink> links;
		cis >> links;

		entities.loadObjects(cis, true, links, [&configurator, &functions...](auto& entity)
		{
			configurator(entity, functions...);
		});

		if (remaining > 0)
		{
			--remaining;
		}

		++totalCounter;
		++afterResetCounter;
	}
}