#include "property.h"
#include "reference.h"
#include "holder.h"
#include "namescope.h"

namespace Expressions
{
			 
PropertiesStruct::PropertiesStruct(const std::string& typeName) :
	structType(typeName)
{
	
}

EvaluationUnit* PropertiesStruct::evaluated(EvaluatedScope& environment) const
{
	auto result = Expressions::add<PropertiesStructUnit>(environment, *this);
	auto& scope = result->scope();
	for (auto& iter: *this)
	{
		auto element = iter.second->evaluated(scope);
		scope.add(iter.first, element, InsertMethod::INSERT);
	}

	return result;
}

//EvaluatedPropertiesStruct
PropertiesStructUnit::PropertiesStructUnit(Expressions::EvaluatedScope& parent, const PropertiesStruct& prototype_):
	EvaluationUnit("eval_prperties_struct", prototype_, parent),
	prototype(prototype_)
{

}

std::string PropertiesStructUnit::typeName() const
{
	return prototype.structType;
}

std::string PropertiesStructUnit::string() const
{ 
	return str::stringize(typeName()); 
}


void PropertiesStructUnit::add(const std::string& name, EvaluationUnit* value)
{
	scope().add(name, value, InsertMethod::INSERT);
}

const EvaluationUnit* PropertiesStructUnit::get(const std::string& name) const
{
	auto it = scope().find(name);
	return it != scope().end() ? it->second : nullptr;
}

}