// Copyright (C) 2014-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <string>
#include <map>
#include <list>
#include <memory>
#include <unordered_map>

#include "defaultLogs/library.include.h"
#include "stuff/enforce.h"

namespace VariantFunctions
{
	
	
	
	template<class VariantType, class VariantTypeList>
	struct Executor 
	{
		virtual VariantType* call(const VariantTypeList& expressions) const = 0;
	};

	
	
	
	
	
	
	template<class VariantType, class VariantTypeList>
	struct SimpleExecutor : public Executor<VariantType, VariantTypeList>
	{
		virtual VariantType* call(const VariantTypeList& expressions) const
		{
			ENFORCE(expressions.size() == 0);
			return call();
		}

		virtual VariantType* call() const = 0;
	};


	template<class ReturnValue, class VariantType, class VariantTypeList, class VariantCreator, class VariantConverter>
	struct SimpleExecutorT : public SimpleExecutor<VariantType, VariantTypeList>
	{
		typedef ReturnValue(*Func)();

		SimpleExecutorT(Func func) :function(func){}

		VariantType* call() const { return VariantCreator::create(function()); }

		Func function;
	};

	
	
	
	template<class VariantType, class VariantTypeList>
	struct UnaryExecutor : public Executor<VariantType, VariantTypeList>
	{
		virtual VariantType* call(const VariantTypeList& expressions) const
		{
			ENFORCE(expressions.size() == 1);
			return call(expressions[0]);
		}

		virtual VariantType* call(const VariantType* e0) const = 0;
	};


	template<class ReturnValue, class T, class VariantType, class VariantTypeList, class VariantCreator, class VariantConverter>
	struct UnaryExecutorT : public UnaryExecutor<VariantType, VariantTypeList>
	{
		typedef ReturnValue (*Func)(T);

		UnaryExecutorT(Func func):function(func){}

		VariantType* call(const VariantType* e0) const
		{
			typedef typename std::remove_reference<T>::type Type;
			return VariantCreator::create(function(VariantConverter::template convert<Type>(*e0)));
		}

		Func function;
	};

	
	
	
	template<class VariantType, class VariantTypeList>
	struct BinaryExecutor : public Executor<VariantType, VariantTypeList>
	{
		virtual VariantType* call(const VariantTypeList& expressions) const
		{
			ENFORCE(expressions.size() == 2);
			return call(expressions[0], expressions[1]);
		}

		virtual VariantType* call(const VariantType* e0, const VariantType* e1) const = 0;
	};


	template<class ReturnValue, class T0, class T1, class VariantType, class VariantTypeList, class VariantCreator, class VariantConverter>
	struct BinaryExecutorT : public BinaryExecutor<VariantType, VariantTypeList>
	{
		typedef ReturnValue (*Func)(T0, T1);

		BinaryExecutorT(Func func):function(func){}

		VariantType* call(const VariantType* e0, const VariantType* e1) const
		{
			return VariantCreator::create(function(VariantConverter::template convert<T0>(*e0), VariantConverter::template convert<T1>(*e1)));
		}

		Func function;
	};


	
	
	
	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantName>
	struct ArgInfo
	{
		void init(const VariantType* expr) { type = VariantName::name(expr); }

		template<class T> void init() { type = TypeToName<T>::name(); }

		bool operator==(const ArgInfo& info) const { return type == info.type; }
		bool operator<(const ArgInfo& info) const { return type < info.type; }

		std::string type;
	};

	template<std::size_t count, class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantName>
	struct ArgumentsInfo
	{
		void init(const VariantTypeList& expressions)
		{
			ENFORCE(expressions.size() == count);

			for (std::size_t i = 0; i < count; ++i)
			{
				arguments[i].init( expressions[i] );
			}
		}

		template<class Arg0>
		void init()
		{
			static_assert(count == 1, "");
			arguments[0].template init<Arg0>();
		}

		template<class Arg0, class Arg1>
		void init()
		{
			static_assert(count == 2, "");
			arguments[0].template init<Arg0>();
			arguments[1].template init<Arg1>();
		}

		bool operator<(const ArgumentsInfo& info) const
		{
			static_assert(count == 2 || count == 1, "");

			if(count == 2)
			{
				return std::tie(arguments[0], arguments[1]) < std::tie(info.arguments[0], info.arguments[1]);
			}

			if(count == 1)
			{
				return (arguments[0] < info.arguments[0]);
			}			

			return false;
		}

		ArgInfo<VariantType, VariantTypeList, TypeToName, VariantName> arguments[count];
	};

	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantName>
	struct ArgumentsInfo<0, VariantType, VariantTypeList, TypeToName, VariantName>
	{
		void init(){}
		void init(const VariantTypeList& expressions){}
	};

	
	
	
	template<int argumentsCount, class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantName>
	struct FunctionsBucket : std::map< ArgumentsInfo<argumentsCount, VariantType, VariantTypeList, TypeToName, VariantName>, Executor<VariantType, VariantTypeList>*>
	{	
		typedef ArgumentsInfo<argumentsCount, VariantType, VariantTypeList, TypeToName, VariantName> ArgumentsInfoType;
		typedef Executor<VariantType, VariantTypeList> ExecutorType;

		void add(const ArgumentsInfoType& info, ExecutorType* executor)
		{
			(*this)[info] = executor;
		}

		VariantType* call(const VariantTypeList& expressions) const
		{
			ENFORCE(expressions.size() == argumentsCount);
			ArgumentsInfoType info;
			info.init(expressions);
			auto it = this->find(info);
			ENFORCE(it != this->end());
			return (it != this->end()) ? it->second->call(expressions) : nullptr;
		}
	};

	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantName>
	struct FunctionsBucket<0, VariantType, VariantTypeList, TypeToName, VariantName>
	{
		typedef ArgumentsInfo<0, VariantType, VariantTypeList, TypeToName, VariantName> ArgumentsInfoType;
		typedef Executor<VariantType, VariantTypeList> ExecutorType;

		FunctionsBucket() :executor(0){}

		ExecutorType* executor;

		void add(const ArgumentsInfoType& info, ExecutorType* executor_)
		{
			executor = executor_;
		}

		VariantType* call(const VariantTypeList& expressions) const
		{
			return executor ? executor->call(expressions) : 0;
		}
	};

	template<class FunctionType>
	struct FunctionInstance
	{
		static FunctionType& impl()
		{
			static FunctionType* func = 0;
			if (!func)
			{
				static FunctionType sfunc;
				func = &sfunc;
				FunctionType::init();
			}

			return *func;
		}
	};

	
	
	
	template<class FunctionType, class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantName, class VariantConverter>
	class SimpleFunction : public FunctionInstance<FunctionType>, public FunctionsBucket<0, VariantType, VariantTypeList, TypeToName, VariantName>
	{
	public:
		typedef FunctionsBucket<0, VariantType, VariantTypeList, TypeToName, VariantName> Bucket;

		template<class CallEnvelope>
		static void add()
		{
			FunctionInstance<FunctionType>::impl().add(&CallEnvelope::call);
		}

		static VariantType* call(const VariantTypeList& expressions)
		{
			return FunctionInstance<FunctionType>::impl().Bucket::call(expressions);
		}

	protected:
		SimpleFunction(){};
						   
	private:
		template<class ReturnValue>
		static void add(ReturnValue(*func)())
		{
			static SimpleExecutorT<ReturnValue, VariantType, VariantTypeList, VariantCreator, VariantConverter> caller(func);
			ArgumentsInfo<0, VariantType, VariantTypeList, TypeToName, VariantName> info;
			info.init();
			FunctionInstance<FunctionType>::impl().Bucket::add(info, &caller);
		}
	};

	
	
	
	template<class FunctionType, class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantName, class VariantConverter>
	class UnaryFunction : public FunctionInstance<FunctionType>, public FunctionsBucket<1, VariantType, VariantTypeList, TypeToName, VariantName>
	{
	public:
		typedef FunctionsBucket<1, VariantType, VariantTypeList, TypeToName, VariantName> Bucket;

		template<class Arg, template <class> class CallEnvelope >
		static void add()
		{
			FunctionInstance<FunctionType>::impl().add( &CallEnvelope<Arg>::call );
		}

		template<class CallEnvelope>
		static void add()
		{
			FunctionInstance<FunctionType>::impl().add(&CallEnvelope::call);
		}

		static VariantType* call(const VariantTypeList& expressions)
		{
			return FunctionInstance<FunctionType>::impl().Bucket::call(expressions);
		}

	protected:
		UnaryFunction(){};

	private:
		template<class ReturnValue, class C>
		static void add( ReturnValue (*func)(C) )
		{
			static UnaryExecutorT<ReturnValue, C, VariantType, VariantTypeList, VariantCreator, VariantConverter> caller(func);
			ArgumentsInfo<1, VariantType, VariantTypeList, TypeToName, VariantName> info;
			info.template init<C>();
			FunctionInstance<FunctionType>::impl().Bucket::add(info, &caller);
		}
	};

	
	
	
	template<class FunctionType, class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantName, class VariantConverter>
	class BinaryFunction : public FunctionInstance<FunctionType>, public FunctionsBucket<2, VariantType, VariantTypeList, TypeToName, VariantName>
	{
	public:
		typedef FunctionsBucket<2, VariantType, VariantTypeList, TypeToName, VariantName> Bucket;

		template< class Arg0, class Arg1, template <class, class> class CallEnvelope >
		static void addSimple()
		{
			FunctionInstance<FunctionType>::impl().add( &CallEnvelope<Arg0, Arg1>::call );
		}

		template< class ArgT, template <class> class CallEnvelope >
		static void addSimple1()
		{
			FunctionInstance<FunctionType>::impl().add(&CallEnvelope<ArgT>::call);
		}

		template<class CallEnvelope>
		static void addSimple() { FunctionInstance<FunctionType>::impl().add(&CallEnvelope::call); }

		template< class Arg0, class Arg1, template <class, class> class CallEnvelope >
		static void addCommutative()
		{
			FunctionInstance<FunctionType>::impl().add( &CallEnvelope<Arg0, Arg1>::call );
			FunctionInstance<FunctionType>::impl().add( &CallEnvelope<Arg1, Arg0>::call );
		}

		static VariantType* call(const VariantTypeList& expressions)
		{
			return FunctionInstance<FunctionType>::impl().Bucket::call(expressions);
		}

	protected:
		BinaryFunction(){};

	private:
		template<class ReturnValue, class C0, class C1>
		static void add( ReturnValue (*func)(C0, C1) )
		{
			static BinaryExecutorT<ReturnValue, C0, C1, VariantType, VariantTypeList, VariantCreator, VariantConverter> caller(func);
			ArgumentsInfo<2, VariantType, VariantTypeList, TypeToName, VariantName> info;
			info.template init<C0, C1>();
			FunctionInstance<FunctionType>::impl().Bucket::add(info, &caller);
		}
	};

	
	
	
	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantConverter, template <class> class ValueFunctionCaller>
	struct FunctionsLib
	{					
		typedef VariantType* (*ResultFunction)(const VariantTypeList& params);

		struct Caller 
		{
			virtual ~Caller(){}
			virtual VariantType* call(const VariantTypeList& params) = 0;
		};

		struct SimpleCaller : public Caller
		{
			SimpleCaller(ResultFunction fnc) : f(fnc){}

			ResultFunction f;
			virtual VariantType* call(const VariantTypeList& params)
			{
				return f(params);
			}
		};	

		template<typename RetT, typename F, typename... T, std::size_t... I>
		static decltype(auto) conversion(F func, const VariantTypeList& params, std::index_sequence<I...>)
		{
			ENFORCE(params.size() == sizeof...(T));
			return ValueFunctionCaller<RetT>::call(func, VariantConverter::template convert<typename std::remove_const<typename std::remove_reference<T>::type>::type>(*params[I])...);
		}

		
		template<class RetT, class... P0>
		struct Envelope : public Caller
		{
			typedef RetT(*BindedFunction)(P0...);

			Envelope(BindedFunction fnc) :f(fnc){}

			BindedFunction f;
			virtual VariantType* call(const VariantTypeList& params)	
			{
				ENFORCE_EQUAL(params.size(), sizeof...(P0));
				return conversion<RetT, BindedFunction, P0...>(f, params, std::make_index_sequence<sizeof...(P0)>{});
			}
		};

		
		static Caller* func(const std::string& name) { return functions()[name].get();}

		static bool exist(const std::string& name)  { return functions().find(name) != functions().end(); }

		static void add(const std::string& name, Caller* caller) { functions()[name] = std::move(std::unique_ptr<Caller>(caller));	}

		static void add(const std::string& name, ResultFunction func) { add(name, new SimpleCaller(func));	}

		template<class RetT, class... P0> static void add(const std::string& name, RetT(*func)(P0...))	{ add(name, new Envelope<RetT, P0...>(func)); }

		static VariantType* execute(const std::string& name, const VariantTypeList& params)
		{
			try
			{
				if (auto function = func(name))
				{
					ENFORCE(function);
					return function->call(params);
				}
				else
				{
					LOG_ERROR("variant function: " << name << " not found");
				}
			}
			catch (std::exception& e)
			{
				LOG_ERROR(e);
			}

			return 0;
		}

		static auto& functions()	
		{ 
			static std::unordered_map<std::string, std::unique_ptr<Caller>> result;	
			return result; 
		}
	};

	template<class VariantType, class VariantTypeList, template <class> class TypeToName, class VariantCreator, class VariantName, class VariantConverter, template <class> class ValueFunctionCaller>
	struct Types
	{
		template<class FunctionType> struct Simple	: public SimpleFunction<FunctionType, VariantType, VariantTypeList, TypeToName, VariantCreator, VariantName, VariantConverter>	{};
		template<class FunctionType> struct Unary	: public UnaryFunction<FunctionType, VariantType, VariantTypeList, TypeToName, VariantCreator, VariantName, VariantConverter>	{};
		template<class FunctionType> struct Binary	: public BinaryFunction<FunctionType, VariantType, VariantTypeList, TypeToName, VariantCreator, VariantName, VariantConverter>	{};

		struct Functions : public FunctionsLib<VariantType, VariantTypeList, TypeToName, VariantCreator, VariantConverter, ValueFunctionCaller>{};
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