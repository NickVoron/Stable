#include "array.h"
#include "holder.h"
#include "DefaultLogs/library.include.h"
#include "reference.h"
#include "conversion/convertType.h"

#include <optional>


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
	return Expressions::add<ArrayContainer>(*this, environment, elements.evaluated(environment));
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


//
//
//
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
	return idx ? elements[idx.value()] : nullptr;
}

EvaluationUnit* ArrayContainer::element(std::size_t index)
{
	auto idx = checkIdx(elements, index);
	return idx ? elements[idx.value()] : nullptr;
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
		//LOG_MSG("make array envelope for: " << unit->string());
		head = add<Array>()->evaluated(unit->scope())->cast<ArrayContainer>();
		head->add(unit);
		result = head;
	}
	else
	{
		//LOG_MSG("head is " << head->string());
		result = head;
	}

	ENFORCE_POINTER(result);

	return result;
}

}//
