// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "unroller.h"
#include "expressions/library.include.h"
#include "propertyAssignment.h"
#include "expressions/noValueExpression.h"


namespace ObjectParser
{

using namespace Expressions;

Unroller::Unroller(const ClassTable& classes, ComponentModelConfigurator& configurator_, bool logging):
	classes(classes), configurator(configurator_), logger(logging)
{

}


Expressions::Expression* Unroller::unrollInstance(const InstanceDefinitionExpression* prototype, Expressions::ScopeNames& parentScopenames)
{


	Expressions::Expression* result = 0;
	
	if (!prototype->noinstance)
	{
		if (prototype->arrayData)
		{
			const Expression* evaluatedArrayData = prototype->arrayData->evaluated(parentScopenames);

			Expressions::Array* arrayInstances = add<Expressions::Array>();
			Expressions::arrayUnroller(parentScopenames, evaluatedArrayData, [&parentScopenames, &prototype, this, &arrayInstances]()
			{
				Expressions::Expression * object = unrollObject(parentScopenames, *prototype);
				arrayInstances->add(object);
			});

			result = arrayInstances;
		}
		else
		{
			result = unrollObject(parentScopenames, *prototype);
		}
	}
	else
	{
		result = unrollPrototype(parentScopenames, *prototype);
	}

	return result;
}

PrototypeHandle* Unroller::unrollPrototype(const Expressions::ScopeNames& parentScopenames, const InstanceDefinitionExpression& prototype)
{
	PrototypeHandle* protoHandle = configurator.beginCreatePrototype(prototype);

	(ScopeNames&)*protoHandle = assignInputValues(prototype.params, parentScopenames);
	
	configurator.endCreatePrototype(protoHandle);

	return protoHandle;
}


InstanceHandle* Unroller::unrollObject(const Expressions::ScopeNames& parentScopenames, const InstanceDefinitionExpression& prototype)
{
	configurator.beginCreateInstance(prototype.name, prototype.type);

	const ClassDesc* instanceClass = classes.get(prototype.type);
	ENFORCE_MSG(instanceClass, str::stringize(" class: ", prototype.type, " not exist").str());
	
	
	ScopeNames scope;
	ScopeNames unEvaluatedPropertyies = fillScopenamesFromClass(scope, *instanceClass);
	ScopeNames objectScopenames = assignInputValues(prototype.params, parentScopenames);

	resolveNames(objectScopenames, unEvaluatedPropertyies);
		
	InstanceHandle* inst = configurator.endCreateInstance();
	(ScopeNames&)*inst = objectScopenames;
	return inst;
}


Expressions::ScopeNames Unroller::assignInputValues(const PropertyAssignmentList& propertyAssigments, const Expressions::ScopeNames& parentScopenames)
{
	Expressions::ScopeNames instanceScopenames;
	for (auto& assigment : propertyAssigments)
	{
		const std::string& name = assigment->propertyName;
		const Expressions::Expression* value = assigment->value->evaluated(parentScopenames);		
		instanceScopenames.add(name, value, Expressions::ScopeNames::INSERT);
	}

	return instanceScopenames;
}


Expressions::ScopeNames Unroller::fillScopenamesFromClass(Expressions::ScopeNames& result, const ClassDesc& classDesc)
{
	
	for (auto& iterator : classDesc.properties())
	{
		const std::string propName = iterator.first;
		const Expression* propValue = iterator.second;

		result.add(propName, propValue, Expressions::ScopeNames::IGNORE_IF_EXIST);
	}

	for (auto& mixInheritance: classDesc.mixInheritanceList)
	{
		const ClassDesc* inheritanceClass = classes.get(mixInheritance->type);
		ENFORCE_MSG(inheritanceClass, str::stringize(" class: ", mixInheritance->type, " not exist").str());

		
		for (auto& assigments : mixInheritance->params)
		{
			result.add(assigments->propertyName, assigments->value, Expressions::ScopeNames::IGNORE_IF_EXIST);
		}

		
		fillScopenamesFromClass(result, *inheritanceClass);
	}

	return result;
}

void Unroller::resolveNames(Expressions::ScopeNames& instanceScopenames, const Expressions::ScopeNames& unEvaluatedProperties)
{
	if(!unEvaluatedProperties.empty())
	{ 
		ScopeNames iterationResult;
		for (auto& iter : unEvaluatedProperties)
		{
			const std::string& name = iter.first;
			const Expression* expr = iter.second;

			
			if (!instanceScopenames.exists(name))
			{
				
				Expressions::References exprRefs = expr->references();
				if (canResolveDependencies(exprRefs, instanceScopenames))
				{
					boost::any uroller(this);
					const Expressions::Expression* unrolledElement = expr->evaluated(instanceScopenames, &uroller);
					ENFORCE_MSG(unrolledElement, "expression can't unroll");
					instanceScopenames.add(name, unrolledElement, ScopeNames::INSERT);
				}
				else
				{
					iterationResult.add(name, expr, ScopeNames::INSERT);
				}
			}
		}

		if (iterationResult.size() < unEvaluatedProperties.size())
		{
			resolveNames(instanceScopenames, iterationResult);
		}	
		else
		{
			str::stringize err("can't evaluate expressions: ", "\n");

			for (auto& iter : unEvaluatedProperties)
			{
				const std::string& name = iter.first;
				const Expression* expr = iter.second;
				
				err(name, " : ", expr->string(), "\n");
			}

			THROW(err.str());
		}
	}
}

ComponentHandle* Unroller::unrollComponent(const Expressions::ScopeNames& instanceScopenames, const Component& componentPrototype)
{
	ComponentHandle* componentHandle = configurator.preCreateComponent(componentPrototype.componentType, componentPrototype.name);

	for (auto& runtimeParam : componentPrototype.runtimeParams)
	{
		configurator.bindRuntimeProperty(componentHandle, runtimeParam->type, runtimeParam->value);
	}

	configurator.beginCreateComponent(componentHandle, componentPrototype.componentType, componentPrototype.name);
	
	for (auto& link : componentPrototype.linkParams)
	{
		const std::string& name = link->propertyName;

		const Expressions::Expression* linkedExpression = link->value->evaluated(instanceScopenames);
		ENFORCE_MSG(linkedExpression, "linked component not found by name:" + link->value->string());

		const ComponentHandle* linkedComponent = linkedExpression->cast<ComponentHandle>();
		ENFORCE_MSG(linkedComponent, "linked object not is component:" + linkedExpression->string());

		configurator.bindLink(componentHandle, name, linkedComponent);
		componentHandle->links.emplace(name, linkedComponent);
	}

	Expressions::ScopeNames componentScopename;
	for (auto& assigment : componentPrototype.propertyAssigments)
	{
		ENFORCE(assigment->value);

		boost::any uroller(this);								
		const Expressions::Expression* evaluatedPropertyValue = assigment->value->evaluated(instanceScopenames, &uroller);
		ENFORCE_MSG(evaluatedPropertyValue, "can't evaluate:" + assigment->value->string());
		ENFORCE(!evaluatedPropertyValue->cast<Expressions::Proxy>());								
		componentScopename.add(assigment->propertyName, evaluatedPropertyValue, Expressions::ScopeNames::INSERT);
		configurator.bindComponentProperty(componentHandle, assigment->propertyName, evaluatedPropertyValue);
	}

	configurator.endCreateComponent(componentHandle);
	componentHandle->name = componentPrototype.name;
	componentHandle->type = componentPrototype.typeName();
	(Expressions::ScopeNames&)*componentHandle = componentScopename;

	return componentHandle;
}

bool Unroller::canResolveDependencies(const Expressions::References& refs, Expressions::ScopeNames& scopeName)
{
	for (const Proxy* ref: refs)
	{
		if (!ref->canResolveReverence(scopeName))
		{
			return false;
		}
	}

	return true;
}

}//



// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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