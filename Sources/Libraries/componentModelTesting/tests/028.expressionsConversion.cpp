#include "028.expressionsConversion.h"

#include "expressions/library.include.h"

#ifdef ENABLE_TEST
namespace ComponentModelTesting
{

template<class T>
Expressions::EvaluationUnit* testType(const T& value)
{
	Expressions::EvaluationUnit* ret = nullptr;
	T res;
	Expressions::convertVar(*(ret = Expressions::convertType(value)), res);
	//ENFORCE(res == value);
	return ret;
}

template<class... Types>
void conversionTest(const Types&... value)
{
	std::vector<Expressions::EvaluationUnit*> vec{ testType(value)... };
	for (auto expr : vec)
	{
//		LOG_EXPRESSION(expr->string());
	}
}
															 
ExpressionsConversion::ExpressionsConversion()
{
	try
	{
		mirror::declare<Vector3>("Vector3")("x", &Vector3::x)("y", &Vector3::y)("z", &Vector3::z);

		conversionTest(LinksDescList());
	}
	catch (const std::exception& e)
	{
		LOG_ERROR(e);
	}
}

}//
#endif