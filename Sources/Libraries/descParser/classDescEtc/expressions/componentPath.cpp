// Copyright (C) 2016-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentPath.h"
#include "instanceExpression.h"
#include "../componentModelConfigurator.h"
#include "stuff/enforce.h"

namespace ObjectParser 
{

	
	
	const Expressions::Expression* ComponentPath::evaluate(const Expressions::Expression* input, const Expressions::ScopeNames& context) const
	{
		Expressions::ConstExprList componentsList;

		if (input)
		{
			if (const InstanceHandle* instace = input->cast<InstanceHandle>() )
			{
				
				componentsList.push_back(getComponent(instace, componentType));
			}
			else
			{
				
				const Expressions::Array* array = input->cast<const Expressions::Array>();

				for (std::size_t i = 0; i < array->count(); ++i)
				{
					const InstanceHandle* instanceHandle = array->element(i)->cast<const InstanceHandle>();
					ENFORCE_MSG(instanceHandle, "");
					componentsList.push_back(getComponent(instanceHandle, componentType));
				}
			}
		}
		else
		{
			LOG_ERROR("context: input is nullptr");
		}

		return Expressions::add<Expressions::Array>(componentsList);
	}

	const Expressions::Expression* ComponentPath::getComponent(const InstanceHandle* instanceHandle, std::string componentType)
	{
		const Expressions::Expression* component = instanceHandle->getByType(componentType);
		ENFORCE_MSG(component, str::stringize ("instance: ", instanceHandle->name, " hasn't component with name: " , componentType).c_str());

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