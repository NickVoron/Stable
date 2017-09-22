// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "arrayContainer.h"
#include "scopeNames.h"
#include "reference.h"
#include "conversion/varToType.h"

namespace Expressions
{

	ArrayContainer::ArrayContainer(const EvaluatedScope& parent):
		EvaluationUnit(parent)
	{
	}


	ArrayContainer::~ArrayContainer()
	{
	}


	std::string ArrayContainer::string() const
	{
		return str::stringize("evalArray: ", str::comma(), elements.string());
	}

	std::string ArrayContainer::typeName() const
	{
		return "ArrayContainer";
	}

	EvaluateState ArrayContainer::evaluateStep(const Expressions::EvaluatedScope&)
	{
		EvaluateState evalState = Complete;
		for (auto unit: elements)
		{
			EvaluateState unitState = const_cast<EvaluationUnit*>(unit)->evaluateStep(EvaluationUnit::commonParent);
			evalState = merge(evalState, unitState);
		}

		return evalState;
	}

	const EvaluationUnit* ArrayContainer::child(const PropertyPath* path) const
	{
		ArrayContainer* result = Expressions::add<ArrayContainer>(EvaluationUnit::commonParent);
		for (auto& element : elements)
		{
			const EvaluationUnit* elementChild = element->child(path);
			if (elementChild)
			{
				if (auto elementArray = elementChild->cast<ArrayContainer>())
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

	std::vector<int> linearize_indices(const ArrayContainer& unit);
	std::vector<int> linearize_indices(const ConstExprList& exprs)
	{
		std::vector<int> result;
		for (auto& inexpr : exprs)
		{
			ENFORCE_POINTER(inexpr);

			auto expr = inexpr->evaluated(EvaluationUnit::commonParent);
			ENFORCE_POINTER(expr);

			EvaluateState evalState = Reject;
			while (evalState != Complete)
			{
				evalState = expr->evaluateStep(EvaluationUnit::commonParent);
				ENFORCE(evalState != Impossible);
			}

			int idx;
			if (Expressions::convertVar(*expr, idx))
			{
				result.push_back(idx);
			}
			else if (auto container = expr->cast<ArrayContainer>())
			{
				auto children = linearize_indices(*container);
				result.insert(result.end(), children.begin(), children.end());
			}
		}
		return result;
	}

	std::vector<int> linearize_indices(const ArrayContainer& array)
	{
		std::vector<int> result;
		auto count = array.count();
		for (std::size_t i = 0; i < count; ++i)
		{
			auto element = array.element(i);
			ENFORCE_POINTER(element);

			int idx;			
			if (Expressions::convertVar(*element, idx))
			{
				result.push_back(idx);
			}
			else if (auto container = element->cast<ArrayContainer>())
			{
				auto children = linearize_indices(*container);
				result.insert(result.end(), children.begin(), children.end());
			}
		}
		return result;
	}

	const EvaluationUnit* ArrayContainer::child(const ArrayPath* path) const
	{
		auto result = Expressions::add<ArrayContainer>(EvaluationUnit::commonParent);
		auto indices = linearize_indices(path->params);

		for (auto idx : indices)
		{
			auto elem = element(idx);
			ENFORCE_POINTER(elem);
			result->add(elem);
 		}

		return result;
	}


	void ArrayContainer::add(const EvaluationUnit* expr)
	{
		elements.push_back(expr);
	}

	const EvaluationUnit* ArrayContainer::element(std::size_t index) const
	{
		if (index < 0 || index >= elements.size())
		{
			if (!elements.empty())
			{
				LOG_ERROR("index is out of range. index: " << index << " range: [" << 0 << ", " << elements.size() - 1 << "]");
			}
			else
			{
				LOG_ERROR("array is empty");
			}

			return nullptr;
		}

		return elements[index];
	}

	std::size_t ArrayContainer::count() const
	{
		return elements.size();
	}

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