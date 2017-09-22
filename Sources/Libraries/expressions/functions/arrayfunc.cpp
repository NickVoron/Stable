// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "arrayfunc.h"

#include "../array.h"
#include "../property.h"

#include "common/breakpoint.h"

namespace Expressions{
namespace Functions{

	Expression* propertiesStructMerge(const std::string& resultName, const PropertiesStruct* ps0, const PropertiesStruct* ps1)
	{
		INCOMPLETE;

		PropertiesStruct* res = add<PropertiesStruct>(resultName);

		
		

		return res;
	}

	Expression* propertiesStructArray(const std::string& structName, const std::string& fieldName, Array* arr)
	{
		ConstExprList exprs;
		exprs.resize(arr->count());
		for (std::size_t i = 0; i < arr->count(); ++i)
		{
			PropertiesStruct* ps = add<PropertiesStruct>(structName);
			ps->add(fieldName, arr->element(i));
			exprs[i] = ps;
		}

		return add<Array>(exprs)->evaluated(EvaluationUnit::commonParent);
	}

	Expression* propertiesArrayMerge(const std::string& resultName, Array* arr0, Array* arr1)
	{
		ConstExprList exprs;

		if (arr0->count() == arr1->count())
		{
			auto count = arr0->count();
			exprs.resize(count);
			for (std::size_t i = 0; i < count; ++i)
			{
				const PropertiesStruct* s0 = arr0->element(i)->cast<const PropertiesStruct>();
				const PropertiesStruct* s1 = arr1->element(i)->cast<const PropertiesStruct>();
				exprs[i] = propertiesStructMerge(resultName, s0, s1);
			}			
		}

		return add<Array>(exprs)->evaluated(EvaluationUnit::commonParent);
	}

	Expression* first_elements(ArrayContainer* array, std::size_t count)
	{
		auto result = add<ArrayContainer>(EvaluationUnit::commonParent);
		count = std::max(std::min(count, array->count()), (std::size_t)0);
		for (std::size_t i = 0; i < count; ++i)
		{
			result->add(array->element(i));
		}

		return result;
	}

	Expression* select_elements(ArrayContainer* array, ArrayContainer* indices)
	{
		auto result = add<ArrayContainer>(EvaluationUnit::commonParent);
		auto count = indices->count();
		for (std::size_t i = 0; i < count; ++i)
		{
			int index;
			convertVar(*indices->element(i), index);
			if (index >= 0 && index < (int) array->count())
			{
				result->add(array->element(index));
			}
		}

		return result;
	}

	std::vector<unsigned int> linear_indices(unsigned int count)
	{
		std::vector<unsigned int> result(count);
		for (unsigned int i = 0; i < count; ++i)
		{			
			result[i] = i;
		}
		return result;
	}

	int array_test(const std::vector<Expression*>& input)
	{
		for (auto expr : input)
		{
			LOG_MSG(expr->string());
		}
		return 0;
	}

	void array()
	{
		FUNCTIONS::add("setname", &propertiesStructArray);
		FUNCTIONS::add("merge", &propertiesArrayMerge);

		BIND_EXPRESSION_FUNCTION(first_elements);
		BIND_EXPRESSION_FUNCTION(select_elements);
		BIND_EXPRESSION_FUNCTION(linear_indices);
		
	}
}
}



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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