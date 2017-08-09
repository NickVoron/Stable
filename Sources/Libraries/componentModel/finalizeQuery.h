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