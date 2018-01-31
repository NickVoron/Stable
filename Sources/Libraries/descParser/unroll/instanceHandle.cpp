// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceHandle.h"
#include "unroller.h"
#include "evaluation.h"

namespace Expressions
{
	void print_scope(EvaluatedScope& namescope, int tabs);
	void print_scope_from_root(EvaluatedScope& namescope);
}


namespace ObjectParser
{

using namespace Expressions;

Unroller* InstanceHandle::unroller = nullptr;


InstanceHandle::InstanceHandle(const ClassDesc& classDesc_, const InstanceDefinitionExpression& proto, Expressions::EvaluatedScope& parent) :
	EvaluationUnit(proto.name, proto, localScope),
	definition(proto),
	classDesc(classDesc_),
	classProperties("classProperties", nullptr),
	localScope("local_scope", &parent)
{
	for (auto& iterator : classDesc.properties())
	{
		const std::string propName = iterator.first;
		const Expression* propValue = iterator.second;

		if (!definition.params.exist(propName))
		{
			classProperties.add(propName, propValue, InsertMethod::IGNORE_IF_EXIST);
			classProperties.classMembers.emplace(propValue);
		}
	}

	prepareEvaluationUnits(scope(), classProperties, scope(), true);
	prepareEvaluationUnits(localScope, definition.params, scope(), false);

	if (definition.arrayData)
	{
		evaluatedArrayData = definition.arrayData->evaluated(parent);
	}	
}

std::string InstanceHandle::typeName() const 
{ 
	return classDesc.typeName; 
}

const ComponentHandle* InstanceHandle::component(const std::string& name) const
{
	return const_cast<InstanceHandle*>(this)->component(name);
}

ComponentHandle* InstanceHandle::component(const std::string& name)
{
	for (auto& unit : scope())
	{
		if (const auto componentHandle = unit.second->cast<ComponentHandle>())
		{
			if (scope().isClassMember(componentHandle) && componentHandle->name == name)
				return componentHandle;
		}
	}

	return nullptr;
}

void InstanceHandle::extract(EvaluationSet& result)
{
	EvaluationUnit::extract(result);
	extractDependencies(localScope, result);
	if (evaluatedArrayData)
	{
		result.add(&evaluatedArrayData);
		evaluatedArrayData->extract(result);
	}	
}

EvaluationInfo InstanceHandle::evaluate()
{
	Expressions::EvaluationInfo result(Impossible);

	if (evaluatedArrayData)
	{
		ENFORCE_POINTER(definition.arrayData);

		if(evaluatedArrayData->evaluate() == Complete)
		{			
			auto container = Expressions::add<Array>()->evaluated(localScope)->cast<ArrayContainer>();

			auto instance = definition.instance();
			auto stepper = [container, instance](EvaluationUnit*& unit, EvaluationUnit* iterator, EvaluationUnit* index)
			{
				auto newObject = instance->evaluated(container->scope())->cast<InstanceHandle>();
				ENFORCE_POINTER(newObject);
				newObject->localScope.add("iterator", iterator, InsertMethod::INSERT);
				newObject->localScope.add("index", index, InsertMethod::INSERT);
				newObject->localScope.add("array_data", container, InsertMethod::INSERT);
				unit = newObject;
			};

			if (unsigned int count = 0; convertVar(*evaluatedArrayData, count))
			{
				container->elements.resize(count);
				for (unsigned int i = 0; i < count; ++i)
				{
					auto index = convertType(i);
					stepper(container->elements[i], index, index);
				}
			}
			else
			{
				auto array = to_array(evaluatedArrayData);
				container->elements.resize(array->count());
				for (auto i = 0; i < array->count(); ++i)
				{
					auto index = convertType(i);
					auto iterator = array->element(i);
					stepper(container->elements[i], iterator, index);
				}
			}

			result.reject(container);
		}
	}
	else if(!evaluatedArrayData)
	{
		result.complete(this);
	}

	return result;
}

std::string InstanceHandle::string() const
{
	return str::spaced(definition.type, ":", definition.name);
}

}//



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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