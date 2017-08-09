// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "containers/library.include.h"

#include "config.h"
#include "storage.h"

class Class;
class ComponentBase;
class EntitiesList;
typedef void(*MappingFunction)(ComponentBase& source, ComponentBase& target);



class EntityData
{
friend class Class;
public:
	~EntityData();
	EntityData() {}
	EntityData(const EntityData& other) = delete;
	EntityData& operator=(const EntityData&) = delete;
	std::size_t getComponentsCount() const;
	ComponentBase& getComponent(std::size_t componentIndex) const;
	ComponentBase& getComponentByShift(std::size_t componentShift) const;
	bool componentIndex(ComponentBase* component, std::size_t& componentIndex) const;

	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	void finalize();
	void destroyComponents();

	void map(EntityData& target, MappingFunction mappingFunction) const;

	const Class* cls = nullptr;

protected:
	void serialize(serializer& ser) const;
	void prepare(const Class& cls);
	ComponentsMemoryBlock componentsMemory;
};

class Entity : public EntityData, public Base::IntrusiveList<Entity>::Node
{
friend class Class;
friend class EntitiesList;
friend class EntityPool;
public:
	virtual ~Entity(){ activate(false); }
	void activate(bool active);
	
	void destroy();

	const Class& getClass() const { CM_KERNEL_ENFORCE(cls); return *cls; }
	EntitiesList& getParent() const { CM_KERNEL_ENFORCE(parent); return *parent; }

protected:

	EntitiesList* parent;
};

struct EntitiesStream
{
	void* streamId = nullptr;
};

struct EntitiesLoadStream
{
	std::string prototypeClassName;
	stream::dmemstream ios;
};

struct PrototypeHolder
{
public:
	void clear();
	EntitiesLoadStream* find(void* id) const;
	EntitiesLoadStream& create(void* id, const std::string& className);
	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	std::string debugstr() const;

private:
	std::map<void*, std::unique_ptr<EntitiesLoadStream>> data;
};

inline std::ostream& operator<<(std::ostream& os, const EntitiesStream& es){return os;}

class EntityPool
{
public:
	template<std::size_t entitiesCount>
	struct Bucket 
	{
		static const std::size_t maxCount = entitiesCount;
		char data[entitiesCount * sizeof(Entity)];
		std::size_t count = 0;

		~Bucket(){clear();}
		Entity* newEntity()	{	return !isFull() ?  &((Entity*)data)[count++] : nullptr;	}

		bool isFull() const { return count == maxCount; }

		Entity* operator[] (std::size_t index)const
		{
			CM_KERNEL_ENFORCE(index < count);
			return &((Entity*)data)[index];
		}

		void clear()
		{
			for (std::size_t i = 0; i < count; ++i)
			{
				auto entity = operator[](i);
				ENFORCE(entity);
				entity->~Entity();
			}
			count = 0;
		}
	};

	void clear();
	Entity* getEntity(std::size_t index) const;
	Entity* newEntity();
	std::size_t entitiesCount() const;

	void debugOutput() const;

private:
	static const std::size_t entitiesInBucket = 64;
	typedef Bucket<entitiesInBucket> BucketType;
	typedef std::vector<std::unique_ptr<BucketType>> BucketList;

	void incrementBucketsCount();
	BucketList buckets;
};



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