// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "entity.h"
#include "class.h"
#include "property.h"
#include "entityList.h"
#include "component.h"

struct DebugStream : public cm_array< stream::dmemstream > 
{

};


std::size_t EntityData::getComponentsCount() const
{
	return cls->getComponentsCount();
}

void EntityData::save(stream::ostream& os) const
{
	CM_KERNEL_ENFORCE(cls);
	os << cls->getComponentsCount();
	auto srlzr = serializer(os);
	serialize(srlzr);
}

void EntityData::load(stream::istream& is)
{
	std::size_t count;
	is >> count;
	ENFORCE_EQUAL(count, getComponentsCount());
	auto srlzr = serializer(is);
	serialize(srlzr);
}

void EntityData::serialize(serializer& ser) const
{
	auto count = cls->getComponentsCount();
	for (std::size_t i = 0; i < count; ++i)
	{
		auto& component = getComponent(i);
		auto& s = ComponentsFactory::constructSerializer(component.classIndex());
		s.serialize(component, ser);
	}
}

EntityData::~EntityData()
{
	destroyComponents();	
}

void EntityData::prepare(const Class& clss)
{
	cls = &clss;
	componentsMemory.allocateMemory(cls->instanceSize);
}

void EntityData::destroyComponents()
{
	std::size_t count = cls->getComponentsCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		getComponent(i).~ComponentBase();
	}
}

void EntityData::finalize()
{
	std::size_t count = getComponentsCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		getComponent(i).finalize();
	}	
}

void EntityData::map(EntityData& target, MappingFunction mappingFunction) const
{
	const ClassMapping::Entry& mapping = cls->map(*target.cls);
	mapping.map(*this, target, mappingFunction);
}

ComponentBase& EntityData::getComponentByShift(std::size_t componentShift) const
{
	CM_KERNEL_ENFORCE(componentShift < componentsMemory.memBlock.len);
	return *(ComponentBase*)&((char*)componentsMemory.memBlock.data)[componentShift];
}

ComponentBase& EntityData::getComponent(std::size_t componentIndex) const
{
	CM_KERNEL_ENFORCE(cls);
	auto shift =  cls->getComponentShift(componentIndex);
	
	return getComponentByShift(shift);
}

bool EntityData::componentIndex(ComponentBase* component, std::size_t& componentIndex) const
{
	if(componentsMemory.memBlock.includeAddress(component))
	{
		auto count = cls->getComponentsCount();
		for(std::size_t i = 0; i < count; ++i)
		{
			if(&getComponent(i) == component)
			{
				componentIndex = i;
				return true;
			}
		}

		ENFORCE(0 && "abnormal situation with component's placement");
	}

	return false;
}

void Entity::activate(bool active)
{
	auto count = cls->getComponentsCount();
	for(std::size_t i = 0; i < count; ++i)
	{
		getComponent(i).activate(active);
	}
}

void Entity::destroy()
{
	activate(false);
	remove();
	destroyComponents();
}

EntitiesLoadStream* PrototypeHolder::find(void* id) const
{
	auto it = data.find(id);
	return it != data.end() ? it->second.get() : 0;
}

void PrototypeHolder::clear()
{
	data.clear();
}

EntitiesLoadStream& PrototypeHolder::create(void* id, const std::string& className)
{
	auto& os = data[id];
	CM_KERNEL_ENFORCE(os == 0);
	os = std::make_unique<EntitiesLoadStream>();
	os->prototypeClassName = className;
	return *os;
}

void PrototypeHolder::save(stream::ostream& os) const
{
	os << data.size();

	for(auto& it : data)
	{
		os << it.first << it.second->prototypeClassName << it.second->ios;
	}
}

void PrototypeHolder::load(stream::istream& is)
{
	data.clear();

	std::size_t size = 0; 
	is >> size;

	for (std::size_t i = 0; i < size; ++i)
	{
		void* id;
		std::string className;
		is >> id >> className;
		auto& els = create(id, className);
		is >> els.ios;
	}
}

std::string PrototypeHolder::debugstr() const
{
	str::stringize result("count: ", data.size(), "\n");
	
	for (auto& it : data)
	{
		result(str::delimeter("\n\t"), it.first, " : ", it.second->prototypeClassName);
	}

	return result.str();
}

void EntityPool::clear()
{
	buckets.clear();
}

Entity* EntityPool::getEntity(std::size_t index) const
{
	std::size_t bucketIndex = index / entitiesInBucket;
	CM_KERNEL_ENFORCE(bucketIndex < buckets.size());
	std::size_t indexInBucket = index - bucketIndex * entitiesInBucket;
	return (*buckets[bucketIndex])[indexInBucket];
}

std::size_t EntityPool::entitiesCount() const
{
	std::size_t res = 0;
	for (auto& bucket : buckets)
	{
		res += bucket->count;
	}

	return res;
}

void EntityPool::incrementBucketsCount()
{
	buckets.emplace_back( new BucketType() );
}

Entity* EntityPool::newEntity()
{
	if( buckets.empty() || buckets.back()->isFull() )
	{
		incrementBucketsCount();
	}
	
	auto& bucket = buckets.back();
	CM_KERNEL_ENFORCE(bucket);
	Entity* entity = bucket->newEntity();
	CM_KERNEL_ENFORCE(entity);

	return new (entity) Entity();
}

void EntityPool::debugOutput() const
{
	
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