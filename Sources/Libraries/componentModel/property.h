#pragma once

#include "stuff/userStruct.h"

#include "resourceDriver.h"
#include "finalizeQuery.h"
#include "propertyEditor.h"
#include "expressions/library.include.h"


struct ComponentExternalLink;

struct Property
{
// 	virtual void dump(const char* name, ResourceDriver::ResourceBase& resource) const = 0;
// 	virtual void edit(const char* name, ResourceDriver::ResourceBase& resource) const = 0;
// 	virtual void edit(const char* name, ResourceDriver::ResourceBase& resource) const { PropertyEditors::edit(name, resourceValue(resource)); }
// 	virtual void dump(const char* name, ResourceDriver::ResourceBase& resource) const{ LOG_ERROR( logs::tabs(2) << name << ": " << resourceValue(resource)); };

	inline Property(std::size_t componentShift_, mirror::runtime::Type* type_): componentShift(componentShift_), type(type_){}
	virtual ~Property() {}

	void* componentData(ComponentBase& component) const	{ return ((char*) &component) + componentShift;	}

	virtual bool convert(ComponentBase& component, const Expressions::EvaluationUnit& rhs) const = 0;

	void evaluateExternalLink(ComponentExternalLink& externalLink, ComponentBase& component, const Expressions::EvaluationUnit& rhs) const;

	mirror::runtime::Type* type = nullptr;
	std::size_t componentShift = 0;
};

template<class Converter>
struct PropertyImpl : public Property
{
	inline PropertyImpl(std::size_t componentShift_, mirror::runtime::Type* type_, const Converter& converter_) : Property(componentShift_, type_), converter(converter_) {}

	virtual bool convert(ComponentBase& component, const Expressions::EvaluationUnit& rhs) const override
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
	static bool convert(ValueType&(*Function_FromResource)(const SourceResourceType&, ValueType&), SourceResourceType&(*Function_ToResource)(const ValueType&, SourceResourceType&), ComponentBase& component, const Property& property, const Expressions::EvaluationUnit& rhs)
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
	static bool convert(ComponentBase& component, const Property& property, const Expressions::EvaluationUnit& rhs)
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
 						[Function_FromResource, Function_ToResource](ComponentBase& component, const Property& property, const Expressions::EvaluationUnit& expr)
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
		return new PropertyImpl<Converter>(componentShift, nullptr, converter);
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
