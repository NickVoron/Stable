#pragma once
#include "componentModelConfigurator/library.include.h"
#include "newmath/library.include.h"
#include "math/library.include.h"
#include "resourceUtils/library.include.h"
#include "expressions/library.include.h"
#include "descParser/library.include.h"
#include "../config.h"

#ifdef ENABLE_UTILS
namespace ComponentModelTesting
{
	template<class Function, class T>
	auto transform_vec(const std::vector<T>& vec, Function&& func)
	{
		std::vector< typename std::result_of<Function(std::size_t, T)>::type> result(vec.size());
		for (std::size_t i = 0; i < vec.size(); ++i)
		{
			result[i] = func(i, vec[i]);
		}

		return result;
	}

	const ObjectParser::InstanceDefinitionExpression* findInstance(const ObjectParser::ClassDesc* cl, const std::string& instanceName);
	const ObjectParser::Component* findComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName);
	const ObjectParser::Component* testComponent(const ObjectParser::ClassDesc* cl, const std::string& componentName);
	bool testType(const ObjectParser::Component* cm, const std::string& type);

	const ObjectParser::ComponentHandle* findComponent(const ObjectParser::InstanceHandle* inst, int index, const std::string& componentName);
	const ObjectParser::ComponentHandle* testComponent(const ObjectParser::InstanceHandle* inst, int index, const std::string& componentName, const std::string& componentType);
	
	bool testType(const ObjectParser::ComponentHandle* cm, const std::string& type);

	const ObjectParser::InstanceHandle* testInstance(std::unique_ptr<Expressions::EvaluatedScope>& scopeName, const std::string& instanceName);
	bool testClassesCount(const ObjectParser::GrammarComposition& result, std::size_t count);

	template<typename Value>
	bool test(const Expressions::EvaluationUnit& expr, const Value& value)
	{
		Value exprVal;
		if(Expressions::convertVar(expr, exprVal))
		{
			ENFORCE_EQUAL(exprVal, value);
			return true;
		}

		LOG_ERROR("can't convert: " << expr.string() << " to " << typeid(Value).name());

		return false;
	}

	Expressions::Reference* parse(const std::string& path);

	template<typename Value>
	bool test(Expressions::EvaluatedScope& scopename, const std::string& path, const Value& value)
	{
		auto proxy = parse(path);
		auto eval = Expressions::unrollExpression(*proxy, scopename, false);
		ENFORCE_POINTER(eval);
		return test(*eval, value);
	}

	template<typename Value>
	bool test(std::unique_ptr<Expressions::EvaluatedScope>& scopename, const std::string& path, const Value& value)
	{
		ENFORCE(scopename);
		return test(*scopename.get(), path, value);
	}


	const Expressions::EvaluationUnit* get(Expressions::EvaluatedScope& scopename, const std::string& path);
	const Expressions::EvaluationUnit* get(std::unique_ptr<Expressions::EvaluatedScope>& scopename, const std::string& path);

}//
#endif