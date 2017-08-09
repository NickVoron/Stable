// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentModelConfigurator.h"
#include "descParser/library.include.h"
#include "resourceUtils/library.include.h"
#include "common/ptr_utils.h"
#include "common/breakpoint.h"




namespace ComponentModel
{

	void ComponentModelConfigurator::InstanceHandle::debug(int& depth) const
	{
		LOG_MSG(logs::spaces(depth++) << " " << name);

		if (!components.empty())
		{
			LOG_MSG(logs::spaces(depth) << " components: " << components.size());
			for (auto&& comp : components)
			{
				LOG_MSG(logs::spaces(depth + 1) << " " << comp.first->component->className);
				LOG_MSG(logs::spaces(depth + 1) << "properties: " << comp.second.properties.size());
					
				for (auto&& property : comp.second.properties)
				{			
					LOG_MSG(logs::spaces(depth + 2) << property.debug());
				}
			}
		}	

		if (!instances.empty())
		{
			LOG_MSG(logs::spaces(depth) << " instances: " << instances.size() );
			for (auto inst : instances)
			{
				int dpth = depth;
				inst->debug(dpth);
			}
		}
	}

	template<class MapType>
	void initComponent(ComponentBase& component, const MapType& properties)
	{
		try
		{
			const ResourceTable* resource = ComponentsFactory::resourceTable(component.classIndex());
			ENFORCE(resource);

			for (auto& a : properties)
			{
				try
				{
					if (auto prop = resource->findProperty(a.name.c_str()))
					{
						ENFORCE(a.expression);
						
						prop->convert(component, *a.expression);
					}
					else
					{
						LOG_ERROR("component: " << ComponentsFactory::className(component) << " hasn't property: " << a.name);
					}
				}
				catch (std::exception& e)
				{
					LOG_ERROR("initComponent failed: \n\t" << e.what());
				}
			}
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e.what());
			throw;
		}
	}

	void initInstance(const ComponentModelConfigurator::InstanceHandle* instance, const Entity* parent, Entity*& entity)
	{
		ENFORCE(instance && entity);
		ENFORCE_EQUAL(instance->classDesc->className, entity->getClass().name());
		ENFORCE_EQUAL(instance->components.size(), entity->getComponentsCount());

		
		for (auto& source : instance->initData())
		{
			try
			{
				auto& component = entity->getComponent(std::get<1>(std::get<0>(source)));
				auto& properties = std::get<1>(source);
				initComponent(component, properties);
			}
			catch (std::exception& e)
			{
				LOG_ERROR(e);
			}					
		}
		

		entity->finalize();

		if (parent)
		{
			parent->map(*entity, &ComponentsFactory::inheriteProperties);
		}

		Entity* prnt = entity;
		for (auto&& child : instance->instances)
		{
			entity->iterate(entity);
			initInstance(child, prnt, entity);
		}
	}	

	ComponentModelConfigurator::ComponentState& ComponentModelConfigurator::ClassState::componentState()
	{
		ENFORCE(!components.empty());
		return components.top();
	}

	ComponentModelConfigurator::ClassState& ComponentModelConfigurator::classState()
	{
		ENFORCE(!state.empty());
		return state.top();
	}

	ClassDesc* ComponentModelConfigurator::currentClass()
	{
		return classState().current;
	}

	ComponentDesc* ComponentModelConfigurator::currentComponent()
	{
		return classState().componentState().current;
	}

	void ComponentModelConfigurator::beginCreateInstance(const std::string& name, const std::string& type)
	{

		state.push(ClassState());
		auto instanceClass = &classes.add(type);
		classState().current = instanceClass;
		auto instance = Expressions::add<InstanceHandle>(instanceCounter, currentInstance, name, instanceClass);
		classState().instance = instance;
		currentInstance = instance;
 		++instanceCounter;
 		++level;
	}

	ObjectParser::InstanceHandle* ComponentModelConfigurator::endCreateInstance()
	{
		--level;



		auto instance = classState().instance;
		
		
		if (auto parentClassDesc = instance->parent->classDesc)
		{
			parentClassDesc->localObjects.push_back({ instance->name, instance->classDesc->className, instance->classDesc->classIndex });
		}
		
		
		currentInstance = instance->parent;
		state.pop();
		return instance;
	}

	ObjectParser::ComponentHandle* ComponentModelConfigurator::preCreateComponent(const std::string& type, const std::string& name)
	{
		classState().components.push(ComponentState());
		auto& componentState = classState().componentState();
		componentState.current = classState().current->addComponent(type, name, componentState.runtimeProperties.value("category"));
		
		ComponentHandle* comp = Expressions::add<ComponentHandle>(currentInstance, currentComponent());
		
		return comp;
	}

	void ComponentModelConfigurator::beginCreateComponent(ObjectParser::ComponentHandle* handle, const std::string& type, const std::string& name)
	{


		++level;
	}

	void ComponentModelConfigurator::endCreateComponent(ObjectParser::ComponentHandle* inhandle)
	{
		--level;

		auto instance = classState().instance;
		auto current = classState().componentState().current;


		ComponentHandle* handle = dynamic_cast<ComponentHandle*>(inhandle);

		instance->components[handle] = classState().componentState();
		classState().components.pop();
		auto& comps = classState().components;
	}


	void ComponentModelConfigurator::bindRuntimeProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const std::string& value)
	{
		if (name == "category")
		{
			classState().componentState().runtimeProperties.emplace_back(name, value);
			classState().current->categories.insert(value);
		}
	}

	void ComponentModelConfigurator::bindLink(ObjectParser::ComponentHandle* handle, const std::string& name, const ObjectParser::ComponentHandle* value)
	{
		const ComponentHandle* valueHandle = dynamic_cast<const ComponentHandle*>(value);
		ENFORCE(valueHandle && valueHandle->component);
		currentComponent()->dependencies.push_back(valueHandle->component);
	}

	void addDependencies(std::vector<ComponentDesc*>& dependencies, const Expressions::Expression* value)
	{
		if (auto array = value->cast<Expressions::Array>())
		{
			for (std::size_t i = 0; i < array->count(); ++i)
			{
				addDependencies(dependencies, array->element(i));
			}
		}
		else if (auto handle = value->cast<ComponentModelConfigurator::ComponentHandle>())
		{
			ENFORCE(handle->component);
			dependencies.push_back(handle->component);
		}
	}

	void ComponentModelConfigurator::bindComponentProperty(ObjectParser::ComponentHandle* handle, const std::string& name, const Expressions::Expression* value)
	{
		if (value)
		{
			if (auto proxy = value->cast<Expressions::Proxy>())
			{
				THROW("Expression must not be Proxy");
			}

			classState().componentState().properties.emplace_back(name, value);
			addDependencies(currentComponent()->dependencies, value);
		}	
		else
		{
			LOG_ERROR("bindComponentProperty: " << name << " value is nullptr ");
		}
	}

	void ComponentModelConfigurator::debug() const
	{
		for (auto instance : root.instances)
		{
			int depth = 0;
			instance->debug(depth);
		}
	}

	EntitiesList& ComponentModelConfigurator::configure(ObjectParser::Unroller& unroller, EntitiesList& entities)
	{
		classes.finalize();
		entities.classes.create(classes, entities.executionList);
		ENFORCE_EQUAL(classes.size(), entities.classes.classes.size());
		configure(unroller, root, entities);
		ENFORCE_EQUAL(entities.pool.entitiesCount(), instanceCounter);

		return entities;
	}










	void createInstance(const ComponentModelConfigurator::InstanceHandle& handle, EntitiesList& entities)
	{
		if (handle.classDesc)
		{
			auto classIndex = handle.classDesc->classIndex;
			auto* entity = entities.create(classIndex);
			auto count = entities.pool.entitiesCount();



			initInstance(&handle, nullptr, entity);
		}
	}











	EntitiesList& ComponentModelConfigurator::configure(ObjectParser::Unroller& unroller, const InstanceHandle& rootInstance, EntitiesList& entities)
	{
		scoped_pointer_assigner<ObjectParser::Unroller> urptr(ObjectParser::ObjectConverter::unroller, unroller);



		createInstance(rootInstance, entities);

 		for (auto& instance : rootInstance.instances)
 		{
 			ENFORCE(instance);
 			createInstance(*instance, entities);
 		}

		entities.classes.finalize(entities.executionList);
		
		for (auto& entity : entities)
		{
			entity.getClass().bindToExecutionList(entity);
		}
		
		return entities;
	}

	EntitiesList& createInstance(const ObjectParser::InstanceDefinitionExpression& instance, ObjectParser::Unroller& unroller, ComponentModelConfigurator& configurator, EntitiesList& entities, bool debug)
	{
		auto scope = Expressions::ScopeNames();
		unroller.unrollInstance(&instance, scope);

		if (debug)
		{
			configurator.debug();
		}

		scoped_pointer_assigner<ClassesLib> clptr(ObjectParser::ObjectConverter::classes, entities.classes);
		
		return configurator.configure(unroller, entities);
	}
	
	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities)
	{
		ENFORCE(filename);

		ObjectParser::Compiler comp(filename);
		ComponentModelConfigurator configurator;
		
		const char* entryPoint = "Main";
		ObjectParser::Unroller unroller(comp.result.classes(), configurator, true);
		createInstance(ObjectParser::InstanceDefinitionExpression(entryPoint, entryPoint), unroller, configurator, entities, false);
		
		return entities;
	}
	
	void descriptionTest(const char* filename)
	{
		ENFORCE(filename);

		EntitiesList entities;
		descriptionLoad(filename, entities);
		entities.activate(true);
		

		for (int i = 0; i < 5; ++i)
		{
			entities.execute();
		}
	}
}





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