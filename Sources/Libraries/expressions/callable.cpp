// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "callable.h"
#include "lambda.h"
#include "function.h"
#include "reference.h"
#include "functions/func.h"

namespace Expressions
{
	Callable::Callable(const std::string& name_, const ConstExprList& params_):
		  name(name_)
		, params(params_)
	{

	}

	void Callable::resolve(Type type, const Reference* lambdaRef)
	{
		switch (type)
		{
		case CALLABLE_FUNCTION_CALL:	resolved = add<Function>(name, params); break;
		case CALLABLE_STRUCT:			resolved = add<Struct>(name, params); break;
		case CALLABLE_LAMBDA_CALL:		resolved = add<LambdaCall>(name, params, lambdaRef); break;
		}

		ENFORCE_POINTER(resolved);
	}

	EvaluationUnit* Callable::evaluated(EvaluatedScope& namescope) const
	{
		ENFORCE_POINTER(resolved);
		return resolved->evaluated(namescope);
	}
}



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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