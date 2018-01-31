// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>

#include "reflection/library.include.h"
#include "multimethods2/library.include.h"

#include "namescope.h"

namespace Expressions
{
	struct PropertyPath;
	struct EvaluationUnitsList;
	class Reference;
	class Expression;
	class EvaluationUnit;	

	typedef NameScope<const Expression> ExpressionScope;
	typedef NameScope<EvaluationUnit> EvaluatedScope;

	
	enum EvaluationState
	{
		Empty,			
		Impossible,		
		Reject,			
		Complete,		
	};

	class EvaluationInfo
	{
	public:
		EvaluationInfo() {}
		explicit EvaluationInfo(EvaluationState state) : evalstate(state) {}
		bool operator==(EvaluationState other_state) const { return evalstate == other_state; }
		bool operator!=(EvaluationState other_state) const { return !operator==(other_state); }
		void operator+=(const EvaluationInfo& info);
		EvaluationInfo operator+(const EvaluationInfo& info) const;

		EvaluationState state() const { return evalstate; };

		void reject(EvaluationUnit* evaluated);
		void impossible();
		void complete(EvaluationUnit* evaluated);
	
		void replace(EvaluationUnit* evaluated) { unit = evaluated; };
		EvaluationUnit* replacement() { return unit; };

	private:
		EvaluationUnit* unit = nullptr;		
		EvaluationState evalstate = Empty;
	};

	struct location
	{
		std::string filename;
		unsigned int line = 0;
	};

	class Expression
	{
	public:
		Expression() {}

		Expression(const Expression&) = delete;
		const Expression& operator=(const Expression&) = delete;

		virtual ~Expression() {}

		template <class T> const T* cast() const { return dynamic_cast<const T*>(this); }
		template <class T> T* cast() { return dynamic_cast<T*>(this); }

		virtual std::string string() const { return "unknown Expression"; }

		
		virtual std::string typeName() const { return "unknown operand type"; }

		
		
		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const = 0;

		std::string location_str() const;

		location loc;
	};

	
	class EvaluationUnit : public multimethods2::ClientT<EvaluationUnit>
	{
	public: 
		struct EvaluationSet
		{
			bool exists(EvaluationUnit** unit) const;
			void add(EvaluationUnit** unit);

			std::set<EvaluationUnit**> result;
		};

		EvaluationUnit(const std::string& namescopeName, const Expression& protoexpr, EvaluatedScope& parent);

		template <class T> const T* cast() const { return dynamic_cast<const T*>(this); }
		template <class T> T* cast() { return dynamic_cast<T*>(this); }

		virtual std::unique_ptr<mirror::runtime::Type> reflectedType() const { return nullptr; }
				
		
		virtual std::string string() const { return "unknown evaluation unit"; }
		virtual std::string typeName() const { return "unknown evaluation unit type"; }

		EvaluatedScope& scope() { return internalScope; };
		const EvaluatedScope& scope() const { return internalScope; };
		virtual EvaluationInfo evaluate();
		virtual void extract(EvaluationSet& result);

		std::string path() const;
		bool equal(const EvaluationUnit* other) const { return (this == other) || deepEqual(other); }

		uint64_t id = newid();
		const Expression& protoexpr;

		static EvaluatedScope commonParent;

	protected:
		virtual bool deepEqual(const EvaluationUnit* other) const { return false; }
		EvaluatedScope internalScope;
	};

	void extractDependencies(EvaluatedScope& scope, EvaluationUnit::EvaluationSet& result);
	EvaluationUnit* unrollExpression(const Expression& expression, EvaluatedScope& namescope, bool debugLog);

	
	template<class ValueType> std::string stringize(ValueType value);
	template<> std::string stringize<std::string>(std::string value);
	template<> std::string stringize<const Expressions::Expression*>(const Expressions::Expression* value);

	enum TypeSelector
	{
		CONSTANT  = 1 << 0,
		CONTAINER = 1 << 1,
		STRUCTURE = 1 << 2,
		PAIR	  = 1 << 3,
		TUPLE	  = 1 << 4,
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