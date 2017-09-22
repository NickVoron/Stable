// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <vector>
#include <string>
#include <boost/any.hpp>

#include "containers/library.include.h"
#include "reflection/library.include.h"

namespace Expressions
{
	template<typename ExpressionType>
	class ScopeNames;

	struct PropertyPath;
	struct ArrayPath;
	class Reference;
	class Expression;
	class EvaluationUnit;

	typedef ScopeNames<const Expression> ExpressionScope;
	typedef ScopeNames<EvaluationUnit> EvaluatedScope;

	
	struct References: public std::vector<const Reference*>
	{
		bool canResolveReference(const EvaluatedScope& scopename);
	};

	
	enum EvaluateState
	{
		Impossible = 0,		
		Reject,			
		Complete		

	};
	Expressions::EvaluateState merge(Expressions::EvaluateState first, Expressions::EvaluateState second);

	class Expression
	{
	public:
		Expression() {}

		Expression(const Expression&) = delete;
		const Expression& operator=(const Expression&) = delete;

		virtual ~Expression() {}

		template <class T> const T* cast() const { return dynamic_cast<const T*>(this); }
		template <class T> T* cast() { return dynamic_cast<T*>(this); }

		
		
		virtual bool elementary() const { return false; }

		
		virtual std::string string() const { return "unknown Expression"; }
		virtual std::vector<std::string> fields() const { return std::vector<std::string>(); }

		
		virtual bool boolean() const { return false; }

		virtual std::unique_ptr<mirror::runtime::Type> reflectedType() const { return nullptr; }

		
		virtual std::string typeName() const { return "unknown operand type"; }

		

		
		
		virtual EvaluationUnit* evaluated(const EvaluatedScope& parentScopename) const = 0;
		virtual References references() const { return References(); }
	};

	
	class EvaluationUnit : public Expression
	{
	public: 
		EvaluationUnit(const EvaluatedScope& parent_);
		virtual References references() const override;
		virtual EvaluationUnit* evaluated(const EvaluatedScope& parentScopename) const override;

		
		virtual EvaluateState evaluateStep(const EvaluatedScope& parentScopename);

		
		virtual const EvaluationUnit* child(const PropertyPath* path) const { return 0; }
		virtual const EvaluationUnit* child(const ArrayPath* path) const	{ ENFORCE(false); return 0; }
	
		static EvaluatedScope commonParent;

	protected:
		bool isParent(const EvaluatedScope& scope);
	private:
		uint64_t parent;
	};

	
	template<class ValueType> static std::string stringize(ValueType value);
	template<> static std::string stringize<std::string>(std::string value) { return value; };
	template<> static std::string stringize<const Expressions::Expression*>(const Expressions::Expression* value) { return value ? value->string() : "#nullptr"; };

	enum TypeSelector
	{
		CONSTANT = 1 << 0,
		CONTAINER = 1 << 1,
		STRUCTURE = 1 << 2,
	};
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