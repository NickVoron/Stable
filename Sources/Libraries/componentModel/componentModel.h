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