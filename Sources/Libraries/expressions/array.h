#pragma once

#include "expression.h"
#include "constExprList.h"
#include "namescope.h"
#include "conversion/varToType.h"

namespace Expressions
{
	//
	class Array : public Expression
	{
	public:
		Array(){}
		Array(const ConstExprList& params);

		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override;
	
		void add(const Expression* expr);
		const Expression* element(std::size_t index) const;
		std::size_t count() const;

		ConstExprList elements;
	};

	class ArrayContainer : public EvaluationUnit
	{
	public:
		ArrayContainer(const Expression& proto, EvaluatedScope& parent);
		ArrayContainer(const Expression& proto, EvaluatedScope& parent, EvaluationUnitsList& elements);

		virtual std::string string() const override;
		virtual std::string typeName() const override;

		virtual EvaluationInfo evaluate() override;
		virtual void extract(EvaluationSet& result) override;

		EvaluationUnit*& add(EvaluationUnit* expr);

		const EvaluationUnit* element(std::size_t index) const;
		EvaluationUnit* element(std::size_t index);
		std::size_t count() const;

		EvaluationUnitsList elements;
	};


	template<class TargetType>
	void linearize(EvaluationUnitsList& elements, std::vector<TargetType>& result);

	template<class TargetType>
	void linearize(EvaluationUnit& expr, std::vector<TargetType>& result)
	{
		if (auto units = expr.cast<ArrayContainer>())
		{
			linearize(units->elements, result);
		}
		else if (auto unit = expr.cast<EvaluationUnit>())
		{
			TargetType type;
			if(convertVar(*unit, type))
			{
				result.push_back(type);
			}			
		}
		else
		{
			THROW("expression must be requested unit type or Array of them");
		}
	}

	template<class TargetType>
	void linearize(EvaluationUnitsList& elements, std::vector<TargetType>& result)
	{
		for (std::size_t i = 0; i < elements.size(); ++i)
		{
			linearize(*elements[i], result);
		}
	}

	ArrayContainer* to_array(EvaluationUnit* unit);
}

