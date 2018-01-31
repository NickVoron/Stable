// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

}





// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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