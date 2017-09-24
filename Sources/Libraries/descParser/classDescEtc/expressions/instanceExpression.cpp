// Copyright (C) 2016-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceExpression.h"
#include "../../unroll/unroller.h"
#include "../../unroll/prototypeHandle.h"
#include "../../unroll/instanceHandle.h"

namespace ObjectParser
{

using namespace Expressions;

template<typename Functor>
bool arrayUnroller(Expressions::EvaluatedScope& parentScopename, const Expressions::Expression* arrayData, Functor unrollInstanceFunctor)
{
	ENFORCE(arrayData);

	size_t elementsCount = 0;

	if (auto expr = arrayData->cast< Expressions::Const<int> >())
	{
		elementsCount = expr->value;
	}
	else if (auto expr = arrayData->cast<Expressions::ArrayContainer>())
	{
		elementsCount = expr->count();
	}
	else
	{
		THROW("ArrayData type no Array and no Const<int>: " + arrayData->string())
	}

	for (int i = 0; i < elementsCount; i++)
	{
		EvaluationUnit* index = Expressions::convertType(i)->cast<Expressions::Const<int>>();
		unrollInstanceFunctor(parentScopename);
	}

	return true;
}


InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_, const PropertyAssignmentList& params_, bool noinst):
	type(type_),
	name(name_),
	params(params_),
	noinstance(noinst)
{
	
}

InstanceDefinitionExpression::InstanceDefinitionExpression(const std::string& type_, const std::string& name_):InstanceDefinitionExpression(type_, name_, PropertyAssignmentList(), false)
{
}

std::string InstanceDefinitionExpression::string() const
{
	auto value = str::stringize("InstanceDefinitionExpression type: ", type, ", name: ", name).str();

	if (arrayData)
	{
		value += str::stringize(", arrayData:", arrayData->string()).str();
	}

	return value;
}




Expressions::EvaluationUnit* InstanceDefinitionExpression::evaluated(const EvaluatedScope& parentScopenames) const
{
	Unroller* unroller = boost::any_cast<Unroller*>(parentScopenames.userData);
	ENFORCE_POINTER(unroller);
	
	EvaluationUnit* result = 0;

	if (!noinstance)
	{
		if (arrayData)
		{
			References refs = arrayData->references();
			if (refs.canResolveReference(parentScopenames))
			{
				EvaluationUnit* evaluatedArrayData = arrayData->evaluated(parentScopenames);
				evaluatedArrayData->evaluateStep(parentScopenames);

				EvaluatedArray* arrayInstances = add<EvaluatedArray>(evaluatedArrayData, parentScopenames);

				arrayUnroller(const_cast<EvaluatedScope&>(parentScopenames), evaluatedArrayData, [&parentScopenames, this, unroller, &arrayInstances](EvaluatedScope& immediatelyParams)
				{
					Expressions::EvaluationUnit* object = evaluateOnce(arrayInstances->internalScope(), unroller->classes, type, name, params);
					arrayInstances->add(object);
				});

				result = arrayInstances;
			}
		}
		else
		{
			result = evaluateOnce(parentScopenames, unroller->classes, type, name, params);
		}
	}
	else
	{
		result = add<PrototypeHandle>(parentScopenames, *this);
	}

	return result;
}

Expressions::References InstanceDefinitionExpression::references() const
{
	
	Expressions::References resultRefs;

	if (arrayData)
	{
		auto arrayDataRefs = arrayData->references();
		resultRefs.insert(resultRefs.begin(), arrayDataRefs.begin(), arrayDataRefs.end());
	}

	return resultRefs;
}

InstanceDefinitionExpression* InstanceDefinitionExpression::instance() const
{
	return Expressions::add<InstanceDefinitionExpression>(type, name, params, false);
}


EvaluationUnit* evaluateOnce(const EvaluatedScope& parentScopenames, const ClassTable& classes, const std::string& type, const std::string& name, const PropertyAssignmentList& params)
{
	InstanceHandle* instance = add<InstanceHandle>(parentScopenames, "");
	instance->params = params;
	instance->name = name;
	instance->scopeName = name;
	instance->type = type;

	const ClassDesc* instanceClass = classes.get(type);
	ENFORCE_MSG(instanceClass, str::stringize(__FUNCTION__" class: ", type, " not exist").str());
	
	
	fillScopenamesFromClass(classes, *instanceClass, params, instance->unEvaluatedProperties);
	

	return instance;
}


void fillScopenamesFromClass(const ClassTable& classes, const ClassDesc& classDesc, const PropertyAssignmentList& params, Expressions::ExpressionScope& result)
{
	
	for (auto& iterator : classDesc.properties())
	{
		const std::string propName = iterator.first;
		const Expression* propValue = iterator.second;

		if (!params.exist(propName))
		{
			result.add(propName, propValue, Expressions::InsertMethod::IGNORE_IF_EXIST, true);
		}
	}

	for (auto& mixInheritance: classDesc.mixInheritanceList)
	{
		const ClassDesc* inheritanceClass = classes.get(mixInheritance->type);
		ENFORCE_MSG(inheritanceClass, str::stringize(__FUNCTION__" class: ", mixInheritance->type, " not exist").str());

		
		for (auto& assigments : mixInheritance->params)
		{
			INCOMPLETE;
			
		}

		
		fillScopenamesFromClass(classes, *inheritanceClass, params, result);
	}
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