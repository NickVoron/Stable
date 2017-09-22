// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceHandle.h"
#include "unroller.h"

namespace ObjectParser
{

using namespace Expressions;

size_t InstanceHandle::globalIndexCounter()
{
	static size_t index = 0;
	++index;
	return index;
}

EvaluateState InstanceHandle::urollParams(const EvaluatedScope& parentScopename)
{
	EvaluateState evalState = Complete;

	if (params.size())
	{
		evalState = Expressions::Impossible;  

		params.erase(std::remove_if(params.begin(), params.end(), [this, &parentScopename, &evalState](auto& param)
		{ 
			const std::string& name = param->propertyName;
			if (param->canResolveReference(parentScopename))
			{
				EvaluationUnit* evalUnit = param->value->evaluated(parentScopename);
				add(name, evalUnit, InsertMethod::INSERT);
				evalState = Reject;	

				return true;
			}

			return false;
		
		}), params.end());
	}

	return evalState;
}

EvaluateState InstanceHandle::unrollUnEvaluatedProperies()
{
	EvaluateState evalState = Complete;

	if (!unEvaluatedProperties.empty())
	{
		evalState = Expressions::Impossible;  

		for (auto& iter = unEvaluatedProperties.cbegin(); iter != unEvaluatedProperties.cend();)
		{
			const std::string& name = iter->first;
			const Expression* expr = iter->second;

			References refs = expr->references();
			if (refs.canResolveReference(*this))
			{
				EvaluationUnit* evalUnit = expr->evaluated(*this);
				add(name, evalUnit, InsertMethod::INSERT, true);
				evalState = Reject;	

				unEvaluatedProperties.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	return evalState;
}

Expressions::EvaluateState InstanceHandle::evaluateStep(const EvaluatedScope& parentScopename)
{
	Unroller* unroller = boost::any_cast<Unroller*>(parentScopename.userData);
	ENFORCE_POINTER(unroller);

	userData = unroller;

	if (isParent(parentScopename))
	{
		
		EvaluateState paramState = urollParams(parentScopename);

		
		EvaluateState propertiesState = unrollUnEvaluatedProperies();

		EvaluateState result = merge(paramState, propertiesState);

		
		for (auto& iter : *this)
		{
			EvaluationUnit* unit = iter.second;
			if (isClassMember(unit))
			{
				EvaluateState unitState = unit->evaluateStep(*this);
				result = merge(result, unitState);

				if (result == Impossible)
				{
					volatile int i = 0;
				}

			}
		}
		return result;
	}

	return Expressions::Complete;
}

const ComponentHandle* InstanceHandle::component(const std::string& name) const
{
	return const_cast<InstanceHandle*>(this)->component(name);
}

ComponentHandle* InstanceHandle::component(const std::string& name)
{
	for (auto& unit : *this)
	{
		if (auto componentHandle = dynamic_cast<ComponentHandle*>(unit.second))
		{
			if (isClassMember(componentHandle) && componentHandle->name == name)
				return componentHandle;
		}
	}

	return nullptr;
}

const Expressions::EvaluationUnit* InstanceHandle::child(const Expressions::PropertyPath* path) const
{
	return get(path->name);
}

std::vector<std::string> InstanceHandle::fields() const
{
	return Expressions::EvaluatedScope::fields();
}

std::string InstanceHandle::string() const
{
	return str::spaced(name).str();
}

}//



// Copyright (C) 2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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