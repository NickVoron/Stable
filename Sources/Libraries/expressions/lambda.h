// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"
#include "constExprList.h"
#include "namescope.h"

namespace Expressions
{
	class Lambda : public Expression
	{
	public:
		Lambda();
		Lambda(const Expression* exrp);		

		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const override;

		void assignParams(const ConstExprList& params);
		void assignCapture(const ConstExprList& captureParams);

		const Expression* internalExpression() const;
		const ConstExprList& capture() const;
		const ConstExprList& parameters() const;

	protected:
		const Expression* expr;
		ConstExprList captureParams;
		ConstExprList callParams;
	};

	class LambdaUnit : public EvaluationUnit
	{
	public:
		LambdaUnit(EvaluatedScope& namescope, const Lambda& prototype);
		const Lambda& master() const;

	protected:
		const Lambda& prototype;
	};

	
	class LambdaCall : public Expression
	{
	public:
		LambdaCall(const std::string& name, const ConstExprList& params, const Reference* lambda);

		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
		virtual std::string string() const override;
		virtual std::string typeName() const override { return str::spaced("lambda call:", name); }

		const Reference* lambda;
		const std::string name;
		ConstExprList params;
	};

	
	class LambdaCallUnit : public EvaluationUnit
	{
	public:
		LambdaCallUnit(EvaluatedScope& parent, const LambdaCall& proto);

		virtual std::string string() const override   { return str::stringize(typeName(), callParams.string()); }
		virtual std::string typeName() const override { return str::spaced("lambda call unit:", prototype.name); }

		virtual EvaluationInfo evaluate();
		virtual void extract(EvaluationSet& result) override;

		const LambdaCall& prototype;
		EvaluationUnitsList callParams;					
		EvaluationUnit* resultUnit = nullptr;
		EvaluationUnit* lambdaUnit = nullptr;
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