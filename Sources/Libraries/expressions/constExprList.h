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
	struct ExprList : public std::vector<Expression*>
	{
		ExprList(){}
		ExprList(const ExprList& exprList);

		template<class... P0> ExprList(const P0&... p0) : vector{ Expressions::add_const(p0)... } { }
	};

	
	struct ConstExprList : public std::vector<const Expression*>
	{
		ConstExprList(const ExprList& exprList);
		template<class... P0> ConstExprList(const P0&... p0) : vector{ Expressions::add_const(p0)... }	{ }
		
		void evaluate(const EvaluatedScope& environment, ConstExprList& result) const;
		std::string string() const;
		std::string naked_string() const;
		References references() const;

	};

	
	struct EvalUnitList : public std::vector<const EvaluationUnit*>
	{
		EvalUnitList() {}
		EvalUnitList(const EvalUnitList& exprList);

		std::string string() const;
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