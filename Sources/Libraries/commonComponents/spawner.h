#pragma once

#include <array>
#include "componentModel/library.include.h"

//
//
//
struct SpawnerImplementation
{
	template<class Configurator, class... ConfigurationFunctions>
	void spawn(EntitiesList& entities, Configurator&& configurator, ConfigurationFunctions&&... functions)
	{
		spawner.spawn(entities, std::forward<Configurator>(configurator), std::forward<ConfigurationFunctions>(functions)...);
	}

	void spawn(EntitiesList& entities);
	void finalize(Entity& entity);

	SystemSpawner spawner;
	EntitiesStream prototype;
};

class Spawner : public SimpleComponentAutoLink<Spawner, Entity>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Spawner);

	struct Resource : public ResourceBase 
	{
		void properties(Spawner& component);
	};

  	template<class Configurator, class... ConfigurationFunctions>
  	void spawn(Configurator&& configurator, ConfigurationFunctions&&... functions)
  	{
		impl.spawn(entities(), std::forward<Configurator>(configurator), std::forward<ConfigurationFunctions>(functions)...);
  	}

	void spawn()
	{
		impl.spawner.spawn(entities());
	}

	void finalize() override
	{
		impl.finalize(entity());
	}
	

private:
	SpawnerImplementation impl;
};


//
//
//
template<class... ComponentsList>
struct ConfigurableSpawner
{
	template<class Function>
	void spawn(Function&& function)
	{
		this->spawner->spawn(SimpleConfigurator<ComponentsList...>(), std::forward<Function>(function));
	}

	Spawner* spawner { nullptr };
};

//
//
//
template<class... ComponentsList>
struct AccumulatingSpawner
{
	auto& components() { return links; }

	template<class Function>
	void spawn(Function&& function)
	{	
		this->spawner->spawn(SimpleConfigurator<ComponentsList...>(), std::forward<Function>(function), [this](Entity& spawned, ComponentsList&... component)
		{
			links.add(spawned, component...);
		});
	}

	template<class ListPredicate, class Function>
	void spawn(ListPredicate&& listPredicate, Function&& function)
	{
		for (auto& link : links)
		{
			if(!link(listPredicate))
				return;				
		}

		spawn(std::forward<Function>(function));
	}

	ComponentLinkList<ComponentsList...> links;
	Spawner* spawner{ nullptr };
};