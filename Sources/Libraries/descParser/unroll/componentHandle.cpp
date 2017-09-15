// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentHandle.h"
#include "prototypeHandle.h"

namespace ObjectParser
{
using namespace Expressions;

ComponentHandle::ComponentHandle(RuntimePropeties runtimeProps_, PropertyAssignmentList links_, PropertyAssignmentList params_):
	runtimePropsLarva(runtimeProps_),
	linksLarva(links_),
	paramsLarva(params_)
{

}

Expressions::EvaluateState ComponentHandle::unrollRuntimeProps()
{
	EvaluateState evalState = Complete;
	
	

	return evalState;
}

Expressions::EvaluateState ComponentHandle::unrollLinks(const EvaluatedScope& parentScopename)
{
	EvaluateState evalState = Complete;

	if (linksLarva.size())
	{
		evalState = Expressions::Impossible;  

		linksLarva.erase(std::remove_if(linksLarva.begin(), linksLarva.end(), [this, &parentScopename, &evalState](auto& param)
		{
			const std::string& name = param->propertyName;
			const Reference* ref = param->value->cast<Reference>();
			ENFORCE_MSG(ref, "component link param may be only Reference to component");
			ENFORCE_MSG(ref->getPath().size() == 1, "component link param may be only Reference to component");

			if (ref->canResolveReverence(parentScopename))
			{
				EvaluationUnit* evalUnit = ref->evaluated(parentScopename);
				ComponentHandle* linkedComponent = evalUnit->cast<ComponentHandle>();
				ENFORCE_MSG(linkedComponent, "component link param may be only Reference to component");
				links.emplace(name, linkedComponent);
				
				evalState = Reject;	

				return true;
			}

			return false;
		}), linksLarva.end());
	}

	return evalState;
}

EvaluateState ComponentHandle::urollParams(const EvaluatedScope& parentScopename, boost::any* userData)
{
	EvaluateState evalState = Complete;

	if (paramsLarva.size())
	{
		evalState = Expressions::Impossible;  

		paramsLarva.erase(std::remove_if(paramsLarva.begin(), paramsLarva.end(), [this, &parentScopename, &userData, &evalState](auto& param)
		{
			const std::string& name = param->propertyName;
			if (param->canResolveReverence(parentScopename))
			{
				EvaluationUnit* evalUnit = param->value->evaluated(parentScopename, userData);

				
				if (EvaluatedArray* array = evalUnit->cast<EvaluatedArray>())
				{
					array->evaluateStep(ScopeNames(), userData);
				}

				add(name, evalUnit, InsertMethod::INSERT);
				evalState = Reject;	

				return true;
			}

			return false;
		}), paramsLarva.end());
	}

	
	for (auto& iter : *this)
	{
		
		

		if (PrototypeHandle* unit = iter.second->cast<PrototypeHandle>())
		{
			EvaluateState unitState = unit->evaluateStep(parentScopename, userData);
			evalState = merge(evalState, unitState);
		}
	}

	return evalState;
}

Expressions::EvaluateState ComponentHandle::evaluateStep(const EvaluatedScope& parentScopename, boost::any* userData)
{
	EvaluateState runtimePropsState = unrollRuntimeProps();

	EvaluateState linksState = unrollLinks(parentScopename);

	EvaluateState paramsState = urollParams(parentScopename, userData);

	EvaluateState result = merge(runtimePropsState, linksState);
	result = merge(result, paramsState);

	return result;
}

Expressions::EvaluationUnit* ComponentHandle::child(const Expressions::PropertyPath* path) const
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