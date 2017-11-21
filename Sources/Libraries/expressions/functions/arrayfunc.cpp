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