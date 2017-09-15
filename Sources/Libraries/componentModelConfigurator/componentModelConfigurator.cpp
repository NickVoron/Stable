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
#include "componentModelTesting/library.include.h"
#include "common/ptr_utils.h"
#include "common/breakpoint.h"




namespace ComponentModel
{


	struct HierarchyNode
	{
		ObjectParser::InstanceHandle* parent = nullptr;
		ObjectParser::InstanceHandle* self = nullptr;
	};

	std::vector<HierarchyNode> linearize(HierarchyNode parent, Expressions::EvaluatedScope& scope)
	{
		std::vector<HierarchyNode> result;

		for (auto& unit : scope)
		{
			if (auto instanceHandle = unit.second->cast<ObjectParser::InstanceHandle>())
			{
				result.push_back({ parent.self, instanceHandle });
				auto children = linearize(result.back(), *instanceHandle);
				result.insert(result.end(), children.begin(), children.end());
			}
			else if (auto instances = unit.second->cast<Expressions::EvaluatedArray>())
			{
				for (std::size_t i = 0; i < instances->count(); ++i)
				{
					if (auto instanceHandle = const_cast<Expressions::EvaluationUnit*>(instances->element(i))->cast<ObjectParser::InstanceHandle>())
					{
						result.push_back({ parent.self, instanceHandle });
						auto children = linearize(result.back(), *instanceHandle);
						result.insert(result.end(), children.begin(), children.end());
					}
				}
			}
		}

		return result;
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
					if (auto prop = resource->findProperty(a.first.c_str()))
					{
						ENFORCE(a.second);
						
						prop->convert(component, *a.second);
					}
					else
					{
						LOG_ERROR("component: " << ComponentsFactory::className(component) << " hasn't property: " << a.first);
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
			LOG_ERROR(e);
			throw;
		}
	}

	EntitiesList& initializeEntities(Expressions::EvaluatedScope& scope, EntitiesList& entities)
	{
		auto objects = linearize(HierarchyNode(), scope);
		

		std::map<ObjectParser::InstanceHandle*, ClassDesc*> objectToClassDesc;
		ClassDescList classes;
		for (auto& object : objects)
		{
			ClassDesc& classDesc = classes.add(object.self->type);
			objectToClassDesc[object.self] = &classDesc;
			for (auto* componentHandle : object.self->components())
			{
				if (object.self->isClassMember(componentHandle))
				{
					classDesc.addComponent(componentHandle->type, componentHandle->name, "");
				}
			}

			LOG_MSG("");
			classDesc.debug();
		}

		classes.finalize();
		entities.classes.create(classes, entities.executionList);
		ObjectParser::ObjectConverter::classes = &entities.classes;

		std::map<ObjectParser::InstanceHandle*, Entity*> objectToEntity;
		for (std::size_t objectIdx = 0; objectIdx < objects.size(); ++objectIdx)
		{
			auto& object = objects[objectIdx];

			auto classDesc = objectToClassDesc[object.self];
			auto classIndex = classDesc->classIndex;
			auto entity = entities.create(classIndex);
			
			objectToEntity[object.self] = entity;
			std::vector<ObjectParser::ComponentHandle*> componentHandles;
			for (std::size_t i = 0; i < entity->getComponentsCount(); ++i)
			{
				std::string componentName = entity->getClass().getComponentName(i);
				auto componentHandle = object.self->component(componentName);
				ENFORCE(componentHandle);
				componentHandles.push_back(componentHandle);
				componentHandle->objectIndex = objectIdx;
				componentHandle->componentIndex = i;
			}
		}

		for (std::size_t objectIdx = 0; objectIdx < objects.size(); ++objectIdx)
		{
			auto& object = objects[objectIdx];
			auto entity = objectToEntity[object.self];
			for (std::size_t i = 0; i < entity->getComponentsCount(); ++i)
			{
				std::string componentName = entity->getClass().getComponentName(i);
				auto componentHandle = object.self->component(componentName);
				ENFORCE(componentHandle);
				initComponent(entity->getComponent(i), *componentHandle);
			}
			entity->finalize();
		}

		for (auto& object : objects)
		{
			if (object.parent)
			{
				auto parent = objectToEntity[object.parent];
				ENFORCE(parent);
				ENFORCE(object.self);
				auto self = objectToEntity[object.self];
				ENFORCE(self);
				parent->map(*self, &ComponentsFactory::inheriteProperties);
			}
		}

		entities.classes.finalize(entities.executionList);

		for (auto& entity : entities)
		{
			entity.getClass().bindToExecutionList(entity);
		}

		return entities;
	}
	
	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities)
	{
		ENFORCE(filename);
		ObjectParser::Compiler comp(filename);
		auto& classTable = comp.result.classes();
		ObjectParser::ObjectConverter::classTable = &classTable;
		auto result = ObjectParser::unroll(classTable, "Main", "main");
		return initializeEntities(result, entities);		
	}

	void descriptionTest(const char* filename)
	{
		ENFORCE(filename);

		EntitiesList entities;
		descriptionLoad(filename, entities).activate(true);
		

		for (int i = 0; i < 3; ++i)
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