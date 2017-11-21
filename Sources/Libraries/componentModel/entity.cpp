#include "entity.h"
#include "class.h"
#include "property.h"
#include "entityList.h"
#include "component.h"

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

void EntityData::prepare(const Class* clss)
{
	ENFORCE_POINTER(clss);
	cls = const_cast<Class*>(clss);
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

void EntityData::map(EntityData& target, MappingFunction mappingFunction)
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
	//LOG_INFO(cls->getComponentName(componentIndex) << " shift: " << shift);
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

		THROW("abnormal situation with component's placement");
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