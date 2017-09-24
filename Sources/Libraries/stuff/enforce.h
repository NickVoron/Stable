#pragma once

#include "common/preprocessor_helpers.h"

#include "stringUtils.h"
#ifdef WIN32
#include <debugapi.h>
#endif
#define ENFORCEMENT_INLINE inline

namespace Base
{
	inline void dbgbreak()
	{
#ifdef WIN32
		if (IsDebuggerPresent())
		{
			__debugbreak();
		}
#endif
	}

	struct DefaultPredicate	{ template <class T> ENFORCEMENT_INLINE static bool Wrong(const T& obj) { return !obj; } };

	struct DefaultRaiser
	{
		template <class T>
		ENFORCEMENT_INLINE static void Throw(const T&, const char* locus)
		{
			dbgbreak();
			throw std::runtime_error(locus);
		}
	};

	template<typename Ref, typename P, typename R>
	class Enforcer
	{
	public:
		ENFORCEMENT_INLINE Enforcer(Ref t, const char* locus) : t_(t), locus_(P::Wrong(t) ? locus : 0) { }

		ENFORCEMENT_INLINE Ref operator*() const
		{
			if (locus_) 
				R::Throw(t_, locus_);
			return t_;
		}

	private:
		Ref t_;
		const char* const locus_;
	};

	template <class P, class R, typename T> ENFORCEMENT_INLINE Enforcer<const T&, P, R> MakeEnforcer(const T& t, const char* locus)	{ return Enforcer<const T&, P, R>(t, locus); }
	template <class P, class R, typename T> ENFORCEMENT_INLINE Enforcer<T&, P, R> MakeEnforcer(T& t, const char* locus)	{ return Enforcer<T&, P, R>(t, locus);	}
}

#define ENFORCE(exp)			*Base::MakeEnforcer<Base::DefaultPredicate, Base::DefaultRaiser>( (exp), SOURCE_LOCATION_STR(exp));
#define ENFORCE_MSG(exp, msg)	*Base::MakeEnforcer<Base::DefaultPredicate, Base::DefaultRaiser>( (exp), str::stringize(SOURCE_LOCATION_STR(exp)" : ", msg).c_str());
#define ENFORCE_EQUAL(exp0, exp1)		ENFORCE_MSG(exp0 == exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_NOT_EQUAL(exp0, exp1)	ENFORCE_MSG(exp0 != exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_LESS(exp0, exp1)		ENFORCE_MSG(exp0 <	exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_GREATER(exp0, exp1)		ENFORCE_MSG(exp0 >	exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_GEQUAL(exp0, exp1)		ENFORCE_MSG(exp0 >= exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_LEQUAL(exp0, exp1)		ENFORCE_MSG(exp0 <= exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_POINTER(exp)			ENFORCE_MSG(exp, str::stringize(STPP_STRINGIZE_VALUES(exp), " is null").c_str());

#define VERIFY(x, y)	{ if (! (x) )	{ Base::dbgbreak(); throw Base::Errors::Simple(str::stringize(__FUNCTION__, " verification failed: ", y) );} }
#define THROW(y)		{ Base::dbgbreak(); throw Base::Errors::Simple(str::stringize(__FUNCTION__, " assertion: ", y)); }

#ifdef WIN32
#define INCOMPLETE
//#define INCOMPLETE	 	__pragma(message(SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION) " function is INCOMPLETE"));	 char buff[512] = {'\0'}; sprintf(buff, "%s : %s", SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION), " INCOMPLETE"); throw Base::Errors::Simple(buff);
#else
#define INCOMPLETE	 
#endif
#define STATIC_INCOMPLETE	 { static_assert(false, __FUNCTION__" not implemented"); }

#define COMPILER_MESSAGE(MESSAGE) __pragma(message(SOURCE_LOCATION FUNCTION_LOCATION STPP_STRINGIZE(MESSAGE)));
#define COMPILER_WARNING_LOCATION(MESSAGE) __pragma(message(SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION) STPP_STRINGIZE(MESSAGE)));