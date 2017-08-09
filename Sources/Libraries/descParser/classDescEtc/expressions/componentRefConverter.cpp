// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentRefConverter.h"
#include "instanceExpression.h"
#include "../component.h"

#include "componentModelConfigurator/library.include.h"


namespace ObjectParser 
{
 	void linearize(const Expressions::Expression& expr, Expressions::ConstExprList& result)
 	{
		if (auto componentExpr = expr.cast<ObjectParser::ComponentHandle>())
		{
			result.push_back(componentExpr);
		}
		else if(auto arrayExpr = expr.cast<Expressions::Array>())
		{
			for (std::size_t i = 0; i < arrayExpr->count(); ++i)
			{
				linearize(*arrayExpr->element(i), result);
			}
		}
		else
		{
			
		}
	}

	auto extractComponentsDesc(const Expressions::ConstExprList& references)
	{
		std::vector<const ComponentModel::ComponentModelConfigurator::ComponentHandle*> result;
		result.reserve(references.size());

		for (auto& refexpr : references)
		{
			auto component = refexpr->cast<ObjectParser::ComponentHandle>();
			ENFORCE(component);

			auto handle = dynamic_cast<const ComponentModel::ComponentModelConfigurator::ComponentHandle*>(component);
			ENFORCE(handle);
			ENFORCE(handle->component);

			result.push_back(handle);
		}
		return result;
	}

	void ComponentsRefConverter::convert(const Expressions::Expression& expr, LinksDescList& client)
	{

		Expressions::ConstExprList references;
		linearize(expr, references);
		auto descriptions = extractComponentsDesc(references);
		
		auto count = descriptions.size();
		if (count > 0)
		{
			client.resize(count);
			for (std::size_t i = 0; i < count; ++i)
			{
				auto componentHandle = descriptions[i];
				auto& address = client[i].address;

				address.objectIndex = componentHandle->parent->indexInEntityList;
				address.componentIndices.push_back(componentHandle->index());
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

namespace Expressions
{
    template<> void RegisterExpressionConverter<LinkDesc>() { UserStructsConvertersLib::addInst<ObjectParser::ComponentRefConverter>(); }
    template<> void RegisterExpressionConverter<LinksDescList>() { UserStructsConvertersLib::addInst<ObjectParser::ComponentsRefConverter>(); }
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