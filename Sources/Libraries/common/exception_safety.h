// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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