// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentRefConverter.h"
#include "../../unroll/componentHandle.h"
#include "instanceExpression.h"
#include "../component.h"


namespace ObjectParser 
{
	std::vector<const ObjectParser::ComponentHandle*> linearize(const Expressions::Expression& expr)
 	{
		std::vector<const ObjectParser::ComponentHandle*> components;

		if (auto componentExpr = expr.cast<ObjectParser::ComponentHandle>())
		{
			components.push_back(componentExpr);
		}
		else if(auto arrayExpr = expr.cast<Expressions::EvaluatedArray>())
		{
			for (std::size_t i = 0; i < arrayExpr->count(); ++i)
			{
				auto componentsArray = linearize(*arrayExpr->element(i));
				components.insert(components.end(), componentsArray.begin(), componentsArray.end());
			}
		}
		else if (auto arrayExpr = expr.cast<Expressions::ArrayContainer>())
		{
			for (std::size_t i = 0; i < arrayExpr->count(); ++i)
			{
				auto componentsArray = linearize(*arrayExpr->element(i));
				components.insert(components.end(), componentsArray.begin(), componentsArray.end());
			}
		}
		else
		{
			THROW("expression must be ObjectParser::Component or Array of them");
		}

		return components;
	}

	void ComponentsRefConverter::convert(const Expressions::Expression& expr, LinksDescList& client)
	{

 		auto descriptions = linearize(expr);
 		auto count = descriptions.size();
 		if (count > 0)
 		{
 			client.resize(count);
 			for (std::size_t i = 0; i < count; ++i)
 			{
 				auto componentHandle = descriptions[i];
 				auto& address = client[i].address; 
  				address.objectIndex = componentHandle->objectIndex;
  				address.componentIndices.push_back(componentHandle->componentIndex);
 			}
 		}
	}

	
	
	
	void ComponentRefConverter::convert(const Expressions::Expression& expr, LinkDesc& client)
	{
		LinksDescList list;
		convertVar(expr, list);
		if (list.size() > 0)
		{
			client = list[0];
		}
	}
}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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