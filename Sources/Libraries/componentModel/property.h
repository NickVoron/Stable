// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/userStruct.h"

#include "resourceDriver.h"
#include "finalizeQuery.h"
#include "propertyEditor.h"
#include "expressions/library.include.h"


struct Property
{





	inline Property(std::size_t componentShift_): componentShift(componentShift_){}
	virtual ~Property() {}

	void* componentData(ComponentBase& component) const	{ return ((char*) &component) + componentShift;	}

	virtual bool convert(ComponentBase& component, const Expressions::Expression& rhs) const = 0;

	std::size_t componentShift = 0;
};

template<class Converter>
struct PropertyImpl : public Property
{
	inline PropertyImpl(std::size_t componentShift_, const Converter& converter_) : Property(componentShift_), converter(converter_) {}

	virtual bool convert(ComponentBase& component, const Expressions::Expression& rhs) const
	{
		return converter(component, *this, rhs);
	}

	Converter converter;
};

template<class ComponentPtrType, class ValueType>
void serialize_component(ComponentPtrType* component, ValueType& value)
{
	component->serialize(value);
}

template<class ValueType0, class ValueType1>
void bidirectional_assign(ValueType0& v0, ValueType1& v1);

struct ResourceTable : public std::unordered_map<str::string32, std::unique_ptr<Property>>
{
public:
	template<class ValueType, class SourceResourceType>
	static bool convert(ValueType&(*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType&(*Function_ToResource)(const ValueType&, SourceResourceType&), ComponentBase& component, const Property& property, const Expressions::Expression& rhs)
	{
		SourceResourceType value;
		bool result = convertVar(rhs, value);
		if (result)
		{
			ValueType& target = *(ValueType*) property.componentData(component);
			bidirectional_assign(target, (const ValueType&) Function_FromResource(value, target));
		}
		
		return result;
	}

	template<class ValueType>
	static bool convert(ComponentBase& component, const Property& property, const Expressions::Expression& rhs)
	{
		ValueType value;
		bool result = convertVar(rhs, value);
		if (result)
		{
			ValueType& target = *(ValueType*) property.componentData(component);
			bidirectional_assign(target, (const ValueType&)value);
		}

		return result;
	}
	

	template<class ValueType>
	void add(const str::string32& name, ValueType& componentValue)
	{
		if (find(name) == end())
		{
			serialize_component(component, componentValue);
			emplace(name, std::unique_ptr<Property>(componentProperty(pointers_distance(&componentValue, component), &convert<ValueType>)));
		}
	}

	template<class ValueType, class SourceResourceType>
	void add(const str::string32& name, ValueType& componentValue, ValueType& (*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType&(*Function_ToResource)(const ValueType&, SourceResourceType&))
	{
		if (find(name) == end())
		{
			serialize_component(component, componentValue);
 			emplace(name, 
					std::unique_ptr<Property>(componentProperty(pointers_distance(&componentValue, component),
 						[Function_FromResource, Function_ToResource](ComponentBase& component, const Property& property, const Expressions::Expression& expr)
 						{
 							return convert<ValueType, SourceResourceType>(Function_FromResource, Function_ToResource, component, property, expr);
 						}))
			);
		}
	}

	const Property* findProperty(const str::string32& name) const;

	void dump(ResourceDriver::ResourceBase& res) const;
	void edit(ResourceDriver::ResourceBase& res) const;

	FinalizeQuery query;
	std::size_t componentGlobalIndex;
	ComponentBase* component;

	void finalize()
	{
		query.finalize(component);
	}

private:
	template<class Converter>
	static Property* componentProperty(std::size_t componentShift, const Converter& converter)
	{
		return new PropertyImpl<Converter>(componentShift, converter);
	}
};


template<class ValueType>
void table_add(ResourceTable* table, const str::string32& name, ValueType& componentValue)
{
	CM_KERNEL_ENFORCE(table);
	table->add(name, componentValue);
}

template<class ValueType, class SourceResourceType>
void table_add(ResourceTable* table, const str::string32& name, ValueType& componentValue, ValueType& (*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType&(*Function_ToResource)(const ValueType&, SourceResourceType&))
{
	CM_KERNEL_ENFORCE(table);
	table->add(name, componentValue, Function_FromResource, Function_ToResource);
}




// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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