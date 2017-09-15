// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; 
	LPCSTR szName; 
	DWORD dwThreadID; 
	DWORD dwFlags; 
} THREADNAME_INFO;
#pragma pack(pop)


inline void SetThreadName(uint32_t dwThreadID, const char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*) &info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

inline void SetThreadName(const char* threadName)
{
	SetThreadName(GetCurrentThreadId(), threadName);
}

inline void SetThreadName(std::thread* thread, const char* threadName)
{
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread->native_handle()));
	SetThreadName(threadId, threadName);
}

#elif defined(__ANDROID__)
#include <pthread.h>
inline void SetThreadName(std::thread* thread, const char* threadName)
{
	auto handle = thread->native_handle();
	pthread_setname_np(handle, threadName);
}	   

#include <sys/prctl.h>
inline void SetThreadName(const char* threadName)
{
	prctl(PR_SET_NAME, threadName, 0, 0, 0);
}

#else
#include <pthread.h>
inline void SetThreadName(std::thread* thread, const char* threadName)
{
	pthread_setname_np(threadName);
}	   

inline void SetThreadName(const char* threadName)
{
	pthread_setname_np(threadName);
}
#endif

template <class F, class ... Args>
inline std::thread launch_named_thread(const char* name, F&& f, Args&&... args)
{
	return std::thread([=]() 
	{
		
		SetThreadName(name);
		f(args...);
		
	});
}

template <class F, class ... Args>
inline std::thread launch_infinite_thread(const char* name, unsigned int sleepTime, F&& f, Args&&... args)
{
	return std::thread([=]()
	{
		LOG_MSG("start infinite thread: " << name);
		SetThreadName(name);
		while (true)
		{
			f(std::forward<Args>(args)...);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}		
		LOG_MSG("finish infinite thread: " << name);
	});
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