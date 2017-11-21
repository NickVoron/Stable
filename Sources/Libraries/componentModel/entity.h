#pragma once

#include "containers/library.include.h"

#include "config.h"
#include "storage.h"

class Class;
class ComponentBase;
class EntitiesList;
typedef void(*MappingFunction)(ComponentBase& source, ComponentBase& target);
//
//
//
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

	void map(EntityData& target, MappingFunction mappingFunction);

	Class* cls = nullptr;

protected:
	void serialize(serializer& ser) const;
	void prepare(const Class* cls);
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

private:
	static const std::size_t entitiesInBucket = 64;
	typedef Bucket<entitiesInBucket> BucketType;
	typedef std::vector<std::unique_ptr<BucketType>> BucketList;

	void incrementBucketsCount();
	BucketList buckets;
};