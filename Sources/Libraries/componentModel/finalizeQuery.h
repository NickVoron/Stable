#pragma once

#include "containers/library.include.h"

#include "config.h"
#include "templates.h"

class Entity;
class ComponentBase;
struct ExecutionUnitBase;

template<int maxComponentsCount>
struct FinalizeQueryBase
{
	struct Entry
	{
		typedef const char* (*className)();
		
		template<class ComponentType> void init(int idx, ComponentType** res)								
		{
			clsId = &ComponentType::ClassName;
			index = idx;
			result = (ComponentBase**)res;
		}

		template<class ComponentType> void init(int idx,  const std::string& alias_, ComponentType** res)	
		{ 
			alias = alias_;
			init<ComponentType>(idx, res);
		}

		
		className clsId;
		std::string alias;
		int index;
		ComponentBase** result;

		ptrdiff_t pointerToComponentShift;
	
		template<class ContainerComponent>
		void finalize(ContainerComponent* container)
		{
			CM_KERNEL_ENFORCE(container);
			CM_KERNEL_ENFORCE(result);
			pointerToComponentShift = pointers_distance(result, container);
			CM_KERNEL_ENFORCE(pointerToComponentShift >= 0);
		}
	};

	template<class ComponentType> void add(int index, ComponentType** result)							{ CM_KERNEL_ENFORCE(!finalized); entries.addEntry().init(index, result);		addExecutionUnit<ComponentType>(executionUnitCreators); }
	template<class ComponentType> void add(const std::string& alias, ComponentType** result)			{ CM_KERNEL_ENFORCE(!finalized); entries.addEntry().init(0, alias, result);		addExecutionUnit<ComponentType>(executionUnitCreators); }
	template<class ComponentType> void add(int index, const std::string& alias, ComponentType** result)	{ CM_KERNEL_ENFORCE(!finalized); entries.addEntry().init(index, alias, result);	addExecutionUnit<ComponentType>(executionUnitCreators); }
	template<class ComponentType> void add(ComponentType** result)	{ add(0, result); } 
 	template<class ComponentType> void add(Entity** parent)	
	{  
		CM_KERNEL_ENFORCE(!finalized);
		auto& entry = parentEntries.addEntry();
		entry.init(parent);
		addExecutionUnit<ComponentType>(executionUnitCreators); 
	}

	void add(Entity** parent)	
	{  
		CM_KERNEL_ENFORCE(!finalized);
		parentEntries.addEntry().init(parent); 
	}

	template<class ContainerComponent>
	void finalize(ContainerComponent* container)
	{
		for(std::size_t i = 0; i < entries.size(); ++i)
			entries[i].finalize(container);		

		for(std::size_t i = 0; i < parentEntries.size(); ++i)
			parentEntries[i].finalize(container);	

		finalized = true;
	}

	void dump(std::ostream& os) const
	{
		for(std::size_t i = 0; i < entries.size(); ++i)
			os << entries[i];
	}

	struct ParentEntry
	{
		void init(Entity** p) { parent = p; }

		template<class ContainerComponent>
		void finalize(ContainerComponent* container)
		{
			CM_KERNEL_ENFORCE(container);
			CM_KERNEL_ENFORCE(parent);
			pointerToEntityShift = pointers_distance(parent, container);
			CM_KERNEL_ENFORCE(pointerToEntityShift >= 0);
		}

		Entity** parent = nullptr;
		ptrdiff_t pointerToEntityShift = -1; 
	};
	

	typedef ExecutionUnitBase* (*executionUnitCreator)();

	Base::FixedArray<Entry, maxComponentsCount> entries;
	Base::FixedArray<ParentEntry, maxComponentsCount> parentEntries;
	Base::FixedArray<executionUnitCreator, maxComponentsCount> executionUnitCreators;
	bool finalized = false;
};

struct FinalizeQuery : public FinalizeQueryBase<32>{};

std::ostream& operator<<(std::ostream& os, const FinalizeQuery::Entry& entry);
std::ostream& operator<<(std::ostream& os, const FinalizeQuery& entry);
