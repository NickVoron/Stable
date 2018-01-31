// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
		void add(const V& value) { push_back(Expressions::add_const_expression(value)->evaluated(EvaluationUnit::commonParent)); }

		void extract(EvaluationUnit::EvaluationSet& result);

		std::string string() const;
		std::string naked_string() const;
		std::set<EvaluationUnit*> classMembers;
	};


	
	struct ConstExprList : public std::vector<const Expression*>
	{
		template<class... P0> explicit ConstExprList(const P0&... p0) : vector{ Expressions::add_const_expression(p0)... }	{ }
		
		template<class V>
		void add(const V& value) { push_back(Expressions::add_const_expression(value)); }
		EvaluationUnitsList evaluated(EvaluatedScope& namescope) const;

		std::string string() const;
		std::string naked_string() const;
	};
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