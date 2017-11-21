#pragma once

#ifdef WIN32
#include <excpt.h>
#endif

#ifdef WIN32
inline int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
	return EXCEPTION_EXECUTE_HANDLER;
}
#endif

template<class Function, class... Params>
void try_execute_cpp(Function&& function, Params&&... p)
{
	try
	{
		function(std::forward<Params>(p)...);
	}
	catch (std::exception& e)
	{
		LOG_ERROR(e);
	}
	catch (...)
	{
		LOG_ERROR("unknown C++ exception");
	}
}

template<class Function, class... Params>
void try_execute_seh(Function&& function, Params&&... p)
{
#ifdef WIN32
	__try
	{
#endif 
		//function(std::forward<Params>(p)...);
		try_execute_cpp(std::forward<Function>(function), std::forward<Params>(p)...);
#ifdef WIN32
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation()))
	{
		#include "exception_codes.h"
		unsigned long code = GetExceptionCode();
		
		for (int i = 0; i < std::size(error_codes); ++i)
		{
			if (error_codes[i] == code)
			{
				std::cout << error_messages[i];
				break;
			}
		}
	}
#endif 
}

template<class Function, class... Params>
void try_execute(Function&& function, Params&&... p)
{
	try_execute_seh(std::forward<Function>(function), std::forward<Params>(p)...);
}