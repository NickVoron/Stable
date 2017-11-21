#pragma once

#include "memoryManager.h"
#include "links.h"

class ComponentBase;
class EntityData;
class Class;
struct ResourceTable;

template<class ValueType>
void table_add(ResourceTable* table, const str::string32& name, ValueType& componentValue);

template<class ValueType, class SourceResourceType>
void table_add(ResourceTable* table, const str::string32& name, ValueType& componentValue, ValueType& (*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType&(*Function_ToResource)(const ValueType&, SourceResourceType&));

struct ResourceDriver
{
	struct ResourceBase 
	{
	friend class ComponentsFactory;
	public:
		void properties(...){};

		template<class StrType, class ValueType, class SourceResourceType>
		void property(const StrType& name, ValueType& value, ValueType& (*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType& (*Function_ToResource)(const ValueType&, SourceResourceType&))
		{
			table_add(table, name, value, Function_FromResource, Function_ToResource);
		}

		template<class ValueType>
		struct Specializer
		{			
			template<class StrType>
			static void property(ResourceTable* table, const StrType& name, ValueType& value)
			{
				table_add(table, name, value);
			}
		};

		template<class... ValueType>
		struct Specializer<ComponentLinkList<ValueType...>>
		{
			template<class StrType>
			static void property(ResourceTable* table, const StrType& name, ComponentLinkList<ValueType...>& value)
			{
				table_add(table, name, value, &ToComponentLinkList<ValueType...>, &FromComponentLinkList<ValueType...>);
			}
		};

		template<class... ValueType>
		struct Specializer<ComponentLink<ValueType...>>
		{
			template<class StrType>
			static void property(ResourceTable* table, const StrType& name, ComponentLink<ValueType...>& value)
			{
				table_add(table, name, value, &ToComponentLink<ValueType...>, &FromComponentLink<ValueType...>);
			}
		};

		template<class StrType, class ValueType>
		void property(const StrType& name, ValueType& value)
		{
			Specializer<ValueType>::property(table, name, value);
 		}

	protected:
		~ResourceBase() {}

	private:
		ResourceTable* table = nullptr;
	};
};

class ComponentResource 
{
public:
	void init(const ComponentBase& component);
	void dump();
	bool empty() const;

	const ResourceTable* table;
};

class ClassResources : public cm_array<ComponentResource>
{
public:
	void create(const Class& cls);
//	void flush(EntityData& entityData);
	void dump() const;

private:
	const Class* parent;
	cm_array<int> globalMapping;
};

void edit(ComponentResource& componentResource);
void edit(ClassResources& classResources);
