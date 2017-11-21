#include "classDesc.h"

namespace ObjectParser
{

Component* ClassDesc::addComponent(Component* component)
{
	_properties.emplace(component->name, component);
	return component;
}

Component* ClassDesc::addComponent(Component* component, const PropertyAssignmentList& attributes)
{
	component->propertyAssigments = attributes;
	
	return addComponent(component);
}

void ClassDesc::addMixInheritance(const std::string& type, const PropertyAssignmentList& params)
{
	mixInheritanceList.emplace_back(new MixInheritance(type));
	mixInheritanceList.back()->params = params;
}
					 
void ClassDesc::addInstance(InstanceDefinitionExpression* instance)
{
	_properties.emplace(instance->name, instance);
}

void ClassDesc::addPrototype(InstanceDefinitionExpression* prototype)
{
	ENFORCE_MSG(prototype->name.size(), ""); 
	_properties.emplace(prototype->name, prototype);
}

void ClassDesc::addCallable(Expressions::Callable* callable)
{
	callables.push_back(callable);
}

void ClassDesc::resolveCallables()
{
	for (auto callable : callables)
	{
		if (Expressions::FUNCTIONS::exist(callable->name))
		{
			callable->resolve(Expressions::Callable::CALLABLE_FUNCTION_CALL, nullptr);
		}
		else if (_properties.find(callable->name) != _properties.end())
		{
			callable->resolve(Expressions::Callable::CALLABLE_LAMBDA_CALL, Expressions::add<Expressions::Reference>( new Expressions::PropertyPathElement(callable->name) ));
		}
		else
		{
			callable->resolve(Expressions::Callable::CALLABLE_STRUCT, nullptr);
		}

		ENFORCE_POINTER(callable->resolved);
	}
}

Expressions::Expression* ClassDesc::addProperty(const std::string& name, Expressions::Expression* property)
{
	_properties.add(name, property);
	return 0;
}

void ClassDesc::addConstant(const std::string& name, Expressions::Expression* expr)
{
	INCOMPLETE;
}

ClassDesc* ClassTable::get(const std::string& type) const
{
	auto iter = find(type);
	return (iter != end()) ? iter->second.get() : nullptr;
}

}//

