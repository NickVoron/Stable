// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "instanceHandle.h"

namespace ObjectParser
{

using namespace Expressions;

size_t InstanceHandle::globalIndexCounter()
{
	static size_t index = 0;
	++index;
	return index;
}

EvaluateState InstanceHandle::urollParams(const EvaluatedScope& parentScopename, boost::any* userData)
{
	EvaluateState evalState = Complete;

	if (params.size())
	{
		evalState = Expressions::Impossible;  

		params.erase(std::remove_if(params.begin(), params.end(), [this, &parentScopename, &userData, &evalState](auto& param)
		{ 
			const std::string& name = param->propertyName;
			if (param->canResolveReverence(parentScopename))
			{
				EvaluationUnit* evalUnit = param->value->evaluated(parentScopename, userData);
				add(name, evalUnit, InsertMethod::INSERT);
				evalState = Reject;	

				return true;
			}

			return false;
		
		}), params.end());
	}

	return evalState;
}

EvaluateState InstanceHandle::unrollUnEvaluatedProperies(boost::any* userData)
{
	EvaluateState evalState = Complete;

	if (unEvaluatedPropertyies.size())
	{
		evalState = Expressions::Impossible;  

		for (auto& iter = unEvaluatedPropertyies.cbegin(); iter != unEvaluatedPropertyies.cend();)
		{
			const std::string& name = iter->first;
			const Expression* expr = iter->second;

			References refs = expr->references();
			if (refs.canResolveReverence(*this))
			{
				EvaluationUnit* evalUnit = expr->evaluated(*this, userData);
				bool isClassMember = unEvaluatedPropertyies.isClassMember(expr);
				add(name, evalUnit, InsertMethod::INSERT, true);
				evalState = Reject;	

				unEvaluatedPropertyies.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	return evalState;
}

Expressions::EvaluateState InstanceHandle::evaluateStep(const EvaluatedScope& parentScopename, boost::any* userData)
{
	
	EvaluateState paramState = urollParams(parentScopename, userData);

	
	EvaluateState propertiesState = unrollUnEvaluatedProperies(userData);

	EvaluateState result = merge(paramState, propertiesState);

	
	for (auto& iter: *this)
	{
		EvaluationUnit* unit = iter.second;
		EvaluateState unitState = unit->evaluateStep(*this, userData);
		result = merge(result, unitState);
	}

	return result;
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
			if (componentHandle->name == name)
				return componentHandle;
		}
	}

	return nullptr;
}

const Expressions::EvaluationUnit* InstanceHandle::child(const Expressions::PropertyPath* path) const
{
	return get(path->name);
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