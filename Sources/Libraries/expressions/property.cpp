// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "property.h"
#include "reference.h"
#include "holder.h"
#include "scopeNames.h"

namespace Expressions
{

void Properties::add(const std::string& name, const Expressions::Expression* value)
{
	this->emplace(name, value);
}

const Expression* Properties::get(const std::string& name) const
{
	auto it = find(name);
	return it != end() ? it->second : 0;
}

std::string Properties::string() const
{
	str::stringize result("(");

	for (auto& value : *this)
	{
		ENFORCE(value.second);
		result(value.first, " = ", Expressions::stringize(value.second->string()));
	}

	return result(")");
}


PropertiesStruct::PropertiesStruct(const std::string& typeName) : StructBase(typeName)
{
	
}

EvaluationUnit* PropertiesStruct::evaluated(const EvaluatedScope& environment) const
{
	EvalPropertiesStruct* evalStruct = Expressions::add<EvalPropertiesStruct>(structType);
	for (auto& iter: *this)
	{
		evalStruct->unEvaluatedPropertyies.add(iter.first, iter.second);
	}

	return evalStruct;
}

std::string PropertiesStruct::string() const
{
	std::string result;

	result += "properties struct: ";
	result += Properties::string();
		
	return result;
}

References PropertiesStruct::references() const
{
	References refs;
	for (auto& iter: *this)
	{
		const References& exprRefs = iter.second->references();
		refs.insert(refs.begin(), exprRefs.begin(), exprRefs.end());
	}

	return refs;
}


void EvalProperties::add(const std::string& name, Expressions::EvaluationUnit* value)
{
	this->emplace(name, value);
}

const EvaluationUnit* EvalProperties::get(const std::string& name) const
{
	auto it = find(name);
	return it != end() ? it->second : 0;
}

std::string EvalProperties::string() const
{
	return "";
}


EvalPropertiesStruct::EvalPropertiesStruct(const std::string& typeName): 
	StructBase(typeName)
{

}

std::string EvalPropertiesStruct::string() const
{
	return typeName();
}

EvaluateState EvalPropertiesStruct::evaluateStep(const Expressions::EvaluatedScope& parentScopename)
{
	EvaluateState evalState = Complete;

	if (unEvaluatedPropertyies.size())
	{
		evalState = Expressions::Impossible;  

		for (auto iter = unEvaluatedPropertyies.cbegin(); iter != unEvaluatedPropertyies.cend();)
		{
			const std::string& name = iter->first;
			const Expression* expr = iter->second;

			References refs = expr->references();
			if (refs.canResolveReference(parentScopename))
			{
				EvaluationUnit* evalUnit = expr->evaluated(parentScopename);
				add(name, evalUnit);
				evalState = Reject;	

				unEvaluatedPropertyies.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	for (auto& iter : *this)
	{
		EvaluationUnit* unit = iter.second;

		EvaluateState unitState = const_cast<EvaluationUnit*>(unit)->evaluateStep(parentScopename);
		evalState = merge(evalState, unitState);
	}


	return evalState;
} 

const EvaluationUnit* EvalPropertiesStruct::child(const PropertyPath* path) const
{
	const EvaluationUnit* expr = get(path->name);
	return expr;
}

std::vector<std::string> EvalPropertiesStruct::fields() const
{
	std::vector<std::string> result;
	for (auto& member : *this)
	{
		result.push_back(member.first);
	}
	return result;
}

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