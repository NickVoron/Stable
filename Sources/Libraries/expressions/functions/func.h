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