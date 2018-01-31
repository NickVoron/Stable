// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"
#include "constExprList.h"
#include "callable.h"


namespace Expressions
{

class Array;


class This : public EvaluationUnit
{
public:
	virtual bool boolean() const { return true; }
	virtual std::string string() const { return "this"; }
};


class BaseIndexerUnit : public EvaluationUnit
{
public:
	BaseIndexerUnit(EvaluatedScope& parent, const Expression& proto);

	virtual std::string string() const override { return "indexer"; }
	virtual std::string typeName() const override { return "indexer"; }

	virtual EvaluationInfo evaluate() override;
	virtual void extract(EvaluationSet& result) override;

	EvaluationUnit* params = nullptr;
};


class BaseSamplerUnit : public EvaluationUnit
{
public:
	BaseSamplerUnit(EvaluatedScope& parent, const Expression& proto) : EvaluationUnit("sampler", proto, parent) {}

	virtual std::string string() const override { return "sampler"; }
	virtual std::string typeName() const override { return "sampler"; }

	virtual EvaluationUnit* sample(EvaluationUnit* source) const = 0;
};



class Reference : public Expression
{
public:
	struct PathElement : public Expression
	{
		virtual std::string string() const override { return "|unknown path element|"; }
		virtual std::string typeName() const override { return "|unknown path element type|"; }
		virtual BaseIndexerUnit* createIndexer(EvaluatedScope& namescope) const = 0;
		virtual BaseSamplerUnit* createSampler(EvaluatedScope& namescope) const = 0;
	};
		
	
	struct Path : std::vector<std::unique_ptr<PathElement>>
	{
		void add(PathElement* element);
		std::string string() const;
		std::string root() const; 
		std::string path_string;
	};

	
	Reference() {}
	Reference(PathElement* element);
	Reference(Expression* head);

	void addPathElement(PathElement* element);

	virtual std::string string() const override;
	virtual EvaluationUnit* evaluated(EvaluatedScope& environment) const override;

	Path path;
	Expression* head = nullptr;
};


class SimpleIndexerUnit : public BaseIndexerUnit
{
public:
	SimpleIndexerUnit(EvaluatedScope& parent, const Expression& proto);
};


class ArrayIndexerUnit : public BaseIndexerUnit
{
public:
	ArrayIndexerUnit(EvaluatedScope& parent, const Expression& proto, const ConstExprList& params);
};

struct PropertyPathElement;

class SimpleSamplerUnit : public BaseSamplerUnit
{
public:
	SimpleSamplerUnit(EvaluatedScope& parent, const Expression& proto);
	virtual EvaluationUnit* sample(EvaluationUnit* source) const override { return source; }
};


class NameSamplerUnit : public BaseSamplerUnit
{
public:
	NameSamplerUnit(EvaluatedScope& parent, const PropertyPathElement& proto);

	virtual EvaluationUnit* sample(EvaluationUnit* source) const override;
	const PropertyPathElement& element;
};


class ReferenceUnit : public EvaluationUnit
{
public:
	struct PathElement
	{
		EvaluationUnit* element{ nullptr };
		EvaluationUnit* indexer{ nullptr };
		EvaluationUnit* sampler{ nullptr };
	};

	ReferenceUnit(EvaluatedScope& parent, const Reference& reference_);

	virtual std::string string() const override;
	virtual std::string typeName() const override { return str::spaced("reference unit"); }
	virtual EvaluationInfo evaluate() override;
	virtual void extract(EvaluationSet& result);

	const Reference& reference;
	std::size_t cursor;
	std::vector<PathElement> path;
};


struct PropertyPathElement : public Reference::PathElement
{
	PropertyPathElement(const std::string& name_) : name(name_) {}

	virtual std::string string() const override;
	virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const;
	virtual BaseIndexerUnit* createIndexer(EvaluatedScope& namescope) const;
	virtual BaseSamplerUnit* createSampler(EvaluatedScope& namescope) const;

	const std::string name;
};


struct ArrayPathElement : public Reference::PathElement
{
	ArrayPathElement(const ConstExprList& params_) : params(params_) {}

	virtual std::string string() const override { return str::stringize("[", params.naked_string(), "]"); }
	virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const;
	virtual BaseIndexerUnit* createIndexer(EvaluatedScope& namescope) const;
	virtual BaseSamplerUnit* createSampler(EvaluatedScope& namescope) const;

	ConstExprList params;
};


struct CallablePathElement : public Reference::PathElement
{
	CallablePathElement(const Callable& callable_) : callable(callable_) {}

	virtual std::string string() const override;
	virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const;
	virtual BaseIndexerUnit* createIndexer(EvaluatedScope& namescope) const;
	virtual BaseSamplerUnit* createSampler(EvaluatedScope& namescope) const;

	const Callable& callable;
};

struct BasePathElementUnit : public EvaluationUnit
{
	BasePathElementUnit(EvaluatedScope& parent, const Reference::PathElement& element_) : EvaluationUnit("path_element", element_, parent), element(element_) {}

	virtual std::string string() const override;
	const Reference::PathElement& element;
};


struct PropertyPathElementUnit : public BasePathElementUnit
{
	PropertyPathElementUnit(EvaluatedScope& parent, const PropertyPathElement& element_) : BasePathElementUnit(parent, element_) {}
	virtual EvaluationInfo evaluate() override;
};



struct ArrayPathElementUnit : public BasePathElementUnit
{
	ArrayPathElementUnit(EvaluatedScope& parent, const ArrayPathElement& element_) : BasePathElementUnit(parent, element_) {}
};

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