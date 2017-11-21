#pragma once

#include "stuff/dependencyGraph.h"

#include "component.h"
#include "entity.h"
#include "entityList.h"
#include "class.h"
#include "property.h"
#include "pointer.h"
#include "spawner.h"
#include "factory.h"
#include "configurationQuery.h"
#include "links.h"
#include "syncronization.h"

namespace ComponentModel
{
	template<class... ModulesList>
	void init();

	template<class... ComponentsList_>
	struct Module 
	{ 
		typedef Loki::MakeTypelist<ComponentsList_...> ComponentsList; 

		template<class ContextData>
		static void init(const ContextData& context)
		{
			ComponentModel::template init<Module>(context);
		}
	};

	template<class Head, class Tail>
	struct Module<Loki::Typelist<Head, Tail>> { typedef Loki::Typelist<Head, Tail> ComponentsList; };

	namespace Private
	{
		//
		//
		//
		template<class InComponentsList, class ML> struct ModulesToComponentsImpl;

		template<class InComponentsList, class ML>
		struct ModulesToComponentsImpl
		{
			typedef typename Loki::TL::Append<InComponentsList, typename ML::Head::ComponentsList>::Result ResList;
			typedef typename ModulesToComponentsImpl<ResList, typename ML::Tail>::Result Result;
		};

		template<class InComponentsList>
		struct ModulesToComponentsImpl<InComponentsList, Loki::NullType>
		{
			typedef InComponentsList Result;
		};

		template<class ModulesList>
		struct ModulesToComponents
		{
			typedef typename ModulesToComponentsImpl<Loki::NullType, ModulesList>::Result Result;
		};

		//
		//
		//
		template<class InComponentsList, class ManagersList>
		struct ExtractComponentManagersImpl
		{
			typedef typename Loki::TL::Append<ManagersList, typename InComponentsList::Head::Parameters>::Result ResList;
			typedef typename ExtractComponentManagersImpl<typename InComponentsList::Tail, ResList>::Result Result;
		};

		template<class ManagersList>
		struct ExtractComponentManagersImpl<Loki::NullType, ManagersList>
		{
			typedef ManagersList Result;
		};

		template<class InComponentsList>
		struct ExtractComponentManagers
		{
			typedef typename Loki::TL::NoDuplicates<typename ExtractComponentManagersImpl<InComponentsList, Loki::NullType>::Result>::Result Result;
		};

		//
		//
		//
// 		template<class InComponentsList>
// 		struct ExtractComponentDependencies
// 		{
// 			template<class InComponentsList, class Dependencies>
// 			struct Impl
// 			{
// 				typedef typename Loki::TL::Append<Dependencies, typename InComponentsList::Head::Dependencies>::Result DepList;
// 				typedef typename Loki::TL::Append<DepList, typename InComponentsList::Head>::Result ResList;
// 				typedef typename Impl<typename InComponentsList::Tail, ResList>::Result Result;
// 			};
// 
// 			template<class Dependencies>
// 			struct Impl<Loki::NullType, Dependencies>
// 			{
// 				typedef Dependencies  Result;
// 			};
// 
// 			typedef typename Loki::TL::NoDuplicates<typename Impl<InComponentsList, Loki::NullType>::Result>::Result Result;
// 		};
	}		

	template<class... ModulesList>
	void init()
	{
		typedef typename Private::ModulesToComponents<Loki::MakeTypelist<ModulesList...>>::Result ComponentsList;
		ComponentsFactory::template registerComponents<ComponentsList>();
	}

	template<class... ComponentsList>
	void components()
	{
		ComponentModel::template init<Module<ComponentsList...>>();
	}
}