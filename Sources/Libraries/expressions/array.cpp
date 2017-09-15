// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "array.h"
#include "holder.h"
#include "DefaultLogs/library.include.h"
#include "reference.h"
#include "conversion/convertType.h"


namespace Expressions
{

Array::Array(const ConstExprList& params_) : elements(params_)
{	
}

std::string Array::string() const
{
	str::stringize result("array: ", str::comma());
	result(elements.string());
	return result;
}

std::string Array::typeName() const
{
	return "Array"; 
}

References Array::references() const
{
	return elements.references();
}

EvaluationUnit* Array::evaluated(const EvaluatedScope& environment, boost::any* userData) const
{
	EvaluatedArray* result = Expressions::add<EvaluatedArray>(elements.size(), const_cast<EvaluatedScope*>(&environment));
	result->unEvaluatedElements = elements;
	return result;
}

const Expression* Array::element(std::size_t index) const
{
	if(index >= elements.size())
	{
		if(!elements.empty())
		{
			LOG_ERROR("index is out of range. index: " << index << " range: (" << 0 << ", " << elements.size() - 1 << ")");
		}
		else
		{
			LOG_ERROR("array is empty");
		}		

		return nullptr;
	}

	return elements[index];
}

std::size_t Array::count() const
{ 
	return elements.size(); 
}

void Array::add(const Expression* expr)
{
	elements.push_back(expr);
}



EvaluatedArray::EvaluatedArray(size_t size, const EvaluatedScope* environment_) :
	environment(environment_) 
{
	internalScopename.setParent(environment);

	arrayData = Expressions::convertType((int)size)->evaluated(EvaluatedScope());
}

EvaluatedArray::EvaluatedArray(EvaluationUnit* arrayData_, const EvaluatedScope* environment_):
	arrayData(arrayData_),
	environment(environment_)
{
	internalScopename.setParent(environment);
}

std::string EvaluatedArray::string() const
{
	str::stringize result("evalArray: ", str::comma());
	result(elements.string());
	return result;
}

std::string EvaluatedArray::typeName() const
{
	return "EvaluatedArray";
}

EvaluateState EvaluatedArray::evaluateStep(const Expressions::EvaluatedScope&, boost::any* userData)
{
	
	EvaluateState evalState = Complete;

	
	if (unEvaluatedElements.size())
	{
		ENFORCE_MSG(environment, "");

		evalState = Impossible;
		if (unEvaluatedElements[0]->references().canResolveReverence(*environment))
		{
			evalState = Reject;
			for (auto& elemet: unEvaluatedElements)
			{
				elements.push_back(elemet->evaluated(*environment));
			}
			unEvaluatedElements.clear();
		}
	}

	
	ENFORCE(arrayData);

	if (auto expr = arrayData->cast< Expressions::Const<int> >())
	{
		for (int i = 0; i < expr->value; i++)
		{
			EvaluationUnit* index = Expressions::convertType(i)->evaluated(EvaluatedScope());
			internalScopename.add("iterator", index, InsertMethod::REPLACE_OR_INSERT);
			internalScopename.add("index", index, InsertMethod::REPLACE_OR_INSERT);

			const EvaluationUnit* unit = elements[i];
			EvaluateState unitState = const_cast<EvaluationUnit*>(unit)->evaluateStep(internalScopename, userData);
			evalState = merge(evalState, unitState);
		}
	}
	else if (auto expr = arrayData->cast<Expressions::EvaluatedArray>())
	{
		for (int i = 0; i < expr->count(); i++)
		{
			Expression* indexExpr = Expressions::convertType(i);
			internalScopename.add("iterator", const_cast<EvaluationUnit*>(expr->element(i)), InsertMethod::REPLACE_OR_INSERT);
			internalScopename.add("index", indexExpr->cast<Expressions::Const<int>>(), InsertMethod::REPLACE_OR_INSERT);

			const EvaluationUnit* unit = elements[i];
			EvaluateState unitState = const_cast<EvaluationUnit*>(unit)->evaluateStep(internalScopename, userData);
			evalState = merge(evalState, unitState);
		}
	}
	else
	{
		THROW("ArrayData type no Array and no Const<int>: " + arrayData->string())
	}


	return evalState;
}


const EvaluationUnit* EvaluatedArray::child(const PropertyPath* path) const
{
	EvaluatedArray* result = Expressions::add<EvaluatedArray>(elements.size(), environment);
	for (auto& element : elements)
	{
		const EvaluationUnit* elementChild = element->child(path);
		if (elementChild)
		{
			if (const EvaluatedArray* elementArray = elementChild->cast<EvaluatedArray>())
			{
				for (auto& childArrayElement : elementArray->elements)
				{
					result->add(childArrayElement);
				}
			}
			else
			{
				result->add(elementChild);
			}
		}
		else
		{
			result = 0;
			break;
		}
	}

	return result;
}

const EvaluationUnit* EvaluatedArray::child(const ArrayPath* path) const
{
	return element(path->index);
}

const EvaluationUnit* EvaluatedArray::element(std::size_t index) const
{
	if (index < 0 || index >= elements.size())
	{
		if (!elements.empty())
		{
			LOG_ERROR("index is out of range. index: " << index << " range: (" << 0 << ", " << elements.size() - 1 << ")");
		}
		else
		{
			LOG_ERROR("array is empty");
		}

		return nullptr;
	}

	return elements[index];
}

std::size_t EvaluatedArray::count() const
{
	return elements.size();
}

void EvaluatedArray::add(const EvaluationUnit* expr)
{
	elements.push_back(expr);
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