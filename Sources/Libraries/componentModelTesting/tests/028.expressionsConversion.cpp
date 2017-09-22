// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "028.expressionsConversion.h"

#include "expressions/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

template<class T>
Expressions::Expression* testType(const T& value)
{
	Expressions::Expression* ret = nullptr;
	T res;
	Expressions::convertVar(*(ret = Expressions::convertType(value)), res);
	ENFORCE(res == value);
	return ret;
}

template<class... Types>
void conversionTest(const Types&... value)
{
	std::vector<Expressions::Expression*> vec{ testType(value)... };
	for (auto expr : vec)
	{
		LOG_EXPRESSION_VALUE(expr->string());
	}
}
															 
ExpressionsConversion::ExpressionsConversion()
{
	try
	{
		mirror::declare<Vector3>("Vector3")("x", &Vector3::x)("y", &Vector3::y)("z", &Vector3::z);

		conversionTest(1, 0.0f, Vector3(1, 1, 1));
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e);
	}
}

}
#endif



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