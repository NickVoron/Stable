#pragma once

#include "Loki/library.include.h"


template< template <class> class BaseCaller >
class TemplateCallDispatcher
{
public:
	template<class... UserCaller> 
	static void addInst() 
	{ 
		auto caller = { ((ptrRef<typename UserCaller::UserStruct>() = holder<UserCaller>()), 0)... };
	}

	template<class UserStruct>	static BaseCaller<UserStruct>&	ref()	{ ENFORCE(ptr<UserStruct>()); return *ptr<UserStruct>(); }
	template<class UserStruct>	static BaseCaller<UserStruct>*	ptr()	{ return ptrRef<UserStruct>();	}

private:
	template<class UserStruct>	static BaseCaller<UserStruct>*& ptrRef() {  static BaseCaller<UserStruct>* conv = 0; return conv;	}
	template<class UserCaller> static BaseCaller<typename UserCaller::UserStruct>* holder() 
	{ 
		static_assert( (std::is_base_of<BaseCaller<typename UserCaller::UserStruct>, UserCaller>::value), "Caller must be inherited from BaseCaller" ); 
		static UserCaller caller; 
		return &caller; 
	}
};