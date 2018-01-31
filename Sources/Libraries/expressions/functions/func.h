// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "common/preprocessor_helpers.h"

#include "variantFunctions/library.include.h"

#include "../const.h"
#include "../holder.h"
#include "../conversion/varToType.h"
#include "../conversion/convertType.h"

namespace Expressions
{
	struct EvaluationUnitCreator	
	{	
		template<class T>	
		static EvaluationUnit* create(const T& value)	
		{	
			return convertType(value);	
		}
	};

	struct EvaluationUnitConverter	
	{	
		template<class T> static T convert(const EvaluationUnit& expr)	
		{	
			T value;			
			convertVar(expr, value);
			return value;
		}	
	};

	struct EvaluationUnitTypeName	{ static std::string name(const EvaluationUnit* expr); }; 

	template<class ReturnT>
	struct ValueFunctionCaller
	{
		template<class FunctionT, class... Args> 
		static EvaluationUnit* call(FunctionT&& func, Args&&... args) 
		{ 
			return EvaluationUnitCreator::create(std::forward<FunctionT>(func)(std::forward<Args>(args)...));
		}
	};

	template<>
	struct ValueFunctionCaller<void>
	{
		template<class FunctionT, class... Args>
		static EvaluationUnit* call(FunctionT&& func, Args&&... args)
		{
			std::forward<FunctionT>(func)(std::forward<Args>(args)...);
			return Expressions::add<Expressions::VoidExpression>()->evaluated(EvaluationUnit::commonParent);
		}
	};
	
	struct CommonFunctions : public VariantFunctions::CommonFunctions<EvaluationUnit, EvaluationUnitsList, Variant::TypeTraits, EvaluationUnitCreator, EvaluationUnitTypeName, EvaluationUnitConverter, ValueFunctionCaller>{};

	struct FUNCTIONS : public CommonFunctions::Functions
	{
		static void init();
	};		  
}

#define BIND_EXPRESSION_FUNCTION(function_name) Expressions::FUNCTIONS::add(STPP_STRINGIZE(function_name), &function_name);



// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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