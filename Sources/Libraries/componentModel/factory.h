// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/dependencyGraph.h"

#include "component.h"
#include "property.h"
#include "serializer.h"

#include <memory>
#include <type_traits>
												



struct ExecutionListFilter : public std::vector<int>
{
	void debug();
};





struct ExecutionList : public std::vector<std::unique_ptr<ExecutionUnitBase>>
{
	void create(const ExecutionListFilter& filter);
	void debug();
	void execute();
	void destroy();
};





template<class C, bool hasExecutionUnit> struct ExecutionUnitExtractImpl;
template<class C> struct ExecutionUnitExtractImpl<C, true>	{ template<class ExecutionUnitCreator>	static ExecutionUnitCreator extract() { return &C::Parameters::executionUnit; } };
template<class C> struct ExecutionUnitExtractImpl<C, false> { template<class ExecutionUnitCreator>	static ExecutionUnitCreator extract() { return nullptr; } };

struct ExecutionUnitExtract
{
	template<class C, class ExecutionUnitCreator> 
	static ExecutionUnitCreator extract() 
	{ 
		static const bool has_member = has_static_member_executionUnit<C>::value;
		return ExecutionUnitExtractImpl<C, has_member>::template extract<ExecutionUnitCreator>();
	}
};




template<class C, bool hasExecutionUnit> struct AddToExecutionDependenciesImpl;

template<class C> struct AddToExecutionDependenciesImpl<C, true>
{
	template<class ExecutionUnitCreator>
	static void add(DependencyGraph<ExecutionUnitCreator>& dependencies, FinalizeQuery& query)
	{
		
		dependencies.node(&C::Parameters::executionUnit);
		for (std::size_t i = 0; i < query.executionUnitCreators.size(); ++i)
		{
			dependencies.dependence(&C::Parameters::executionUnit, query.executionUnitCreators[i]);
		}
	}
};

template<class C> struct AddToExecutionDependenciesImpl<C, false>
{
	template<class ExecutionUnitCreator>
	static void add(DependencyGraph<ExecutionUnitCreator>& dependencies, FinalizeQuery& query)
	{
	}
};

struct AddToExecutionDependencies
{
	template<class C, class ExecutionUnitCreator>
	static void add(DependencyGraph<ExecutionUnitCreator>& dependencies, FinalizeQuery& query)
	{
		AddToExecutionDependenciesImpl<C, has_static_member_executionUnit<C>::value >::add(dependencies, query);
	}
};

template<class ValueType0, class ValueType1> struct bidirectional_assign_impl;
template<class ValueType0, class ValueType1> struct bidirectional_assign_impl<const ValueType0&, ValueType1&> { static void assign(const ValueType0& v0, ValueType1& v1) { v1 = v0; } };
template<class ValueType0, class ValueType1> struct bidirectional_assign_impl<ValueType0&, const ValueType1&> { static void assign(ValueType0& v0, const ValueType1& v1) { v0 = v1; } };

template<class ValueType0, class ValueType1> 
void bidirectional_assign(ValueType0& v0, ValueType1& v1)
{
	if (&v0 != &v1)
	{
		bidirectional_assign_impl<ValueType0&, ValueType1&>::assign(v0, v1);
	}	
}


template<class ComponentsList, bool isAbstract>
struct ComponentsFactoryRegistrarImpl;


template<class InComponentsList>
struct ComponentsFactoryRegistrar
{
	static inline void reg() { ComponentsFactoryRegistrarImpl<InComponentsList, std::is_abstract<typename InComponentsList::Head>::value>::reg(); }
};

class ComponentBase;
class ComponentsFactory
{
friend class Class;
template<class ComponentsList, bool isAbstract> friend struct ComponentsFactoryRegistrarImpl;
public:

	typedef ExecutionUnitBase* (*ExecutionUnitCreator)();

	template<class ComponentsList>	static void registerComponents() 
	{ 
		ComponentsFactoryRegistrar<ComponentsList>::reg();
		InheritanceDetector<ComponentsList>::detect(inheritanceTable);
	}

	static std::size_t classesCount();
	static bool isRegistred(const std::string& classId);
	static bool isInherited(const std::string& parent, const std::string& candidate) { return inheritanceTable.valid(parent, candidate); }
	static FinalizeQuery& constructQuery(const std::string& classId);
	static FinalizeQuery& constructQuery(std::size_t classIndex);
	static std::pair<std::size_t, bool> classIndex(const std::string& className);
	static const char* className(std::size_t classIndex);
	static const char* className(const ComponentBase& component);
	static void createExecutionList(ExecutionList& executionList, const ExecutionListFilter& filter);
	static ExecutionUnitCreator componentClassID(std::size_t clsIdx);
	static unsigned int sizeOf(std::size_t clsIdx);
	static ResourceTable* resourceTable(std::size_t clsIdx);
	static ResourceTable* resourceTable(const ComponentBase& component);
	static void inheriteProperties(ComponentBase& parent, ComponentBase& child);

	static Serializer& constructSerializer(std::size_t classIndex);


private:
	typedef ComponentBase* (*CreationFunc)(ComponentsMemoryBlock& memoryBlock);
	typedef void (*ConstructorInvoker)(void* addr);
	typedef std::size_t (*ClassIndex)();
	typedef const char* (*ClassName)();


	ComponentsFactory(){};

	struct Data
	{
		typedef ResourceDriver::ResourceBase ResourceBase;

		struct Entry
		{
			CreationFunc creator;
			ExecutionUnitCreator executionUnitCreator;
			ConstructorInvoker constructorInvoker;
			ClassName className;
			MappingFunction inheriteParentProperties;
						
			ResourceTable resourceTable;
			unsigned int sizeOf;
			Serializer serial;

			template<class T> static T resource_cast(ResourceBase* res)			{ T r = static_cast<T>(res); CM_KERNEL_ENFORCE(r); return r; }
			template<class T> static T resource_cast(const ResourceBase* res)	{ T r = static_cast<T>(res); CM_KERNEL_ENFORCE(r); return r; }

			template<class Component> 
			void InitResourceTable(typename Component::Resource* res)
			{
				
				Component component;
				typename Component::Resource resource;
				resource.table = &resourceTable;
				resourceTable.componentGlobalIndex = Component::ClassIndex();
				resourceTable.component = &component;
				ComponentLinker<Component>::call(component);
				resource.properties(component);
				resourceTable.query.finalize(&component);				
			}

			template<class Component> void InitResourceTable(...)
			{
				
				
				
				Component component;
				resourceTable.componentGlobalIndex = Component::ClassIndex();
				resourceTable.component = &component;
				ComponentLinker<Component>::call(component);
				resourceTable.finalize();
			}

			struct YES{};
			struct NO{};

			template<class Component> static YES ResourceDetector(typename Component::Resource* resource = nullptr){}
			template<class Component> static NO ResourceDetector(...) {} 

			template<class Component> static void InheriteProperties(ComponentBase& parent, ComponentBase& child) { properties_inheritance::inherite((Component&)parent, (Component&) child); }
			
			template<class C>
			void init(DependencyGraph<ExecutionUnitCreator>& dependencies)
			{
				struct Impl
				{
					static ComponentBase* createInBuffer(ComponentsMemoryBlock& memoryBlock) { return memoryBlock.allocateMemory<C>();}
					static void invokeConstructor(void* addr) { new (addr) C(); }
					static void inheriteProperties(ComponentBase& parent, ComponentBase& child) { InheriteProperties<C>(parent, child); }
				};

				sizeOf				= sizeof(C);

				creator				= &Impl::createInBuffer;
				constructorInvoker	= &Impl::invokeConstructor;
				className			= &C::ClassName;
				inheriteParentProperties = &Impl::inheriteProperties;
				executionUnitCreator = ExecutionUnitExtract::extract<C, ExecutionUnitCreator>();

				InitResourceTable<C>(nullptr);

				AddToExecutionDependencies::add<C>(dependencies, resourceTable.query);
			}
		};

		template <class T>	struct has_member_resource	
		{ 
			static const bool value = std::is_same<Entry::YES, decltype(Entry::ResourceDetector<T>(0))>::value;
		};

		DependencyGraph<ExecutionUnitCreator> dependencies;
		Base::FixedArray<Entry, 1024> data;

		template<class C>
		void add()
		{
			if (!isRegistred(C::ClassName()))
			{
				C::ClassIndex() = data.size();
				data.addEntry().init<C>(dependencies);
			}													
		}
	};

	static void invokeConstructor(std::size_t classIndex, void* addr);
	static ComponentBase* create(std::size_t classIndex, ComponentsMemoryBlock& memoryBlock);
	static ComponentBase* create(const std::string& classId, ComponentsMemoryBlock& memoryBlock);

	static Data data;
	static InheritanceTable inheritanceTable;
};
	
inline bool isComponentRegistred(const std::string& classId) { return ComponentsFactory::isRegistred(classId); }


template<class ComponentsList>
struct ComponentsFactoryRegistrarImpl<ComponentsList, false>
{
	static inline void reg()
	{
		
		ComponentsFactory::data.add<typename ComponentsList::Head>();
		ComponentsFactoryRegistrarImpl<typename ComponentsList::Tail, std::is_abstract<typename ComponentsList::Tail::Head>::value >::reg();
	}
};


template<class ComponentsList>
struct ComponentsFactoryRegistrarImpl<ComponentsList, true>
{
	static inline void reg()
	{
		
		ComponentsFactoryRegistrarImpl<typename ComponentsList::Tail, std::is_abstract<typename ComponentsList::Tail::Head>::value >::reg();
	}
};

template<>	struct ComponentsFactoryRegistrarImpl<Loki::NullType, false> { static inline void reg() {} };


template<class... T>
void SerializerInterface::serialize_impl(T&... value)
{
	ComponentsFactory::constructSerializer(classIndex()).add(*this, &value...);
}

template<class... ComponentType>
void SerializerInterface::serialize_impl(ComponentLink<ComponentType...>& lnk)
{
	link(lnk.parent);
	ComponentsFactory::constructSerializer(classIndex()).add(*this, &lnk);
}

template<class... ComponentType>
void SerializerInterface::serialize_impl(ComponentLinkList<ComponentType...>& lnk)
{
	link(lnk.parent);
	ComponentsFactory::constructSerializer(classIndex()).add(*this, &lnk);
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