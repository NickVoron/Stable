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
		ENFORCEMENT_INLINE static void Throw(std::string locus)
		{
			dbgbreak();
			throw std::runtime_error(locus);
		}
	};

	struct DefaultBreaker
	{
		ENFORCEMENT_INLINE static void Throw(std::string locus)
		{
			dbgbreak();
		}
	};
}

#define ENFORCE(exp) do { if(Base::DefaultPredicate::Wrong((exp))) Base::DefaultRaiser::Throw(SOURCE_LOCATION_STR(exp)); } while(false);
#define ENFORCE_MSG(exp, msg) do { if(Base::DefaultPredicate::Wrong((exp))) Base::DefaultRaiser::Throw(str::stringize(SOURCE_LOCATION_STR(exp)" : ", msg).str()); } while(false);
#define ENFORCE_EQUAL(exp0, exp1)		ENFORCE_MSG((exp0) == (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_NOT_EQUAL(exp0, exp1)	ENFORCE_MSG((exp0) != (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_LESS(exp0, exp1)		ENFORCE_MSG((exp0) <  (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_GREATER(exp0, exp1)		ENFORCE_MSG((exp0) >  (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_GEQUAL(exp0, exp1)		ENFORCE_MSG((exp0) >= (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_LEQUAL(exp0, exp1)		ENFORCE_MSG((exp0) <= (exp1), STPP_STRINGIZE_VALUES(exp0, exp1));
#define ENFORCE_POINTER(exp)			ENFORCE_MSG(exp, str::stringize(STPP_STRINGIZE_VALUES(exp), " is null").c_str());
#define ENFORCE_NULL(exp)				ENFORCE_MSG(!exp, str::stringize(STPP_STRINGIZE_VALUES(exp), " is not null").c_str());

#define BREAKPOINT(exp) do { if(!Base::DefaultPredicate::Wrong((exp))) Base::DefaultBreaker::Throw(SOURCE_LOCATION_STR(exp)); } while(false);
#define BREAKPOINT_MSG(exp, msg) do { if(!Base::DefaultPredicate::Wrong((exp))) Base::DefaultBreaker::Throw(str::stringize(SOURCE_LOCATION_STR(exp)" : ", msg).str()); } while(false);
#define BREAKPOINT_EQUAL(exp0, exp1)		BREAKPOINT_MSG(exp0 == exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_NOT_EQUAL(exp0, exp1)	BREAKPOINT_MSG(exp0 != exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_LESS(exp0, exp1)		BREAKPOINT_MSG(exp0 <	exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_GREATER(exp0, exp1)		BREAKPOINT_MSG(exp0 >	exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_GEQUAL(exp0, exp1)		BREAKPOINT_MSG(exp0 >= exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_LEQUAL(exp0, exp1)		BREAKPOINT_MSG(exp0 <= exp1, STPP_STRINGIZE_VALUES(exp0, exp1));
#define BREAKPOINT_POINTER(exp)			BREAKPOINT_MSG(exp, str::stringize(STPP_STRINGIZE_VALUES(exp), " is null").c_str());
#define BREAKPOINT_NULL(exp)				BREAKPOINT_MSG(!exp, str::stringize(STPP_STRINGIZE_VALUES(exp), " is not null").c_str());


#define VERIFY(x, y)	{ if (! (x) )	{ Base::dbgbreak(); throw Base::Errors::Simple(str::stringize(__FUNCTION__, " verification failed: ", y) );} }
#define THROW(y)		{ Base::dbgbreak(); throw Base::Errors::Simple(str::stringize(__FUNCTION__, " assertion: ", y)); }

#ifdef WIN32
//#define INCOMPLETE
#define INCOMPLETE	 	__pragma(message(SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION) " function is INCOMPLETE"));	 char buff[512] = {'\0'}; sprintf(buff, "%s : %s", SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION), " INCOMPLETE"); throw Base::Errors::Simple(buff);
#else
#define INCOMPLETE	 
#endif
#define STATIC_INCOMPLETE	 { static_assert(false, __FUNCTION__" not implemented"); }

#define COMPILER_MESSAGE(MESSAGE) __pragma(message(SOURCE_LOCATION FUNCTION_LOCATION STPP_STRINGIZE(MESSAGE)));
#define COMPILER_WARNING_LOCATION(MESSAGE) __pragma(message(SOURCE_LOCATION COMPILER_WARNING(FUNCTION_LOCATION) STPP_STRINGIZE(MESSAGE)));