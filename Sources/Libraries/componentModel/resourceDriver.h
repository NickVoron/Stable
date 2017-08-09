// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	void dump() const;

private:
	const Class* parent;
	cm_array<int> globalMapping;
};

void edit(ComponentResource& componentResource);
void edit(ClassResources& classResources);




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