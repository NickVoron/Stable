// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "class.h"
#include "factory.h"
#include "entityList.h"

std::ostream& operator<<(std::ostream& os, const Class& cls)
{
	return os << cls.debugstr();
}

stream::ostream& operator<<(stream::ostream& os, const Class& cls) { cls.save(os); return os; }
stream::istream& operator>>(stream::istream& is, Class& cls) { cls.load(is); return is; }

Entity& systemCreateClassInstance(const Class& cls, EntitiesList& entities)
{
	return cls.createInstance(entities);
}

void remapEntitiesToClassesLib(EntitiesList& source, const ClassesLib& classes)
{ 
	for (auto& entity : source)
	{
		auto& eclass = (Class&)entity.getClass();


		for (auto& cls : classes.classes)
		{

			if (eclass.equal(*cls))
			{

				eclass.index = cls->index;
				break;
			}
		}
	}
}





Class::ComponentMapping& Class::ComponentLinksMapping::step()
{
	return operator[](current++);
}

void Class::GlobalMappingEntry::create()
{
	if(cc > 0)
	{
		init(cc);
		cc = 0;
	}		
}	  




void Class::ComponentsGlobalMapping::create()
{
	init(ComponentsFactory::classesCount());
	zero();
}

void Class::initGlobalMapping()
{
	globalMapping.create();

	for(std::size_t i = 0; i < components.size(); ++i)
	{
		globalMapping[ componentClasses[i] ].cc++;
	}

	for(std::size_t i = 0; i < components.size(); ++i)
	{
		globalMapping[ componentClasses[i] ].create();
	}

	for(std::size_t i = 0; i < components.size(); ++i)
	{
		GlobalMappingEntry& e = globalMapping[ componentClasses[i] ];
		auto idx = e.cc++;
		e[idx].shift = components[i];
		e[idx].index = index;
	}
}

void Class::addComponent(const ComponentDesc& componentDesc, std::size_t index)
{
	if(index > 0)
	{
		components[index] = components[index - 1] + ComponentsFactory::sizeOf( componentClasses[index - 1] );
	}
	else
	{
		components[index] = 0;
	}

	auto clsIndex = ComponentsFactory::classIndex(componentDesc.className);
	CM_KERNEL_ENFORCE(clsIndex.second);

	names[index] = componentDesc.name;
	componentClasses[index] = clsIndex.first;
	instanceSize += ComponentsFactory::sizeOf( componentClasses[index] );
}


std::size_t Class::getComponentClassIndex(std::size_t componentIndex) const
{
	return componentClasses[componentIndex];
}


void Class::init(const ClassDesc& classDesc)
{
	instanceSize = 0;
	className = classDesc.className;
	auto componentsCount = classDesc.components.size();
	if(componentsCount > 0)
	{
		names.resize(componentsCount);
		components.init(componentsCount);
		componentClasses.init(componentsCount);
		executionUnits.resize(componentsCount);

		for(std::size_t i = 0; i < componentsCount; ++i)
		{
			addComponent(*classDesc.components[i], i);
		}

		initGlobalMapping();
		finalizeComponents(classDesc.components);
	}
	finalizeLocalObjects(classDesc.localObjects);
}


std::pair<std::size_t, bool> Class::findComponentIndex(const std::string& name) const
{
	for(std::size_t i = 0; i < components.size(); ++i)
	{
		if(names[i] == name)
			return std::make_pair(i, true);
	}

	return std::make_pair(-1, false);



}

std::pair<std::size_t, bool> Class::findFirstComponent(const std::string& clsId, std::size_t startIndex)
{
	CM_KERNEL_ENFORCE(startIndex <= components.size());

	for(std::size_t i = startIndex; i < components.size(); ++i)
	{
		if(ComponentsFactory::isInherited(clsId, ComponentsFactory::className( componentClasses[i] )) )
			return std::make_pair(i, true);
	}

	return std::make_pair(-1, false);
}

void Class::findIndicesOfComponents(const std::string& clsId, std::vector<std::size_t>& res) const
{
	res.reserve(components.size());

	for(std::size_t i = 0; i < components.size(); ++i)
	{
		if(ComponentsFactory::isInherited(clsId, ComponentsFactory::className( componentClasses[i] )) )
			res.push_back(i);
	}
}

std::size_t Class::localObjectIndex(const std::string& name) const
{
	std::size_t count = localObjects.size();
	for (std::size_t i = 0; i < count; ++i)
	{
		if(localObjects[i].objectName == name)
		{
			return i;
		}
	}

	return -1;
}

const Class& Class::localObjectClass(std::size_t objIdx) const
{
	CM_KERNEL_ENFORCE(objIdx >=0 && objIdx < (int)localObjects.size())
	return *(*classes).classes[ localObjects[objIdx].clsIndex ];
}

std::size_t Class::getComponentShift(std::size_t componentIndex) const
{
	CM_KERNEL_ENFORCE(componentIndex < components.size());
	return components[componentIndex];
}

std::size_t Class::getComponentsCount() const
{ 
	return components.size(); 
}


const std::string& Class::getComponentName(std::size_t idx) const
{ 
	return names[idx]; 
}

const Class::GlobalMappingEntry& Class::getComponentsByGlobalIndex(std::size_t componentGlobalIndex) const
{
	CM_KERNEL_ENFORCE(componentGlobalIndex < ComponentsFactory::classesCount());
	return globalMapping[componentGlobalIndex];
}

const std::string& Class::name() const 
{ 
	return className; 
}

std::size_t Class::getIndex() const
{ 
	return index; 
}

Entity* Class::spawn(Entity& parent) const
{
	INCOMPLETE;
	return spawner.spawn(parent);
}

bool Class::equal(const Class& other) const
{
	auto componentCount = getComponentsCount();
	if (componentCount == other.getComponentsCount())
	{
		if (localObjects.size() == other.localObjects.size())
		{
			for (std::size_t i = 0; i < componentCount; ++i)
			{
				if (getComponentClassIndex(i) != other.getComponentClassIndex(i))
					return false;
			}

			auto& classes0 = *classes;
			auto& classes1 = *other.classes;

 			auto objectsCount = localObjects.size();
 			for (std::size_t i = 0; i < objectsCount; ++i)
 			{
				if (!classes0.classes[localObjects[i].clsIndex]->equal(*classes1.classes[other.localObjects[i].clsIndex]))
 					return false;
 			}

			return true;
		}
	}

	return false;	
}

Entity& Class::createInstance(EntitiesList& entitiesList) const
{
	Entity& e = createInstance(entitiesList.add());
	for(std::size_t i = 0; i < localObjects.size(); ++i)
	{
		Entity& child = localObjectClass(i).createInstance(entitiesList);				   
	}
	return e;
}

Entity& Class::createInstance(Entity& e) const
{
	CM_KERNEL_ENFORCE(components.size() == linksTable.size());

	
	e.prepare(*this);
	for(std::size_t i = 0; i < components.size(); ++i)
	{
		
		ComponentBase& nc = e.getComponent(i);
		ComponentsFactory::invokeConstructor(componentClasses[i], &nc);
		fixPointersForComponent(&e, i);
	}

	bindToExecutionList(e);

	return e;
}

void Class::bindToExecutionList(Entity& target) const
{
	for (std::size_t i = 0; i < components.size(); ++i)
	{
		if (auto unit = executionUnits[i])
		{
			ComponentBase& nc = target.getComponent(i);
			unit->add(&nc);
		}
	}
}

void Class::fixPointersForComponent(Entity* entity, std::size_t componentIndex) const
{
	ComponentBase& nc = entity->getComponent(componentIndex);

	
	
	auto& links = linksTable[componentIndex];
	for (std::size_t mappingIdx = 0; mappingIdx < links.size(); ++mappingIdx)
	{
		auto& mapping = links[mappingIdx];
		ComponentBase** clientDataPtr = (ComponentBase**)(&((char*)&nc)[mapping.shiftFromClientComponentThis]);
		*clientDataPtr = &entity->getComponentByShift(mapping.shiftInEntityTable);
	}









	
	for (const auto& entityMapping : entityPointersTable[componentIndex])
	{
		Entity** eptr = (Entity**)(&((char*)&nc)[ entityMapping ]);
		*eptr = entity;
	}
	
}

void Class::finalizeComponents(const ClassDesc::ComponentsList& componentsList)
{
	if(components.size() > 0)
	{
		linksTable.init(components.size());
		entityPointersTable.init(components.size());

		for(std::size_t i = 0; i < components.size(); ++i)
		{
			const ComponentDesc& componentDesc = *componentsList[i];
			ComponentLinksMapping& mapping = linksTable[i];

			const FinalizeQuery& query = componentDesc.query;
			std::size_t count = query.entries.size();
			if(count > 0)
			{
				mapping.init(count);
				for(std::size_t j = 0; j < count; ++j)
				{
					const FinalizeQuery::Entry& entry = query.entries[j];
					auto index = componentDesc.linkIndex(entry.clsId(), entry.alias);
					if(!index.second)
					{
						index = findFirstComponent(entry.clsId(), 0);
						
						if (!index.second)
						{







						}
					}									

					if (index.second)
					{
						mapComponent(mapping, entry.pointerToComponentShift, components[index.first]);
					}					
				}
			}

			
			std::size_t size = query.parentEntries.size();
			if(size > 0)
			{
				entityPointersTable[i].init(size);
				for (std::size_t j = 0; j < size; ++j)
				{
					entityPointersTable[i][j] = (query.parentEntries[j].pointerToEntityShift);
				}	
			}					
		}
	}
}

void Class::mapComponent(ComponentLinksMapping& mapping, std::size_t componentShift, std::size_t shift)
{

	ComponentMapping& cm = mapping.step();
	cm.shiftFromClientComponentThis = componentShift;
	cm.shiftInEntityTable = shift;












}

void Class::finalizeLocalObjects(const ClassDesc::LocalObjects& locObjs)
{
	if(auto count = locObjs.size())
	{
		localObjects.init(count);

		for(std::size_t i = 0; i < count; ++i)
		{
			localObjects[i].clsIndex = locObjs[i].classId;
			localObjects[i].objectName = locObjs[i].objectName;
		}
	}	
}

void Class::save(stream::ostream& os) const
{
	linksTable.save(os);
	components.save(os);
	globalMapping.save(os);
	localObjects.save(os);
 	entityPointersTable.save(os);
 	componentClasses.save(os);

	os << index;
	os << className;
	os << names;
	os << instanceSize;
}

void Class::load(stream::istream& is)
{
 	linksTable.load(is);
 	components.load(is);
 	globalMapping.load(is);
 	localObjects.load(is);
 	entityPointersTable.load(is);
 	componentClasses.load(is);
	
	is >> index;
	is >> className;
	is >> names;	
	is >> instanceSize;

}

const ClassMapping::Entry& Class::map(const Class& target) const
{
	return classes->mapping.entry(*this, target);
}

std::size_t Class::findComponentShift(std::size_t componentClassIndex) const
{
	const GlobalMappingEntry& entry = getComponentsByGlobalIndex(componentClassIndex);
	if(entry.size() > 0)
	{
		return entry[0].shift;
	}

	CM_KERNEL_THROW(str::spaced("can't find component:", ComponentsFactory::className(componentClassIndex), "in class", name()) );
}

void Class::debug()	const
{
	LOG_MSG(debugstr());
}

std::string Class::debugstr() const
{
	str::spaced result("class:", className, "index:", index, "components: (", str::comma());
	for (std::size_t i = 0; i < componentClasses.size(); ++i)
	{
		result(ComponentsFactory::className(componentClasses[i]));
	}
	result(str::delimeter(), ")");

	return result;
}

void Class::LocalObject::save(stream::ostream& os) const
{
	os << objectName << clsIndex;
}

void Class::LocalObject::load(stream::istream& is)
{
	is >> objectName >> clsIndex;
}




ClassMapping::Entry::Entry(const Class& class0, const Class& class1)
{
	create(class0, class1);
}

void ClassMapping::Entry::create(const Class& class0, const Class& class1)
{
	classes[0] = &class0;
	classes[1] = &class1;

	
	auto count0 = class0.getComponentsCount();
	auto count1 = class1.getComponentsCount();
	const Class& fetch = count0 < count1 ? class0 : class1;
	auto count = std::min(count0, count1);

	if(count > 0)
	{
		data.init(count);
	}	

	for(std::size_t i = 0; i < count; ++i)
	{
		Components& components = data[i];

		auto gi =  fetch.getComponentClassIndex(i);
		components.globalComponentIndex = gi;

		const Class::GlobalMappingEntry& shifts0 = class0.getComponentsByGlobalIndex(gi);
		const Class::GlobalMappingEntry& shifts1 = class1.getComponentsByGlobalIndex(gi);

		auto count0 = shifts0.size();
		auto count1 = shifts1.size();

		if(count0 > 0 && count1 > 0)
		{
			components.shifts[0].init( count0 );
			components.shifts[1].init( count1 );

			for(std::size_t j = 0; j < count0; ++j)
			{
				components.shifts[0][j] = shifts0[j].shift;
			}

			for(std::size_t j = 0; j < count1; ++j)
			{
				components.shifts[1][j] = shifts1[j].shift;
			}
		}
	}
}
								   
void ClassMapping::Entry::map(const EntityData& source, EntityData& target, MappingFunction mappingFunction) const
{
	int idx0 = (source.cls == classes[0]) ? 0 : 1;
	int idx1 = (target.cls == classes[1]) ? 1 : 0;
	CM_KERNEL_ENFORCE(idx0 != idx1);
	
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		auto& sourceIdx = data[i].shifts[idx0];
		auto& targetIdx = data[i].shifts[idx1];
		auto sourceCount = sourceIdx.size();
		auto targetCount = targetIdx.size();

		for (std::size_t s = 0; s < sourceCount; ++s)
		{
			ComponentBase& src = source.getComponentByShift(sourceIdx[s]);

			for (std::size_t t = 0; t < targetCount; ++t)
			{
				ComponentBase& trg = target.getComponentByShift(targetIdx[t]);

				mappingFunction(src, trg);
			}
		}
	} 	
}

void ClassMapping::clear()
{					
	entries.clear();
}

const ClassMapping::Entry& ClassMapping::entry(const Class& class0, const Class& class1)
{
	Entry* entry = find(class0, class1);

	return entry ? *entry : add(class0, class1);
}

ClassMapping::Entry& ClassMapping::add(const Class& class0, const Class& class1)
{
	entries.emplace_back(new Entry(class0, class1));

	return *entries.back().get();
}

ClassMapping::Entry* ClassMapping::find(const Class& class0, const Class& class1)
{
	for (auto& entry : entries)
	{
		if( ((entry->classes[0] == &class0) && (entry->classes[1] == &class1)) ||
			((entry->classes[0] == &class1) && (entry->classes[1] == &class0)))
		{
			return entry.get();
		}
	}

	return nullptr;
}



// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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