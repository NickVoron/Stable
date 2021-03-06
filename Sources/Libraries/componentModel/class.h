// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "entity.h"
#include "desc.h"
#include "factory.h"



class Class;
class ClassesLib;

class ClassMapping
{
public:
	struct Components
	{
		std::size_t globalComponentIndex;
		cm_array<std::size_t> shifts[2];
	};
																						 
	struct Entry
	{
		Entry(const Class& class0, const Class& class1);
		void create(const Class& class0, const Class& class1);
		void map(const EntityData& source, EntityData& target, MappingFunction mappingFunction) const;

		const Class* classes[2];
		cm_array<Components> data;
	};

	void clear();
	const Entry& entry(const Class& class0, const Class& class1);

private:
	Entry& add(const Class& class0, const Class& class1);
	Entry* find(const Class& class0, const Class& class1);

	std::vector<std::unique_ptr<Entry>> entries;
};

Entity& systemCreateClassInstance(const Class& cls, EntitiesList& entities);
void remapEntitiesToClassesLib(EntitiesList& source, const ClassesLib& classes);

class Class
{
friend Entity& systemCreateClassInstance(const Class& cls, EntitiesList& entities);
friend void remapEntitiesToClassesLib(EntitiesList& source, const ClassesLib& classes);
friend class EntityData;
friend class ClassesLib;
friend class ClassMapping;
friend class SystemSpawner;
friend class EntitiesList;
friend class ClassResources;
public:
	
	struct LocalObject 
	{
		void save(stream::ostream& os) const;
		void load(stream::istream& is);

		std::size_t clsIndex;
		std::string objectName;		
	};

	struct LocalObjects : public cm_array<LocalObject> {};

public:
	void init(const ClassDesc& clsDesc);

	std::pair<std::size_t, bool> findComponentIndex(const std::string& name) const;
	void findIndicesOfComponents(const std::string& clsId, std::vector<std::size_t>& res) const;
	std::size_t findComponentShift(std::size_t componentClassIndex) const;

	std::size_t localObjectIndex(const std::string& name) const;
	const Class& localObjectClass(std::size_t objIdx) const;

	std::size_t getComponentShift(std::size_t componentIndex) const;

	std::size_t getComponentsCount() const;
	std::size_t getComponentClassIndex(std::size_t componentIndex) const;

	const std::string& getComponentName(std::size_t idx) const;

	const std::string& name() const;
	std::size_t getIndex() const;

	void spawn(Entity& parent) const;

	bool equal(const Class& other) const;

	const ClassMapping::Entry& map(const Class& target);

	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	void debug() const;
	std::string debugstr() const;

	ClassesLib* classes = nullptr;

	void bindToExecutionList(Entity& target) const;

private:
	struct EntityMapping : cm_array<ptrdiff_t>
	{
	};

	struct ComponentMapping
	{
		std::size_t shiftInEntityTable;
		std::size_t shiftFromClientComponentThis;
	};

	class ComponentLinksMapping : public cm_array<ComponentMapping>
	{
	public:
		ComponentMapping& step();

	private:
		std::size_t current = 0;
	};

	struct LinksTable : public cm_array<ComponentLinksMapping>{};
	struct EntityPointersInComponentsTable : public cm_array<EntityMapping>{};

	struct MappingValues
	{
		std::size_t shift;
		std::size_t index;
	};

	struct GlobalMappingEntry : public cm_array<MappingValues>
	{ 
		void create();
		std::size_t cc;
	};

	struct ComponentsGlobalMapping : public cm_array<GlobalMappingEntry>
	{
		void create();
	};

	Entity& createInstance(EntitiesList& entitiesList) const;
	Entity& createInstance(Entity& target) const;

	void addComponent(const ComponentDesc& componentDesc, std::size_t index);
	
	void finalizeComponents(const ClassDesc::ComponentsList& components);
	void finalizeLocalObjects(const ClassDesc::LocalObjects& localObjects);

	void mapComponent(ComponentLinksMapping& mapping, std::size_t componentShift, std::size_t shift);
	void fixPointersForComponent(Entity* entity, std::size_t componentIndex) const;

	std::pair<std::size_t, bool> findFirstComponent(const std::string& clsId, std::size_t startIndex);
	
	const GlobalMappingEntry& getComponentsByGlobalIndex(std::size_t componentGlobalIndex) const;
									
	void initGlobalMapping();

	
	LinksTable linksTable;
	EntityPointersInComponentsTable entityPointersTable; 	

	ComponentsGlobalMapping globalMapping;
	LocalObjects localObjects;

	std::size_t index;

	
	
	std::string className;
	std::vector<std::string> names; 
	cm_array<std::size_t> components; 
	cm_array<std::size_t> componentClasses; 
	std::vector<ExecutionUnitBase*> executionUnits;
	std::size_t instanceSize = 0;
};

std::ostream& operator<<(std::ostream& os, const Class& cls);

stream::ostream& operator<<(stream::ostream& os, const Class& cls);
stream::istream& operator>>(stream::istream& os, Class& cls);



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