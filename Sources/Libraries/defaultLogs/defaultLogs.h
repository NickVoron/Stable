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
#include <stdio.h> 

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include "reports/log.h"
#include "reports/logQueue.h"
#include "reports/timedLogQueue.h"

#include "common/preprocessor_helpers.h"
#include "containers/circular_buffer.h"

namespace std
{
	::std::ostream& operator<<(::std::ostream& s, const ::std::exception& error);
}

namespace logs
{
	namespace implementation 
	{
		extern std::recursive_mutex cs; 
	}
	#define LOG_SYSTEM_THREAD_SAFE std::lock_guard<std::recursive_mutex> logs_mutex_lock(::logs::implementation::cs);
	
	void update(float dt);
	
	struct repeat
	{
		inline repeat(char s, std::size_t c):count(c), str(s){}
		std::size_t count;
		char str;
	};

	struct tabs		: public repeat { inline tabs(std::size_t c) : repeat('\t', c){} };
	struct spaces	: public repeat { inline spaces(std::size_t c) : repeat(' ', c){} };

	std::ostream& operator <<(std::ostream& s, const repeat& t);
	
	implementation::Log& log();
	implementation::Err& err();
	implementation::Info& info();
	implementation::Warning& warning();
	implementation::LogQueue& report(); 
	implementation::LogQueue& timeReport();

	extern const char* endl;

	void default_color();

	struct logstream : public std::stringstream {};

	template<class T>
	logstream& operator<<(logstream& ls, const T& value)
	{
		((std::stringstream&)ls) << value << " ";
		return ls;
	}

#if defined(USE_ANDROID) || defined(USE_APPLE) || defined(USE_LINUX)

	std::string accumulated_log();
	void accumulate_log(const std::string& value);
	void clear_log();
#if defined(USE_ANDROID)
#define LOGE(message)  do { __android_log_write(ANDROID_LOG_ERROR, "SharedTec", message); } while(false);
#define LOGD(message)  do { __android_log_write(ANDROID_LOG_DEBUG, "SharedTec", message); } while(false);
#define LOGV(message)  do { __android_log_write(ANDROID_LOG_VERBOSE, "SharedTec", message); } while(false);
#define LOGW(message)  do { __android_log_write(ANDROID_LOG_WARN, "SharedTec", message); } while(false);
#define LOGI(message)  do { __android_log_write(ANDROID_LOG_INFO, "SharedTec", message); } while(false);
#else
#define LOGE(message)  do { } while(false);
#define LOGD(message)  do { } while(false);
#define LOGV(message)  do { } while(false);
#define LOGW(message)  do { } while(false);
#define LOGI(message)  do { } while(false);
#endif

#define LOG_ERROR(messages)	 	do { LOG_SYSTEM_THREAD_SAFE; ::logs::logstream os; os << messages << logs::endl; std::string res = os.str(); LOGE(res.c_str());	::logs::accumulate_log(res); } while(false);
#define LOG_MSG(messages)		do { LOG_SYSTEM_THREAD_SAFE; ::logs::logstream os; os << messages << logs::endl; std::string res = os.str(); LOGD(res.c_str());	::logs::accumulate_log(res); } while(false);
#define LOG_INFO(messages)		do { LOG_SYSTEM_THREAD_SAFE; ::logs::logstream os; os << messages << logs::endl; std::string res = os.str(); LOGI(res.c_str());	::logs::accumulate_log(res); } while(false);
#define LOG_WARNING(messages)	do { LOG_SYSTEM_THREAD_SAFE; ::logs::logstream os; os << messages << logs::endl; std::string res = os.str(); LOGW(res.c_str());	::logs::accumulate_log(res); } while(false);
#define LOG_REPORT(messages)	do { LOG_SYSTEM_THREAD_SAFE; ::logs::logstream os; os << messages << logs::endl; std::string res = os.str(); ::logs::report() << res;	} while(false);

#define LOG_ERROR_UNDECORATED(messages)		LOG_ERROR(messages);
#define LOG_MSG_UNDECORATED(messages)		LOG_MSG(messages);
#define LOG_REPORT_UNDECORATED(messages)	LOG_REPORT(messages);
#define LOG_INFO_UNDECORATED(messages)		LOG_INFO(messages);
#define LOG_WARNING_UNDECORATED(messages)	LOG_WARNING(messages);

#else

#define OUTPUT_TO_LOG(LogType, messages, dbgOut)				{	LOG_SYSTEM_THREAD_SAFE; (LogType) << messages << logs::endl; logs::default_color(); if(dbgOut) { ::logs::logstream os; os << SOURCE_LOCATION << messages << logs::endl; OutputDebugStringA(os.str().c_str());}};
#define OUTPUT_TO_LOG_UNDECORATED(LogType, messages, dbgOut)	{	LOG_SYSTEM_THREAD_SAFE; (LogType) << messages << logs::endl; logs::default_color(); if(dbgOut) { ::logs::logstream os; os << messages << logs::endl; OutputDebugStringA(os.str().c_str());}};

#define LOG_ERROR(messages)		OUTPUT_TO_LOG(logs::err(),		messages, true);
#define LOG_MSG(messages)		OUTPUT_TO_LOG(logs::log(),		messages, true);
#define LOG_REPORT(messages)	OUTPUT_TO_LOG(logs::report(),	messages, false);
#define LOG_INFO(messages)		OUTPUT_TO_LOG(logs::info(),		messages, true);
#define LOG_WARNING(messages)	OUTPUT_TO_LOG(logs::warning(),	messages, true);

#define LOG_ERROR_UNDECORATED(messages)		OUTPUT_TO_LOG_UNDECORATED(logs::err(),		messages, true);
#define LOG_MSG_UNDECORATED(messages)		OUTPUT_TO_LOG_UNDECORATED(logs::log(),		messages, true);
#define LOG_REPORT_UNDECORATED(messages)	OUTPUT_TO_LOG_UNDECORATED(logs::report(),	messages, false);
#define LOG_INFO_UNDECORATED(messages)		OUTPUT_TO_LOG_UNDECORATED(logs::info(),		messages, true);
#define LOG_WARNING_UNDECORATED(messages)	OUTPUT_TO_LOG_UNDECORATED(logs::warning(),	messages, true);


#define LOG_FUNCTION_NAME		LOG_MSG		(__FUNCTION__);
#define REPORT_FUNCTION_NAME	LOG_REPORT	(__FUNCTION__);

#define LOG_ERROR_ENDL		OUTPUT_TO_LOG(logs::err(),		logs::endl, true);
#define LOG_MSG_ENDL		OUTPUT_TO_LOG(logs::log(),		logs::endl, true);
#define LOG_REPORT_ENDL		OUTPUT_TO_LOG(logs::report(),	logs::endl, false);
#define LOG_INFO_ENDL		OUTPUT_TO_LOG(logs::info(),		logs::endl, true);
#define LOG_WARNING_ENDL	OUTPUT_TO_LOG(logs::warning(),	logs::endl, true);

#define LOG_ERROR_EX(messages)	 LOG_ERROR	("function: " << __FUNCTION__ << " line: " << __LINE__ << " " << messages);
#define LOG_MSG_EX(messages)	 LOG_MSG	("function: " << __FUNCTION__ << " line: " << __LINE__ << " " << messages);
#define LOG_WARNING_EX(messages) LOG_WARNING("function: " << __FUNCTION__ << " line: " << __LINE__ << " " << messages);

#define TLOG_ERROR(messages)	LOG_ERROR("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);
#define TLOG_MSG(messages)		LOG_MSG("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);
#define TLOG_WARNING(messages)  LOG_WARNING("pid: " << GetCurrentProcessId() << " tid: " << std::this_thread::get_id() << " : " << messages);

#endif

#define LOG_REPORT_CLEAR	{ LOG_SYSTEM_THREAD_SAFE; logs::report().clear(); };

#define TLOG_EXPRESSION(...) do { TLOG_MSG(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);
#define LOG_EXPRESSION(...) do { LOG_MSG(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);
#define ERROR_EXPRESSION(...) do { LOG_ERROR(STPP_STRINGIZE_VALUES(__VA_ARGS__)); } while (false);


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