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

namespace ComponentModel
{
	bool isQueryInclude(const FinalizeQuery& query, const std::string& classId, int index, int& queryEntry)
	{
		for (int i = 0; i < query.entries.size(); ++i)
		{
			const FinalizeQuery::Entry& entry = query.entries[i];

			if (ComponentsFactory::isInherited(entry.clsId(), classId))
			{
				if (entry.index == index)
				{
					queryEntry = i;
					return true;
				}
			}
		}

		return false;
	}

	bool mayBeIsAlias(const FinalizeQuery& query, const std::string& alias)
	{
		for (int i = 0; i < query.entries.size(); ++i)
		{
			const FinalizeQuery::Entry& entry = query.entries[i];
			if (!entry.alias.empty())
			{
				if (alias == entry.alias)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool isQueryIncludeWithAlias(const FinalizeQuery& query, const std::string& alias, const std::string& classId, int index, int& queryEntry)
	{
		for (int i = 0; i < query.entries.size(); ++i)
		{
			const FinalizeQuery::Entry& entry = query.entries[i];
			if (!entry.alias.empty())
			{
				if (alias == entry.alias)
				{
					if (ComponentsFactory::isInherited(entry.clsId(), classId))
					{
						if (entry.index == index)
						{
							queryEntry = i;
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	struct DescLink
	{
		std::string name;
		std::string className;
		std::string alias;
	};

	bool isGoodForQuery(DescLink& ilink, const std::string& componentClassId, const FinalizeQuery& query, const ObjectParser::InstanceHandle& instance, int& queryEntry)
	{
		queryEntry = -1;

		LOG_MSG("\t\tlink: " << ilink.name);
		const ObjectParser::ComponentHandle* link = instance.component(ilink.name);
		if (!link)
		{
			LOG_ERROR("component try to link with not existed component: " << "class name: " << std::quoted(instance.typeName())  << " link name: " << std::quoted(ilink.name));
			return false;
		}
		else if (!ComponentsFactory::isInherited(ilink.className, link->type))
		{
			bool isAlias = false;
			if (mayBeIsAlias(query, ilink.className))
			{
				if (!isQueryIncludeWithAlias(query, ilink.className, link->type, 0, queryEntry))
				{
					LOG_ERROR("alias failed");
					return false;
				}

				ilink.alias = ilink.className;
				isAlias = true;
			}
			else if (!isComponentRegistred(ilink.className))
			{
				LOG_ERROR("trying lo link with ñîmponent of undefined class: " << ilink.className);
				return false;
			}

			if (!isAlias)
			{
				LOG_ERROR("can't convert link class: \n\tfrom: " << link->type << "\n\tto: " << ilink.className << "\n\tcheck your components hierarchy");
				return false;
			}
		}
		else if (!isQueryInclude(query, link->type, 0, queryEntry))
		{
			LOG_ERROR("query for component: " << componentClassId << " not include link: " << ilink.className);
			return false;
		}

		ilink.className = link->type;

		return true;
	}

	bool solveLinks(const ObjectParser::ComponentHandle& component, const ObjectParser::InstanceHandle& instance, ComponentDesc& compDesc)
	{
		int querySize = compDesc.query.entries.size();
		std::vector<bool> links(querySize, false);

		
		for (auto& param : component.links)
		{
			auto handle = param.second->cast<const ObjectParser::ComponentHandle>();
			if(handle)
			{
				int idx = -1;
				DescLink descLink{ handle->name, handle->type };
				if (isGoodForQuery(descLink, compDesc.className, compDesc.query, instance, idx))
				{
					LOG_MSG("link resolved: " << handle->name << " : " << handle->type << idx);
					
				}

				links[idx] = (idx >= 0);
			}
		}

		
		for (int k = 0; k < querySize; ++k)
		{
			if (!links[k])
			{
				for (auto& comp : instance.components())
				{
					if (ComponentsFactory::isInherited(compDesc.query.entries[k].clsId(), component.type))
					{
						LOG_MSG("ComponentsFactory::isInherited(compDesc.query.entries[k].clsId(), component.type)");
						
						links[k] = true;
						break;
					}
				}
			}
		}

		
		bool res = true;
		for (int k = 0; k < querySize; ++k)
		{
			if (!links[k])
			{
				LOG_ERROR("Can't link class: " << instance.typeName() << " component: " << compDesc.className << " can't find component: " << compDesc.query.entries[k].clsId());
			}

			res &= links[k];
		}

		return res;
	}


	struct HierarchyNode
	{
		const ObjectParser::InstanceHandle* parent = nullptr;
		const ObjectParser::InstanceHandle* self = nullptr;
		unsigned int level = 0;
	};
	
	template<class NodeVisitor>
	void linearize(HierarchyNode parent, const Expressions::EvaluatedScope& scope, NodeVisitor&& visitor)
	{
		auto processNode = [](HierarchyNode parent, ObjectParser::InstanceHandle* instanceHandle, NodeVisitor&& visitor)
		{
			HierarchyNode node{ parent.self, instanceHandle, parent.level + 1 };
			visitor(node);
			linearize(node, instanceHandle->scope(), std::forward<NodeVisitor>(visitor));
		};

		for (auto& unit : scope)
		{
			if (auto instanceHandle = unit.second->cast<ObjectParser::InstanceHandle>())
			{
				if ((parent.self && parent.self->scope().isClassMember(instanceHandle)) || !parent.self)
				{
					processNode(parent, instanceHandle, std::forward<NodeVisitor>(visitor));
				}
			}
			else if (auto instances = unit.second->cast<Expressions::ArrayContainer>())
			{
				for (std::size_t i = 0; i < instances->count(); ++i)
				{
					if (auto instanceHandle = instances->element(i)->cast<ObjectParser::InstanceHandle>())
					{
						processNode(parent, instanceHandle, std::forward<NodeVisitor>(visitor));
					}
				}
			}
		}
	}

	std::vector<HierarchyNode> linearize(const Expressions::EvaluatedScope& scope)
	{
		HierarchyNode parent;
		std::vector<HierarchyNode> result;
		linearize(parent, scope, [&result](auto& node)
		{
			result.push_back(node);
		});
		return result;
	}

	void print_objects(const Expressions::EvaluatedScope& scope)
	{
		HierarchyNode parent;
		linearize(parent, scope, [](auto& node)
		{
			if(node.self)
			{
				LOG_MSG(logs::tabs(node.level) << node.self->string());
			}			
		});
	}

	template<class MapType>
	void initComponent(ComponentBase& component, const MapType& properties, ExternalComponentLinks& externalLinks)
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

						
						if (auto handle = a.second->template cast<ObjectParser::ComponentHandle>(); handle)
						{
							auto ext_it = externalLinks.find(handle);
							if (ext_it != externalLinks.end())
							{
								
								prop->evaluateExternalLink(ext_it->second, component, *a.second);
								continue;
							}
						}
						
						
						prop->convert(component, *a.second);
						
					}
					else
					{
						
					}
				}
				catch (std::exception& e)
				{
					LOG_ERROR("initComponent failed: \n\t" << e);
				}
			}
		}
		catch (std::exception& e)
		{
			LOG_ERROR(e);
			throw;
		}
	}

	EntitiesList& initializeEntities(const Expressions::EvaluatedScope& scope, EntitiesList& entities, ExternalComponentLinks& externalLinks)
	{
		auto objects = linearize(scope);
		
		
		std::map<const ObjectParser::InstanceHandle*, ClassDesc*> objectToClassDesc;
		ClassDescList classes;
		for (auto& object : objects)
		{
			ClassDesc& classDesc = classes.add(object.self->definition.type);
			objectToClassDesc[object.self] = &classDesc;
			for (auto* componentHandle : object.self->components())
			{
				
				classDesc.addComponent(componentHandle->type, componentHandle->name, "");
			}
		}
		

		classes.finalize();
		entities.classes.create(classes, entities.executionList);

		std::map<const ObjectParser::InstanceHandle*, Entity*> objectToEntity;
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
				
				componentHandles.push_back(const_cast<ObjectParser::ComponentHandle*>(componentHandle));
				const_cast<ObjectParser::ComponentHandle*>(componentHandle)->objectIndex = objectIdx;
				const_cast<ObjectParser::ComponentHandle*>(componentHandle)->componentIndex = i;
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
				initComponent(entity->getComponent(i), componentHandle->scope(), externalLinks);
			}
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

		return entities;
	}

	EntitiesList& initializeEntities(const Expressions::EvaluatedScope& scope, EntitiesList& entities)
	{
		ExternalComponentLinks externalLinks;
		return initializeEntities(scope, entities, externalLinks);
	}
	
	EntitiesList& descriptionLoad(const char* filename, EntitiesList& entities)
	{
		Base::Timer timer;
		ENFORCE(filename);
		ObjectParser::Compiler comp(filename);
		auto result = ObjectParser::unroll(comp.result.classes(), entities, "Main", "main");
		initializeEntities(*result.get(), entities);

		
		
		for (auto& entity : entities)
		{
			
			entity.getClass().bindToExecutionList(entity);
		}

		
		return entities;
	}

	void descriptionCompile(const char* filename)
	{
		ENFORCE(filename);
		ObjectParser::Compiler comp(filename);
		auto worldScopename = ObjectParser::unroll(comp.result.classes(), "Main", "main");
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