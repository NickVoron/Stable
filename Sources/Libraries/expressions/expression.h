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

	//expression
	enum EvaluationState
	{
		Empty,			// значение при инициализации
		Impossible,		// на текущем шаге не было ни одного раскрученного
		Reject,			// часть была раскрученна, но ещё остались 
		Complete,		// все обьекты раскрученны
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

		// тип, применяется для оладочных условий сравнения типов и что бы знать как интерпретировать данное выражение
		virtual std::string typeName() const { return "unknown operand type"; }

		// интерфейс необходимый для расскрутки
		// начало раскрутки, отдает evaluationUnit, который дальше будет раскручиваться итеративно( и хранить в себе состойние раскрутки
		virtual EvaluationUnit* evaluated(EvaluatedScope& namescope) const = 0;

		std::string location_str() const;

		location loc;
	};

	//
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
				
		// шаг раскрутки
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

	//
	template<class ValueType> static std::string stringize(ValueType value);
	template<> static std::string stringize<std::string>(std::string value) { return value; };
	template<> static std::string stringize<const Expressions::Expression*>(const Expressions::Expression* value) { return value ? value->string() : "#nullptr"; };

	enum TypeSelector
	{
		CONSTANT  = 1 << 0,
		CONTAINER = 1 << 1,
		STRUCTURE = 1 << 2,
		PAIR	  = 1 << 3,
		TUPLE	  = 1 << 4,
	};
}


