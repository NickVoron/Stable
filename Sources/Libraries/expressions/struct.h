// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "holder.h"
#include "constExprList.h"

namespace Expressions
{
	
	class Struct : public Expression
	{
	public:
		Struct(const std::string& structType, const ConstExprList& params);

 		virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;
 		virtual std::string string() const override;
		
		const std::string structType;
		const ConstExprList params;
	};

	class EvalStruct : public EvaluationUnit
	{
	public:
		EvalStruct(EvaluatedScope& scope, const Struct& prototype);

		virtual std::string typeName() const override { return prototype.structType; }
		virtual std::string string() const override { return str::stringize(typeName(), params.string()); }
		virtual void extract(EvaluationSet& result) override;

		const Struct& prototype;
		EvaluationUnitsList params;
	};
}




// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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