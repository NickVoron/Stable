#pragma once

#include "stuff/enforce.h"
#include "memory/library.include.h"

namespace multimethods2
{
	struct BaseClient 
	{
		virtual ~BaseClient(){};
		virtual int index() const = 0;
	};

	template<class ClientClass, class Parent = BaseClient>
	struct ClientT : public Parent
	{
		typedef ClientT ParentType;

		ClientT(){}

		template<class T> ClientT(T& t):Parent(t){}

		virtual int index() const { return ClassIndex(); }
		static int& ClassIndex() { static int id = -1; return id; }
	};


	//
	//
	//
	class Table2Simple
	{
	public:
		virtual void execute(BaseClient& c0, BaseClient& c1) = 0;
		virtual void execute(BaseClient* c0, BaseClient* c1) = 0;
		virtual void execute(BaseClient& c0, BaseClient* c1) = 0;
		virtual void execute(BaseClient* c0, BaseClient& c1) = 0;
	};

	template<class ReturnValue>
	class Table2T : public Table2Simple
	{
	public:
		virtual void execute(BaseClient& c0, BaseClient& c1) { exec(c0, c1); }
		virtual void execute(BaseClient* c0, BaseClient* c1) { exec(c0, c1); }
		virtual void execute(BaseClient& c0, BaseClient* c1) { exec(c0, c1); }
		virtual void execute(BaseClient* c0, BaseClient& c1) { exec(c0, c1); }

		inline ReturnValue exec(BaseClient* c0, BaseClient* c1) { return exec(*c0, *c1);	}
		inline ReturnValue exec(BaseClient& c0, BaseClient* c1) { return exec(c0, *c1);		}
		inline ReturnValue exec(BaseClient* c0, BaseClient& c1) { return exec(*c0, c1);		}
														   
		inline ReturnValue exec(BaseClient& c0, BaseClient& c1)
		{
			const int i0 = c0.index();
			const int i1 = c1.index();
			
			return (i0 <= i1) ? impl(i0, i1)->execute(c0, c1) : impl(i1, i0)->execute(c1, c0);
		}

		inline bool exists(BaseClient& c0, BaseClient& c1)
		{
			const int i0 = c0.index();
			const int i1 = c1.index();

			return (i0 >= 0) && (i1 >= 0);
		}

	protected:
		struct Caller { virtual ReturnValue execute(BaseClient& c0, BaseClient& c1) = 0; };

		virtual Caller* impl(std::size_t i0, std::size_t i1) = 0;
	};

	template<class ReturnValue, class C0, class C1, class BaseCallerType>
	struct CallerT : public BaseCallerType
	{
		typedef ReturnValue(*Func)(C0&, C1&);

		CallerT(Func f) :func(f) {}
		virtual ReturnValue execute(BaseClient& c0, BaseClient& c1) { return func((C0&) c0, (C1&) c1); }

		Func func;
	};

	template<class ReturnValue>
	class Table2 : public Table2T<ReturnValue>
	{
	public:
		template<class ReturnValue, class C0, class C1>
		void add( ReturnValue (*func)(C0&, C1&))
		{
			static CallerT<ReturnValue, C0, C1, Table2T<ReturnValue>::Caller> caller(func);
			add<C0, C1>(&caller);
		}

	protected:
		template<class C0, class C1>
		void add(Table2T<ReturnValue>::Caller* impl)
		{
			int& i0 = C0::ClassIndex();
			int& i1 = C1::ClassIndex();

			if(i0 < 0) i0 = (int) cursor++;
			if(i1 < 0) i1 = (int) cursor++;

			ENFORCE_LESS(cursor, size);
								 
			table.set(i0, i1, impl);
		}

		template<std::size_t size>
		struct TableHolder
		{
			static const std::size_t realSize = ((size*size)+size) >> 1;

			Table2T<ReturnValue>::Caller* callers[realSize];

			TableHolder(){mem::memzero(callers);}
			
			Table2T<ReturnValue>::Caller*& operator()(std::size_t x, std::size_t y)
			{
				std::size_t i = idx(x, y);
				ENFORCE_LESS(i, realSize);
				return callers[i];
			}

			void set(std::size_t x, std::size_t y, Table2T<ReturnValue>::Caller* c)
			{
				ENFORCE_POINTER(c);

				auto& current = operator()(x, y);
				ENFORCE_EQUAL(current, 0);
				current = c;
			}

			static std::size_t idx(std::size_t x, std::size_t y)
			{
				return ( x - y ) + (y * (size * 2 - y + 1)) / 2;
			}
		};

		virtual Table2T<ReturnValue>::Caller* impl(std::size_t i0, std::size_t i1) override
		{
			return table(i0, i1); 
		}

		static const std::size_t size = 256;
		TableHolder<size> table;
		std::size_t cursor = 0;
	};
}