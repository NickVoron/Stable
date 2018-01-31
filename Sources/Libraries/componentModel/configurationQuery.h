// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "common/function_traits.h"

#include "entity.h"
#include "class.h"

template<int maxComponentsCount>
struct ConfigurationQueryBase
{
	static const int componentsCount = maxComponentsCount;

	struct Entry;

	template<class ComponentType>
	struct Handle 
	{
		Handle():entry(0), query(0){}
		Handle(Entry* e, ConfigurationQueryBase* q):entry(e), query(q){}
		
		ComponentType& operator()(EntityData* entity)	{ CM_KERNEL_ENFORCE(entity); return component(*entity); }
		ComponentType& operator()(EntityData& entity)	{ return component(entity);	}

		ComponentType& component(EntityData& entity)
		{
			CM_KERNEL_ENFORCE(query);
			CM_KERNEL_ENFORCE(entry);

			query->execute(entity);
			CM_KERNEL_ENFORCE(entry->input.classIndex == ComponentType::ClassIndex());
			CM_KERNEL_ENFORCE(entry->result.componentShift>=0);
			ComponentBase* res = &entity.getComponentByShift(entry->result.componentShift);

			return *(ComponentType*)res;
		}

		ConfigurationQueryBase* query;
		Entry* entry;
	};

	struct Entry
	{
		template<class ComponentType>
		void init()
		{
			input.classIndex = ComponentType::ClassIndex();
		}

		void execute(const Class& cls)
		{

			result.invalidate();
			result.componentShift = cls.findComponentShift(input.classIndex);
		}	

		struct Input 
		{
			std::size_t classIndex;
		};

		struct Result
		{
			Result() { invalidate(); }
			void invalidate() { componentShift = -1; }
			std::size_t componentShift;
		};

		Input input;
		Result result;
	};

	template<class ComponentType> 
	Handle<ComponentType> add()
	{
		Entry& entry = entries.addEntry();
		entry.template init<ComponentType>();
		return Handle<ComponentType>(&entry, this); 
	}

	void execute(const Class& cls)
	{
		if(bindedClass != &cls)
		{
			bindedClass = nullptr;

			for (std::size_t i = 0; i < entries.size(); ++i)
			{
				entries[i].execute(cls);				
			}

			bindedClass = &cls;
		}
	}

	void execute(EntityData& entity)
	{
		CM_KERNEL_ENFORCE(entity.cls)
		execute(*entity.cls);
	}

	Base::FixedArray<Entry, maxComponentsCount> entries;
	const Class* bindedClass = nullptr;
};


struct ConfigurationQuery : public ConfigurationQueryBase<8>{};

template<class... ComponentsList>
struct SimpleConfigurationQuery : public ConfigurationQueryBase<sizeof...(ComponentsList)>
{
	SimpleConfigurationQuery()
	{
		auto sz = { ((std::get<typename SimpleConfigurationQuery::template Handle<ComponentsList>>(handles) = this->template add<ComponentsList>()), 0)... };
	}

	template<class ComponentType> ComponentType& get(EntityData& entity) { return std::get<typename SimpleConfigurationQuery::template Handle<ComponentType>>(handles)(entity); }
	template<class ComponentType> ComponentType& get(EntityData* entity) { return std::get<typename SimpleConfigurationQuery::template Handle<ComponentType>>(handles)(*entity); }

	std::tuple<typename SimpleConfigurationQuery::template Handle<ComponentsList>...> handles;
};

template<class Function, bool has_entity_arg = std::is_same<typename stl::function_traits<Function>::template arg<0>::type, Entity&>::value>
struct ConfigurePack;

template<class Function>
struct ConfigurePack<Function, true>
{
	template<class... ComponentsList>
	static decltype(auto) pack(Entity& entity, ComponentsList&... components)
	{
		return std::tie(entity, components...);
	}
};

template<class Function>
struct ConfigurePack<Function, false>
{
	template<class... ComponentsList>
	static decltype(auto) pack(Entity& entity, ComponentsList&... components)
	{
		return std::tie(components...);
	}
};


template<class... ComponentsList>
class SimpleConfigurator
{
public:
	template<class... ConfigurationFunctions>
	void operator()(Entity& spawned, ConfigurationFunctions&&... functions)
	{ 
		configure(spawned, this->template component<ComponentsList>(spawned)..., std::forward<ConfigurationFunctions>(functions)...);
	}

	template<class ComponentType> ComponentType& component(EntityData& entity) {					return query.template get<ComponentType>(entity); }
	template<class ComponentType> ComponentType& component(EntityData* entity) { ENFORCE(entity);	return this->template component<ComponentType>(*entity);}

	template<class... ConfigurationFunctions>
	void configure(Entity& entity, ComponentsList&... components, ConfigurationFunctions&&... functions)
	{
		auto sz = { (configure_impl(entity, components..., functions), 0)... };
	}

protected:
	template<class Function>
	void configure_impl(Entity& entity, ComponentsList&... components, Function&& function)
	{
		auto pack = ConfigurePack<Function>::pack(entity, components...);
		typedef typename stl::function_traits<Function>::params_tuple params_tuple;
		static_assert(std::tuple_size<decltype(pack)>::value == std::tuple_size<params_tuple>::value, "spawner configuration function has undeclared spawned components");
		params_tuple* val = nullptr;
		stl::apply(function, stl::select_tuple_items(pack, val));
	}

	SimpleConfigurationQuery<ComponentsList...> query;
};



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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