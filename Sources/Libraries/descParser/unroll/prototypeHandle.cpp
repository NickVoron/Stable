// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "prototypeHandle.h"
#include "unroller.h"

namespace ObjectParser
{
	using namespace Expressions;


	PrototypeHandle::PrototypeHandle(const Expressions::EvaluatedScope& parentScope_, const InstanceDefinitionExpression& prototype_) :
		EvaluationUnit(parentScope_),
		parentScope(parentScope_), prototype(prototype_) 
	{
		unrolledParams = prototype.params;
	}

	
	
	Expressions::EvaluateState PrototypeHandle::evaluateStep(const Expressions::EvaluatedScope& parentScopename)
	{
		EvaluateState evalState = Complete;

		if (unrolledParams.size())
		{
			evalState = Expressions::Impossible;  

			unrolledParams.erase(std::remove_if(unrolledParams.begin(), unrolledParams.end(), [this, &parentScopename, &evalState](auto& param)
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

			}), unrolledParams.end());
		}

		return evalState;

	}


	Expressions::EvaluationUnit*  PrototypeHandle::unroll(Unroller* unroller)
	{
		ENFORCE_MSG(unroller, "Unroller == 0");
		ENFORCE_MSG(unrolledParams.size() == 0, "");

		PropertyAssignmentList params;
		for (auto& iter: *this)
		{
			params.emplace_back(new PropertyAssignment(iter.first, iter.second));
		}

		InstanceDefinitionExpression instanceDefinition(prototype.type, prototype.name, params, false);
		return unroller->unrollInstance(instanceDefinition, *this);

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