// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "common/function_traits.h"
#include "common/apply.h"

#include "componentModel/library.include.h"

class Spawner : public DataComponent<Spawner>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Spawner);

	struct Resource : public ResourceBase 
	{
		void properties(Spawner& component);
	};

  	template<class Configurator>
  	Entity* spawn(Configurator& configurator)
  	{
  		return Spawner::spawner.spawn(*parent, configurator);
  	}

	virtual void finalize() override;

	Entity* spawn();

	void linker();

	Entity* parent;

private:
	EntitiesStream prototype;
	SystemSpawner spawner;
};

template<class... ComponentsList>
struct ComplexSpawner
{
	struct Caller
	{
		virtual void configure(ComponentsList&... component) = 0;
	};

	template<class Function>
	struct CallerT : public Caller
	{
		CallerT(Function&& function) : function(function) {}

		virtual void configure(ComponentsList&... component)
		{
			typedef typename stl::function_traits<Function>::params_tuple params_tuple;
			auto pack = std::tie(component...);
			static_assert(std::tuple_size<decltype(pack)>::value == std::tuple_size<params_tuple>::value, "spawner configuration function has undeclared spawned components");
			params_tuple* val = nullptr;
			stl::apply(function, stl::select_tuple_items(pack, val));
		}

		Function function;
	};

	Spawner* spawner;
};

template<class... ComponentsList>
struct ConfigurableSpawner : public ComplexSpawner<ComponentsList...>
{
	using Caller = typename ComplexSpawner<ComponentsList...>::Caller;

	struct Configurator : public SimpleConfiguratorT<ComponentsList...>
	{
	public:
		virtual void configure(Entity& spawned, ComponentsList&... component)
		{
			caller->configure(component...);
		}
		Caller* caller;
	};

	template<class Function>
	Entity* spawn(Function&& function)
	{
		typename ConfigurableSpawner::template CallerT<Function> invoker(std::forward<Function>(function));
		configurator.caller = &invoker;
		return this->spawner->spawn(configurator);
	}

private:
	Configurator configurator;
};

template<class... ComponentsList>
struct AccumulatingSpawner : public ComplexSpawner<ComponentsList...>
{
	using Caller = typename ComplexSpawner<ComponentsList...>::Caller;
	
	struct Configurator : public SimpleConfiguratorT<ComponentsList...>
	{
	public:
		virtual void configure(Entity& spawned, ComponentsList&... component)
		{
			caller->configure(component...);
			links.add(spawned, component...);
		}
		Caller* caller;
		ComponentLinkList<ComponentsList...> links;
	};

	auto& components() { return configurator.links; }

	template<class Function>
	Entity* spawn(Function&& function)
	{		
		typename AccumulatingSpawner::template CallerT<Function> caller(std::forward<Function>(function));
		configurator.caller = &caller;
		return this->spawner->spawn(configurator);
	}

	template<class ListPredicate, class Function>
	Entity* spawn(ListPredicate&& listPredicate, Function&& function)
	{
		for (auto& link : configurator.links)
		{
			if(!link(listPredicate))
				return nullptr;				
		}

		return spawn(std::forward<Function>(function));
	}

private:
	Configurator configurator;
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