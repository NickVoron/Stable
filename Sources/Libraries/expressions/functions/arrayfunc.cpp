// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "arrayfunc.h"

#include <numeric>

#include "../array.h"
#include "../property.h"

#include "common/breakpoint.h"

namespace Expressions{
namespace Functions{

	EvaluationUnit* propertiesStructMerge(const std::string& structName, const PropertiesStructUnit* ps0, const PropertiesStructUnit* ps1)
	{		
		auto proto = add<PropertiesStruct>(structName);
		auto res = proto->evaluated(EvaluationUnit::commonParent)->cast<PropertiesStructUnit>();

		for (auto& it : ps0->scope()) {	res->add(it.first, it.second); }		
		for (auto& it : ps1->scope()) {	res->add(it.first, it.second); }

		return res;
	}

	EvaluationUnit* propertiesStructArray(const std::string& structName, const std::string& fieldName, ArrayContainer* arr)
	{
		auto proto = add<PropertiesStruct>(structName);

		EvaluationUnitsList exprs;
		exprs.resize(arr->count());
		for (std::size_t i = 0; i < arr->count(); ++i)
		{
			auto ps = proto->evaluated(EvaluationUnit::commonParent)->cast<PropertiesStructUnit>();
			ps->add(fieldName, arr->element(i));
			exprs[i] = ps;
		}

		return add<ArrayContainer>(*proto, EvaluationUnit::commonParent, exprs);
	}

	EvaluationUnit* propertiesArrayMerge(const std::string& resultName, ArrayContainer* arr0, ArrayContainer* arr1)
	{
		EvaluationUnitsList exprs;

		if (arr0->count() == arr1->count())
		{
			auto count = arr0->count();
			exprs.resize(count);
			for (std::size_t i = 0; i < count; ++i)
			{
				auto e0 = arr0->element(i);
				auto e1 = arr1->element(i);

				auto s0 = e0->cast<const PropertiesStructUnit>();
				auto s1 = e1->cast<const PropertiesStructUnit>();
				exprs[i] = propertiesStructMerge(resultName, s0, s1);
			}			
		}

		return add<ArrayContainer>(arr0->protoexpr, EvaluationUnit::commonParent, exprs);
	}

	EvaluationUnit* first_elements(ArrayContainer* array, std::size_t count)
	{
		auto result = add<ArrayContainer>(array->protoexpr, EvaluationUnit::commonParent);
		count = std::max(std::min(count, array->count()), (std::size_t)0);
		for (std::size_t i = 0; i < count; ++i)
		{
			result->add(array->element(i));
		}

		return result;
	}

	std::vector<unsigned int> get_indices(ArrayContainer* array, ArrayContainer* query)
	{
		std::vector<unsigned int> result;
		
		for (std::size_t i = 0; i < array->count(); ++i)
		{
			if (auto item0 = array->element(i))
			{
				for (std::size_t j = 0; j < query->count(); ++j)
				{
					if (auto item1 = query->element(i))
					{
						if (item0->equal(item1))
						{
							result.push_back(i);
						}						
					}
				}
			}			
		}

		return result;
	}

	EvaluationUnit* select_elements(ArrayContainer* array, const std::vector<unsigned int>& indices)
	{
		auto result = add<ArrayContainer>(array->protoexpr, EvaluationUnit::commonParent);
		for (auto index : indices)
		{
			if (index >= 0 && index < array->count())
			{
				result->add(array->element(index));
			}
		}

		return result;
	}

	std::vector<unsigned int> iota(unsigned int count, int start)
	{
		std::vector<unsigned int> result(count);
		std::iota(result.begin(), result.end(), start);
		return result;
	}

	std::vector<unsigned int> linear_indices(unsigned int count)
	{
		return iota(count, 0);
	}

	unsigned int count(ArrayContainer* array)
	{
		ENFORCE_POINTER(array);
		return array->count();
	}


	int array_test(const std::vector<EvaluationUnit*>& input)
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

		BIND_EXPRESSION_FUNCTION(count);
		BIND_EXPRESSION_FUNCTION(first_elements);
		BIND_EXPRESSION_FUNCTION(select_elements);
		BIND_EXPRESSION_FUNCTION(linear_indices);
		BIND_EXPRESSION_FUNCTION(iota);
		BIND_EXPRESSION_FUNCTION(get_indices);
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