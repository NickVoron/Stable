// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include "expression.h"
#include "const.h"
#include "array.h"
#include "scopeNames.h"

#include "conversion/convertType.h"

namespace Expressions
{

template<typename Functor>
bool arrayUnroller(Expressions::ScopeNames& scopeName, const Expressions::Expression* arrayData, Functor unrollInstanceFunctor)
{
	ENFORCE(arrayData);

	arrayData = arrayData->evaluated(scopeName);
	if (auto expr = arrayData->cast< Expressions::Const<int> >())
	{
		for (int i = 0; i < expr->value; i++)
		{
			scopeName.add("iterator", Expressions::convertType(i), Expressions::ScopeNames::REPLACE_OR_INSERT);
			scopeName.add("index", Expressions::convertType(i), Expressions::ScopeNames::REPLACE_OR_INSERT);
			unrollInstanceFunctor();
		}
	}
	else if (auto expr = arrayData->cast< Expressions::Array >())
	{
		for (std::size_t i = 0; i < expr->count(); i++)
		{
			scopeName.add("iterator", expr->element(i), Expressions::ScopeNames::REPLACE_OR_INSERT);
			scopeName.add("index", Expressions::convertType(i), Expressions::ScopeNames::REPLACE_OR_INSERT);
			unrollInstanceFunctor();
		}
	}
	else
	{
		THROW("ArrayData type no Array and no Const<int>: " + arrayData->string())
	}

	return true;
}


class This : public Expression
{
public:
	virtual bool boolean() const { return true; }
	virtual std::string string() const { return "this"; }
};


class Proxy : public Expression
{
public:
	struct PathElement
	{
		virtual ~PathElement() {}

		virtual const Expression* evaluate(const Expression* input, const ScopeNames& context) const = 0;
		virtual std::string string() const { return ""; }
	};
		
	
	struct Path : std::vector<std::unique_ptr<PathElement>>
	{
		const Expression* evaluate(const ScopeNames& environment, boost::any* userData = 0) const;
		bool canResolveReverence(const ScopeNames& rootEnvironment) const;
		std::string string() const;
	};

	
	Proxy() {}
	Proxy(PathElement* element);

	void addPathElement(PathElement* element);

	const Path& getPath() const { return path; } 
	virtual std::string string() const override;
	virtual const Expression* evaluated(const ScopeNames& environment, boost::any* userData = 0) const override;

	virtual References references() const override;
	bool canResolveReverence(const ScopeNames& rootEnvironment) const;

	bool extraFlag;

protected:
	Path path;
private:

	mutable const Expression* evaluatedValue = 0;
};


struct PropertyPath : public Expressions::Proxy::PathElement
{
	PropertyPath(const std::string& name_) : name(name_){}
	virtual const Expressions::Expression* evaluate(const Expressions::Expression* previous, const Expressions::ScopeNames& context) const override;
	virtual std::string string() const override { return name; }

	const std::string name;
};



struct ArrayPath : public Proxy::PathElement
{
	ArrayPath(int index_) : index(index_){}
	const size_t index;

	virtual const Expression* evaluate(const Expression* input, const ScopeNames& context) const override;
	virtual std::string string() const override { return str::stringize("[", index, "]"); }
};



}




// Copyright (C) 2014-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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