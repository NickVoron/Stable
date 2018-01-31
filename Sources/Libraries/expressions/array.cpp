// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "array.h"
#include "holder.h"
#include "defaultLogs/library.include.h"
#include "reference.h"
#include "conversion/convertType.h"

#if !defined(USE_APPLE)
#include <optional>
#else
#include <experimental/optional>
namespace std
{
    template<class T>
    using optional = std::experimental::optional<T>;
}
#endif


namespace Expressions
{

Array::Array(const ConstExprList& params_) : elements(params_)
{	
}

std::string Array::string() const
{
	return str::stringize("array: ", str::comma(), elements.string());
}

std::string Array::typeName() const
{
	return "Array"; 
}

EvaluationUnit* Array::evaluated(EvaluatedScope& environment) const
{
    auto element = elements.evaluated(environment);
	return Expressions::add<ArrayContainer>(*this, environment, element);
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





ArrayContainer::ArrayContainer(const Expression& proto, EvaluatedScope& parent) :
	EvaluationUnit("array_container", proto, parent)
{
}

ArrayContainer::ArrayContainer(const Expression& proto, EvaluatedScope& parent, EvaluationUnitsList& elements_) :
	EvaluationUnit("array_container", proto, parent)
	, elements(elements_)
{
}

std::string ArrayContainer::string() const
{
	return str::stringize("array:", str::comma(), elements.string());
}

std::string ArrayContainer::typeName() const
{
	return "ArrayContainer";
}

EvaluationInfo ArrayContainer::evaluate()
{
	EvaluationInfo result(Impossible);
		
	bool complete = true;
	for(auto& unit : elements)
	{
		complete &= (unit->evaluate() == Complete);

		if(complete)
		{
			if(unit->cast<ArrayContainer>())
			{
				result.reject(this);
			}
		}
	}

	if(complete)
	{
		result.complete(this);
	}
	
	return result;
}

void ArrayContainer::extract(EvaluationSet& result)
{
	EvaluationUnitsList linearized;
	linearize(elements, linearized);
	elements = linearized;
	elements.extract(result);
}

EvaluationUnit*& ArrayContainer::add(EvaluationUnit* expr)
{
	ENFORCE_POINTER(expr);
	elements.push_back(expr);
	return elements.back();
}

template<class Container>
std::optional<std::size_t> checkIdx(const Container& container, std::size_t index)
{
	if (index >= container.size())
	{
		if (!container.empty())
		{
			LOG_ERROR("index is out of range. index: " << index << " range: [" << 0 << ", " << container.size() - 1 << "]");
		}
		else
		{
			LOG_ERROR("array is empty");
		}

		return {};
	}

	return index;
}

const EvaluationUnit* ArrayContainer::element(std::size_t index) const
{
	auto idx = checkIdx(elements, index);
	return idx ? elements[*idx] : nullptr;
}

EvaluationUnit* ArrayContainer::element(std::size_t index)
{
	auto idx = checkIdx(elements, index);
	return idx ? elements[*idx] : nullptr;
}

std::size_t ArrayContainer::count() const
{
	return elements.size();
}

ArrayContainer* to_array(EvaluationUnit* unit)
{
	ENFORCE_POINTER(unit);
	ArrayContainer* result = nullptr;

	if (auto head = unit->cast<ArrayContainer>(); !head)
	{
		
		head = add<Array>()->evaluated(unit->scope())->cast<ArrayContainer>();
		head->add(unit);
		result = head;
	}
	else
	{
		
		result = head;
	}

	ENFORCE_POINTER(result);

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