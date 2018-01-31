// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "reference.h"
#include "array.h"
#include "holder.h"
#include "namescope.h"

#include "conversion/varToType.h"
#include "conversion/convertType.h"

#include "reflection/library.include.h"
#include <numeric>

namespace Expressions
{

Reference::Reference(PathElement* element) 
{
	addPathElement(element);
}

Reference::Reference(Expression* head_):
	head(head_)
{

}

void Reference::addPathElement(PathElement* element)
{
	path.add(element);
}

EvaluationUnit* Reference::evaluated(EvaluatedScope& environment) const
{
	auto unit = add<ReferenceUnit>(environment, *this);

	auto pathlen = path.size();
	if (head)
	{
		pathlen += 1;
	}
	
	ENFORCE_GREATER(pathlen, 0);
	unit->path.resize(pathlen);
	unit->cursor = 0;

	auto& element = unit->path[0];
	element.indexer = add<SimpleIndexerUnit>(environment, *this);
	element.sampler = add<SimpleSamplerUnit>(environment, *this);

	return unit;
}

std::string Reference::string() const
{
	return str::spaced("proxy:", path.string());
}


void Reference::Path::add(PathElement* element)
{
	emplace_back(element);
	path_string = string();
}


std::string Reference::Path::string() const
{
	str::stringize result;
	bool first = true;
	for (auto& element : *this)
	{
		if (auto arrayPath = element->cast<ArrayPathElement>())
		{
			result(str::nodelim(), "");
		}
		else
		{
			if (!first)
			{
				result(".");
			}
		}

		result(element->string());

		first = false;
	}

	return result;
}

std::string Reference::Path::root() const
{
	ENFORCE(size() > 0);
	auto& rootPath = at(0);	
	auto rootProperty = rootPath->cast<PropertyPathElement>();
	ENFORCE(rootProperty);
	return rootProperty->name;
}




ReferenceUnit::ReferenceUnit(EvaluatedScope& parent, const Reference& reference_) : 
	EvaluationUnit("reference_unit", reference_, parent)
	, reference(reference_)
{

}

std::string ReferenceUnit::string() const
{
	return reference.string();
}


const Expression* refexpr(std::size_t cursor, const Reference& reference)
{
	if (reference.head)
	{
		if (cursor == 0)
		{
			return reference.head;
		}
		else
		{
			return reference.path.at(cursor - 1).get();
		}
	}
	else
	{
		return reference.path.at(cursor).get();
	}

	return nullptr;
}

void pre_evaluate(ReferenceUnit::PathElement& element, std::size_t cursor, const Reference& reference, EvaluatedScope& context)
{
	if (!element.element)
	{
		element.element = refexpr(cursor, reference)->evaluated(context);
	}
}


EvaluationUnit* un_array(EvaluationUnit* unit);
void evaluate(EvaluatedScope& scope, ReferenceUnit::PathElement& prev, const Reference::PathElement& nextProto, ReferenceUnit::PathElement& next);

EvaluationInfo ReferenceUnit::evaluate()
{
	if (path.size() == 1)
	{
		pre_evaluate(path[0], cursor, reference, scope());
		EvaluationInfo result;
		result.reject(path[0].element);
		return result;
	}
	else
	{
		PathElement& prev = path[cursor];
		pre_evaluate(prev, cursor, reference, scope());
		auto pathSize = path.size();
		auto nextIdx = cursor + 1;
		auto elementProto = refexpr(cursor, reference);
		ENFORCE_POINTER(elementProto);
		if(auto prototype = elementProto->cast<Reference::PathElement>())
		{
			if (nextIdx < pathSize)
			{
				if (auto& element = path[nextIdx]; !element.element)
				{
					ENFORCE_POINTER(elementProto);

					if (!element.element)
					{
						Expressions::evaluate(scope(), prev, *prototype, element);
						if (element.element)
						{
							++cursor;
							return EvaluationInfo(Reject);
						}
					}
				}
			}
			else if (nextIdx == pathSize)
			{
				PathElement endpoint;
				Expressions::evaluate(scope(), prev, *prototype, endpoint);
				if (endpoint.element)
				{
					endpoint.element = un_array(endpoint.element);
					EvaluationInfo result;
					result.reject(endpoint.element);
					return result;
				}
			}
		}
		else
		{
			
		}
	}

	return EvaluationInfo(Impossible);
}

void ReferenceUnit::extract(EvaluationSet& result)
{
	PathElement& head = path[cursor];
	if(head.element)
	{
		result.add(&head.element);
		head.element->extract(result);
	}

	if (head.indexer)
	{
		result.add(&head.indexer);
		head.indexer->extract(result);
	}

	if (head.sampler)
	{
		result.add(&head.sampler);
		head.sampler->extract(result);
	}
}




ArrayContainer* linearize_array(ArrayContainer& unit)
{
	auto array = add<Array>()->evaluated(unit.scope())->cast<ArrayContainer>();
	linearize(unit, array->elements);
	return array;
}

EvaluationInfo array_indexer(EvaluatedScope& scope, ArrayContainer& arrayData, const Reference::PathElement& prototype, EvaluationUnit*& indexer)
{
	if (indexer)
	{
		auto result = indexer->evaluate();
		if (result != Complete)
		{
			return result;
		}
		else if(indexer->cast<ArrayContainer>())
		{
			result.complete(indexer);
		}

		return result;
	}
	else
	{
		if (!indexer)
		{
			indexer = prototype.createIndexer(scope);
		}
	}

	return EvaluationInfo(Impossible);
}

EvaluationInfo array_sampler(ArrayContainer& arrayData, const Reference::PathElement& prototype, EvaluationUnit*& sampler)
{
	if (sampler)
	{
		auto result = sampler->evaluate();
		if (result != Complete)
		{
			return result;
		}
		else
		{
			result.replace(&arrayData);
		}

		return result;
	}
	else
	{
		sampler = prototype.createSampler(arrayData.scope());
	}

	return EvaluationInfo(Impossible);
}

EvaluationUnit* evaluate_sample(EvaluationUnit* input, EvaluationUnit* sampler)
{
 	ENFORCE_POINTER(input);
 	ENFORCE_POINTER(sampler);
	
	auto baseSampler = sampler->cast<BaseSamplerUnit>();
	ENFORCE_POINTER(baseSampler);
	auto result = baseSampler->sample(input);
	return result;
}

EvaluationUnit* un_array(EvaluationUnit* unit)
{
	ENFORCE_POINTER(unit);
	
	if (auto head = unit->cast<ArrayContainer>(); !head)
	{
		return unit;
	}
	else
	{
		if (head->count() == 1)
		{
			return un_array(head->element(0));
		}
	}

	return unit;
}

void evaluate(EvaluatedScope& scope, ReferenceUnit::PathElement& prev, const Reference::PathElement& prevProto, ReferenceUnit::PathElement& next)
{
	ENFORCE_POINTER(prev.element);

	
	if (auto result = prev.element->evaluate(); result != Complete)
		return;
	
	
	ArrayContainer* head = to_array(prev.element);

	
	if (auto result = array_indexer(scope, *head, prevProto, prev.indexer); result != Complete || !prev.indexer->cast<ArrayContainer>())
		return;

	
	if (auto result = array_sampler(*head, prevProto, prev.sampler); result != Complete)
		return;

	
	
	
	auto output = add<Array>()->evaluated(scope)->cast<ArrayContainer>();
	std::vector<int> indices;
	linearize(*prev.indexer, indices);
	
	if(indices.empty())
	{
		indices.resize(head->count());
		std::iota(indices.begin(), indices.end(), 0);
	}

	for (auto idx : indices)
	{
		auto oldelem = head->element(idx);
		if(oldelem)
		{
			auto newelem = evaluate_sample(oldelem, prev.sampler);
			output->add(newelem);
		}
	}

	next.element = linearize_array(*output);
}




SimpleSamplerUnit::SimpleSamplerUnit(EvaluatedScope& parent, const Expression& proto) :
	BaseSamplerUnit(parent, proto) 
{

}




NameSamplerUnit::NameSamplerUnit(EvaluatedScope& parent, const PropertyPathElement& proto) : 
	BaseSamplerUnit(parent, proto), element(proto) 
{

}

EvaluationUnit* NameSamplerUnit::sample(EvaluationUnit* source) const
{
	auto& scope = source->scope();
	return scope.get(element.name);
}




BaseIndexerUnit::BaseIndexerUnit(EvaluatedScope& parent, const Expression& proto) :
	EvaluationUnit("indexer", proto, parent)
{
	params = add<Array>()->evaluated(parent);
}


EvaluationInfo BaseIndexerUnit::evaluate()
{
	ENFORCE_POINTER(params);
	auto evalinfo = params->evaluate();
	if (evalinfo == Complete)
	{
		evalinfo.complete(params);
	}
	else
	{
		evalinfo.reject(this);
	}

	return evalinfo;
}

void BaseIndexerUnit::extract(EvaluationSet& result)
{
	ENFORCE_POINTER(params);
	result.add(&params);
	params->extract(result);
}




SimpleIndexerUnit::SimpleIndexerUnit(EvaluatedScope& parent, const Expression& proto) : 
	BaseIndexerUnit(parent, proto)
{
}




ArrayIndexerUnit::ArrayIndexerUnit(EvaluatedScope& parent, const Expression& proto, const ConstExprList& inparams) : 
	BaseIndexerUnit(parent, proto) 
{
	params->cast<ArrayContainer>()->elements = inparams.evaluated(parent);
}

						  



EvaluationUnit* PropertyPathElement::evaluated(EvaluatedScope& namescope) const
{
	return add<PropertyPathElementUnit>(namescope, *this);
}

BaseIndexerUnit* PropertyPathElement::createIndexer(EvaluatedScope& namescope) const
{
	return add<SimpleIndexerUnit>(namescope, *this);
}

BaseSamplerUnit* PropertyPathElement::createSampler(EvaluatedScope& namescope) const
{
	return add<NameSamplerUnit>(namescope, *this);
}

std::string PropertyPathElement::string() const 
{
	return name;
}




EvaluationUnit* ArrayPathElement::evaluated(EvaluatedScope& namescope) const
{
	return add<ArrayPathElementUnit>(namescope, *this);
}

BaseIndexerUnit* ArrayPathElement::createIndexer(EvaluatedScope& namescope) const
{
	return add<ArrayIndexerUnit>(namescope, *this, params);
}

BaseSamplerUnit* ArrayPathElement::createSampler(EvaluatedScope& namescope) const
{
	return add<SimpleSamplerUnit>(namescope, *this);
}




std::string CallablePathElement::string() const
{
	return callable.string();
}

EvaluationUnit* CallablePathElement::evaluated(EvaluatedScope& namescope) const
{
	return callable.evaluated(namescope);
}

BaseIndexerUnit* CallablePathElement::createIndexer(EvaluatedScope& namescope) const
{
	return add<SimpleIndexerUnit>(namescope, *this);
}

BaseSamplerUnit* CallablePathElement::createSampler(EvaluatedScope& namescope) const
{
	return add<SimpleSamplerUnit>(namescope, *this);
}




std::string BasePathElementUnit::string() const
{
	return str::stringize(scope().scopeName, ":", element.string());
}

EvaluationInfo PropertyPathElementUnit::evaluate()
{
	EvaluationInfo result;
	auto ref = element.cast<PropertyPathElement>();
	ENFORCE_POINTER(ref);
	if (auto unit = scope().get(ref->name))
	{		
		result.reject(unit);
	}
	else
	{
		
		
		
		result.impossible();
	}

	return result;
}

}//



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