// Copyright (C) 2017-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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
	namespace impl
	{ 
		Severity BaseLogger::global_severity = Severity::LOG_SEVERITY_ALL;

		std::array<Flusher, static_cast<uint8_t>(Severity::LOG_SEVERITY_ALL)> flushers;

		const char* severity_str(Severity severity)
		{			
			static const char* strs[] = { "NONE", "ERROR", "WARNING", "INFO", "VERBOSE", "ALL" };
			return strs[static_cast<uint8_t>(Severity::LOG_SEVERITY_ALL)];
		}

		void Flusher::flush(const EntryInformation& information, const char* entry)
		{
			std::string result = entry;
			for (auto& filter : filters)
			{
				result = filter(information, result);
			}

			for (auto& handler : handlers)
			{
				handler(result.c_str());
			}
		}

		namespace
		{			   							  
			std::string decoratedName(const char* name)
			{
				return Base::FileUtils::getModuleDir() + name;
			}
		}

		std::recursive_mutex cs; 

#if defined(__ANDROID__) || defined(__APPLE__)
		circular_buffer<std::string, 15> log_accum;
#endif
	}


#if defined(__ANDROID__) || defined(__APPLE__)
	std::string accumulated_log()
	{
        std::string result;
        






		return result;
	}

	void accumulate_log(const std::string& value)
	{


	}

	void clear_log()
	{
#ifdef USE_WINDOWS
		::logs::implementation::log_accum = decltype(::logs::implementation::log_accum)();
#endif
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

	void default_color()
	{
		Base::Console::color(false);
	}

	void severity_none()	{ impl::BaseLogger::global_severity = impl::Severity::LOG_SEVERITY_NONE; }
	void severity_error()	{ impl::BaseLogger::global_severity = impl::Severity::LOG_SEVERITY_ERROR; }
	void severity_warning() { impl::BaseLogger::global_severity = impl::Severity::LOG_SEVERITY_WARNING; }
	void severity_info()	{ impl::BaseLogger::global_severity = impl::Severity::LOG_SEVERITY_INFO; }
	void severity_all()		{ impl::BaseLogger::global_severity = impl::Severity::LOG_SEVERITY_ALL; }


	const char* endl = "\n";
}




// Copyright (C) 2017-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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