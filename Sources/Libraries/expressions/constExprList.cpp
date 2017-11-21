#include "constExprList.h"
#include "expression.h"
#include "reference.h"

namespace Expressions
{
	//
	//
	//
	std::string EvaluationUnitsList::string() const
	{
		str::stringize result("(", str::comma());
		for (auto unit : *this)
		{
			result(unit->string());
		}
		result(str::nodelim(), ")");

		return result;
	}

	std::string EvaluationUnitsList::naked_string() const
	{
		str::stringize result;
		for (auto unit : *this)
		{
			result(unit->string());
		}
		return result;
	}

	void EvaluationUnitsList::extract(EvaluationUnit::EvaluationSet& result)
	{
		for (int i = 0; i < size(); ++i)
		{
			EvaluationUnit*& refptr = operator[](i);
			EvaluationUnit** ptr = &refptr;

			if(!result.exists(ptr))
			{
				result.add(ptr);
				refptr->extract(result);
			}
		}
	}

	//
	//
	//
	EvaluationUnitsList ConstExprList::evaluated(EvaluatedScope& namescope) const
	{
		EvaluationUnitsList result;
		result.resize(size());
		for (std::size_t i = 0; i < size(); ++i)
		{
			const Expression* param = at(i);
			EvaluationUnit* paramUnit = param->evaluated(namescope);
			result[i] = paramUnit;
		}

		return result;
	}

	std::string ConstExprList::string() const
	{
		str::stringize result("(", str::comma());
		for (auto expr : *this)
		{
			result(stringize(expr));
		}
		result(str::nodelim(), ")");

		return result;
	}

	std::string ConstExprList::naked_string() const
	{
		str::stringize result;
		for (auto expr : *this)
		{
			result(stringize(expr));
		}
		return result;
	}
}
