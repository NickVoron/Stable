// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once
#include <exception>
#include <sstream>
#include <mutex>
#include <array>
#include <stdio.h> 

#include "common/stringize.h"
#include "common/preprocessor_helpers.h"
#include "containers/circular_buffer.h"

namespace std
{
	ostream& operator<<(ostream& s, const exception& error);
}

namespace logs
{
	struct repeat
	{
		inline repeat(char s, std::size_t c) :count(c), str(s) {}
		std::size_t count;
		char str;
	};

	struct tabs : public repeat { inline tabs(std::size_t c) : repeat('\t', c) {} };
	struct spaces : public repeat { inline spaces(std::size_t c) : repeat(' ', c) {} };

	std::ostream& operator <<(std::ostream& s, const repeat& t);

	namespace impl
	{
		struct logstream : public std::stringstream { };

		template<class T>
		logstream& operator<<(logstream& ls, const T& value)
		{
			auto& ss = static_cast<std::stringstream&>(ls);

			if (ls.gcount() > 0)
			{
				ss << " ";
			}

			ss << value;

			return ls;
		}

		enum class Severity : uint8_t
		{
			LOG_SEVERITY_NONE,
			LOG_SEVERITY_ERROR,
			LOG_SEVERITY_WARNING,
			LOG_SEVERITY_INFO,
			LOG_SEVERITY_VERBOSE,

			LOG_SEVERITY_ALL,
		};

		const char* severity_str(Severity severity);

		struct EntryInformation
		{
			std::string severity;
			std::string source_location;
		};

		struct Flusher
		{
			void flush(const EntryInformation& information, const char* entry);
			std::vector<std::function<void(const char*)>> handlers;
			std::vector<std::function<std::string(const EntryInformation& information, const std::string&)>> filters;
		};

		extern std::array<Flusher, static_cast<uint8_t>(Severity::LOG_SEVERITY_ALL)> flushers;

		struct BaseLogger
		{
			static Severity global_severity;
		};

		template<Severity severity>
		class Logger : BaseLogger
		{
		public:
			static_assert(severity < Severity::LOG_SEVERITY_ALL);

			Logger(const EntryInformation& info_) : info(info_) {}
			~Logger()
			{
				if (severity <= global_severity)
				{
					flushers[static_cast<uint8_t>(severity)].flush(info, os.str().c_str());
				}
			}

			template<class T>
			Logger& operator<<(const T& obj)
			{
				if (severity <= global_severity)
				{
					operator<<(str::spaced(obj).c_str());
				}

				return *this;
			}

			Logger& operator<<(const char* entry)
			{
				if (severity <= global_severity)
				{
					os << entry;
				}

				return *this;
			}

		private:
			EntryInformation info;
			logstream os;
		};

		template<Severity severity, class F>	void add_severity_handler(F&& handler)	{ flushers[static_cast<uint8_t>(severity)].handlers.emplace_back(std::forward<F>(handler)); }
		template<Severity severity, class F>	void add_severity_filter(F&& filter)	{ flushers[static_cast<uint8_t>(severity)].filters.emplace_back(std::forward<F>(filter)); }
	}

	void severity_none();
	void severity_error();
	void severity_warning();
	void severity_info();
	void severity_all();

	template<class F> void add_error_severity_handler(F&& handler)		{ impl::add_severity_handler<impl::Severity::LOG_SEVERITY_ERROR>(std::forward<F>(handler)); }
	template<class F> void add_warning_severity_handler(F&& handler)	{ impl::add_severity_handler<impl::Severity::LOG_SEVERITY_WARNING>(std::forward<F>(handler)); }
	template<class F> void add_info_severity_handler(F&& handler)		{ impl::add_severity_handler<impl::Severity::LOG_SEVERITY_INFO>(std::forward<F>(handler)); }
	template<class F> void add_verbose_severity_handler(F&& handler)	{ impl::add_severity_handler<impl::Severity::LOG_SEVERITY_VERBOSE>(std::forward<F>(handler)); }

	template<class F> void add_error_severity_filter(F&& filter) { impl::add_severity_filter<impl::Severity::LOG_SEVERITY_ERROR>(std::forward<F>(filter)); }
	template<class F> void add_warning_severity_filter(F&& filter) { impl::add_severity_filter<impl::Severity::LOG_SEVERITY_WARNING>(std::forward<F>(filter)); }
	template<class F> void add_info_severity_filter(F&& filter) { impl::add_severity_filter<impl::Severity::LOG_SEVERITY_INFO>(std::forward<F>(filter)); }
	template<class F> void add_verbose_severity_filter(F&& filter) { impl::add_severity_filter<impl::Severity::LOG_SEVERITY_VERBOSE>(std::forward<F>(filter)); }


	template<class F> void add_handler(F&& handler)
	{
		add_error_severity_handler(std::forward<F>(handler));
		add_warning_severity_handler(std::forward<F>(handler));
		add_info_severity_handler(std::forward<F>(handler));
		add_verbose_severity_handler(std::forward<F>(handler));
	}

	template<class F> void add_filter(F&& filter)
	{
		add_error_severity_filter(std::forward<F>(filter));
		add_warning_severity_filter(std::forward<F>(filter));
		add_info_severity_filter(std::forward<F>(filter));
		add_verbose_severity_filter(std::forward<F>(filter));
	}

	void default_color();

#define OUTPUT_TO_LOG(SEVERITY_TAG, messages) do { if(SEVERITY_TAG <= ::logs::impl::BaseLogger::global_severity) { ::logs::impl::Logger<SEVERITY_TAG> logger({ severity_str(SEVERITY_TAG), SOURCE_LOCATION}); logger << messages << "\n"; } } while(false);

#define LOG_ERROR(messages)		OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_ERROR, messages);
#define LOG_MSG(messages)		OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_INFO, messages);
#define LOG_INFO(messages)		OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_INFO, messages);
#define LOG_VERBOSE(messages)	OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_VERBOSE, messages);
#define LOG_WARNING(messages)	OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_WARNING, messages);
#define LOG_REPORT(messages)	OUTPUT_TO_LOG(::logs::impl::Severity::LOG_SEVERITY_INFO, messages);

#define LOG_FUNCTION_NAME		LOG_MSG		(__FUNCTION__);
#define REPORT_FUNCTION_NAME	LOG_REPORT	(__FUNCTION__);

#define TLOG_ERROR(messages)	LOG_ERROR("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);
#define TLOG_MSG(messages)		LOG_MSG	("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);
#define TLOG_WARNING(messages)  LOG_WARNING("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);

#define TLOG_EXPRESSION(...)	do { TLOG_MSG(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);
#define LOG_EXPRESSION(...)		do { LOG_MSG(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);
#define ERROR_EXPRESSION(...)	do { LOG_ERROR(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);


}



// Copyright (C) 2017-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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