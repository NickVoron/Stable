// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "expression.h"
#include "constExprList.h"
#include "scopeNames.h"

namespace Expressions
{
	class Array : public Expression
	{
	public:
		Array(){}
		Array(const ConstExprList& params);

		virtual EvaluationUnit* evaluated(const EvaluatedScope& environment, boost::any* userData = 0) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override;
		virtual References references() const override;

	
		void add(const Expression* expr);
		const Expression* element(std::size_t index) const;
		std::size_t count() const;

		ConstExprList elements;
	};

	
	class EvaluatedArray : public EvaluationUnit
	{
	public:
		EvaluatedArray(size_t size, const EvaluatedScope* environment_);
		EvaluatedArray(EvaluationUnit* arrayData_, const EvaluatedScope* environment_);

		virtual std::string string() const override;
		virtual std::string typeName() const override;

		virtual EvaluateState evaluateStep(const Expressions::EvaluatedScope&, boost::any* userData = 0) override;

		virtual const EvaluationUnit* child(const PropertyPath* path) const override;
		virtual const EvaluationUnit* child(const ArrayPath* path) const override;


		void add(const EvaluationUnit* expr);
		const EvaluationUnit* element(std::size_t index) const;
		std::size_t count() const;

		ConstExprList unEvaluatedElements;
	private:
		EvaluatedScope internalScopename;

		EvalUtinList elements;

		EvaluationUnit* arrayData = 0;
		const Expressions::EvaluatedScope* environment;
	};

}





// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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