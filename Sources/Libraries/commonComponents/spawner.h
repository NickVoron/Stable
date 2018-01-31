// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <array>
#include "componentModel/library.include.h"
#include "timer.h"




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



// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.