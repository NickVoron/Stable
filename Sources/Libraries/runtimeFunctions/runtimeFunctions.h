#pragma

#include <array>
#include <boost/any.hpp>

#include "multimethods2/library.include.h"

namespace RuntimeFunctions
{	
	struct Value : public multimethods2::BaseClient
	{
		virtual boost::any get_value() const = 0;
		
		template<class T>
		bool flush(T& target)
		{
			try
			{
				target = boost::any_cast<T>(get_value());
				return true;
			}
			catch (const std::exception&)
			{
				
			}
			return false;
		}
	};

	template<class T>
	struct ValueT : public multimethods2::ClientT<ValueT<T>, Value>
	{
		ValueT() {}
		ValueT(const T& v) : value(v) {}

		virtual boost::any get_value() const override
		{
			return boost::any(value);
		}

		T value;
	};

	class Operation 
	{
	public:
		virtual Value* execute() = 0;
	};

	class Store : public Operation
	{
	public:
		Store(Value* val = nullptr) : value(val) {}

		virtual Value* execute() override
		{
			return value;
		}

		Value* value;
	};

	template<class OperationClass>
	class BinaryOperationDispatcher : public Operation
	{
	public:
		BinaryOperationDispatcher(Operation* op0 = nullptr, Operation* op1 = nullptr) : input{ op0, op1 } {}

		virtual Value* execute() override
		{
			return dispatch(input[0]->execute(), input[1]->execute());
		}

		std::array<Operation*, 2> input;

	protected:
		Value* dispatch(Value* v0, Value* v1)
		{
			return dispatcher().exec(v0, v1);			
		}

		static multimethods2::Table2<Value*>& dispatcher()
		{
			static multimethods2::Table2<Value*> table;
			return table;
		}		
	};

	template<class Op, class V0, class V1>
	struct ImplementationSelector
	{
		static Value* implementation(ValueT<V0>& v0, ValueT<V1>& v1)
		{
			return Op::implementation(v0, v1);
		}
	};

	struct Add : public BinaryOperationDispatcher<Add>
	{
		Add(Operation* op0 = nullptr, Operation* op1 = nullptr) : BinaryOperationDispatcher(op0, op1) {}

		template<class V0, class V1>
		static Value* implementation(ValueT<V0>& v0, ValueT<V1>& v1)
		{
			return new ValueT<decltype(v0.value + v1.value)>(v0.value + v1.value);
		}

		static void initialize()
		{
			dispatcher().add(&ImplementationSelector<Add, float, float>::implementation);
		}
	};
}