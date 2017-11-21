#pragma once

#include "nonCopyable.h"

namespace Base
{
	template<class T>
	class MeyersSingleton : public Base::NonCopyable
	{
	public:
		static T& get() { static T t; return t; }

	protected:
		MeyersSingleton()
		{
			static bool inited = false;
			if(inited) throw std::runtime_error("singleton must be inited at once");
			inited = true;
		}
	};

	template<class T>
	class ExplicitMeyersSingleton : public Base::NonCopyable
	{
	public:
		static void create()
		{
			T*& p = ptr();
			if(!p) p = new T();
		}

		static void destroy()
		{
			T*& p = ptr();
			delete p;
			p = 0;
		}

		static T& get() { return *ptr(); }

	protected:
		ExplicitMeyersSingleton(){}

	private:
		
		static T*& ptr(){ static T* p = 0; return p; } 
	};
}