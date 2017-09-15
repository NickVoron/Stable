// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentPath.h"
#include "instanceExpression.h"
#include "../../unroll/componentModelConfigurator.h"
#include "stuff/enforce.h"

namespace ObjectParser 
{
	using namespace Expressions;

	
	
	Expressions::EvaluationUnit* ComponentPath:: evaluate(const Expressions::EvaluationUnit* input, const Expressions::EvaluatedScope& context) const
	{
		Expressions::EvaluationUnit* result = 0;

		if (input)
		{
			if (const InstanceHandle* instace = input->cast<InstanceHandle>() )
			{
				
				 result = const_cast<Expressions::EvaluationUnit*>(getComponent(instace, componentType));
			}
			else
			{
				
				const Expressions::EvaluatedArray* array = input->cast<const Expressions::EvaluatedArray>();

				ENFORCE_MSG(array, "");
				Expressions::EvaluatedArray* resultArr = Expressions::add<Expressions::EvaluatedArray>(array->count(), nullptr);
				result = resultArr;

				for (std::size_t i = 0; i < array->count(); ++i)
				{
					const InstanceHandle* instanceHandle = array->element(i)->cast<const InstanceHandle>();
					ENFORCE_MSG(instanceHandle, "");

					const Expressions::EvaluationUnit* foundComponent = getComponent(instanceHandle, componentType);
					if (!foundComponent)
					{
						
						result = 0;
						break;
					}
					resultArr->add(foundComponent);
				}
			}
		}
		else
		{
			LOG_ERROR("context: input is nullptr");
		}

		return result;
	}

	std::unique_ptr<Expressions::Reference::PathElement> ComponentPath::copy() const
	{
		return std::unique_ptr<Expressions::Reference::PathElement>(new ComponentPath(componentType));
	}

	const Expressions::EvaluationUnit* ComponentPath::getComponent(const InstanceHandle* instanceHandle, std::string componentType)
	{
		const Expressions::EvaluationUnit* component = instanceHandle->getByType(componentType);
		return component;
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