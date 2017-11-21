#pragma once

#include <vector>
#include <string>
#include "expression.h"
#include "const.h"

namespace Expressions
{
	struct EvaluationUnitsList : public std::vector<EvaluationUnit*>
	{
		template<class... P0> explicit EvaluationUnitsList(const P0&... p0) : vector{ Expressions::add_const_expression(p0)->evaluated(EvaluationUnit::commonParent)... } { }

		template<class V>
		void add(const V& value) { push_back(Expressions::add_const(value)); }

		void extract(EvaluationUnit::EvaluationSet& result);

		std::string string() const;
		std::string naked_string() const;
		std::set<EvaluationUnit*> classMembers;
	};


	//
	struct ConstExprList : public std::vector<const Expression*>
	{
		template<class... P0> explicit ConstExprList(const P0&... p0) : vector{ Expressions::add_const_expression(p0)... }	{ }
		
		template<class V>
		void add(const V& value) { push_back(Expressions::add_const(value)); }
		EvaluationUnitsList evaluated(EvaluatedScope& namescope) const;

		std::string string() const;
		std::string naked_string() const;
	};
}