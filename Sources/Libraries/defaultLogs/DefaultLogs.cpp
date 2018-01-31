// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "defaultLogs.h"
#include "Console/library.include.h"
#include "stuff/library.include.h"

#include <fstream>
#include <thread>
#include <mutex>

#pragma init_seg(lib)

namespace std
{
	::std::ostream& operator<<(::std::ostream& s, const ::std::exception& error)
	{
		std::string value = error.what();
		return s << value;
	}
}


namespace logs
{
	namespace implementation 
	{ 
		float defaultPopTime = 10.0f;
		
		Log obj_log;
		Err obj_err;
		Info obj_info;
		Warning obj_warn;
		LogQueue obj_lq;
		TimedLogQueue obj_tlq(defaultPopTime);

		namespace
		{			   							  
			std::string decoratedName(const char* name)
			{
				return Base::FileUtils::getModuleDir() + name;
			}
		}

		struct Initializer 
		{
			Initializer()
			{
				LOG_SYSTEM_THREAD_SAFE;

				auto errfile = decoratedName("_err.txt");
				auto logfile = decoratedName("_log.txt");

				obj_log.add(logfile).add(std::cout);
				obj_info.add(logfile).add(std::cout);
				obj_warn.add(logfile).add(std::cerr);
				obj_err.add(errfile).add(std::cerr);
			}
		};
	
		std::recursive_mutex cs; 

#if defined(__ANDROID__) || defined(__APPLE__)
		circular_buffer<std::string, 15> log_accum;
#endif

		void init() { static Initializer initializer; }
	}


#if defined(__ANDROID__) || defined(__APPLE__)
	std::string accumulated_log()
	{
		LOG_SYSTEM_THREAD_SAFE;

		std::string result;

		for (unsigned int i = implementation::log_accum.begin_index(); i < implementation::log_accum.end_index(); ++i)
		{
			result += implementation::log_accum[i];
		}

		return result;
	}

	void accumulate_log(const std::string& value)
	{
		LOG_SYSTEM_THREAD_SAFE;
		::logs::implementation::log_accum.step() = value;
	}

	void clear_log()
	{
		::logs::implementation::log_accum = decltype(::logs::implementation::log_accum)();
	}
#endif
	
	
	
	std::ostream& operator <<(std::ostream& s, const repeat& t)
	{
		static const std::size_t MAX_REPEAT_COUNT = 256;
		static char buffer[MAX_REPEAT_COUNT];		
		std::size_t count = std::min(t.count, MAX_REPEAT_COUNT - 1);
		std::fill(buffer, buffer + count, t.str);		
		buffer[count] = 0;
		return s << buffer;
	}

	
	void update(float dt)
	{
		LOG_SYSTEM_THREAD_SAFE;
		((implementation::TimedLogQueue&)timeReport()).update(dt);
	}
	
	void default_color()
	{
		Base::Console::color(false);
	}

	implementation::Log&		log()			{ implementation::init();  Base::Console::color(false);																							return implementation::obj_log; }
	implementation::Err&		err()			{ implementation::init();  Base::Console::color(false, Base::Console::COLOR_RED | Base::Console::COLOR_INTENSITY	);								return implementation::obj_err;	}
	implementation::Info&		info()			{ implementation::init();  Base::Console::color(false, Base::Console::COLOR_RED | Base::Console::COLOR_GREEN		);								return implementation::obj_info;	}
	implementation::Warning&	warning()		{ implementation::init();  Base::Console::color(false, Base::Console::COLOR_RED | Base::Console::COLOR_BLUE | Base::Console::COLOR_INTENSITY	);	return implementation::obj_warn;	}
	implementation::LogQueue&	report()		{ implementation::init();  																														return implementation::obj_lq;	}
	implementation::LogQueue&	timeReport()	{ implementation::init();  																														return implementation::obj_tlq;	}

	const char* endl = "\n";
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