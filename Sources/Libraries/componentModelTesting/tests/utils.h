// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

}
#endif



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