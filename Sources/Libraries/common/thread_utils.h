#pragma once

#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
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
		//LOG_MSG("start thread: " << name);
		SetThreadName(name);
		f(args...);
		//LOG_MSG("finish thread: " << name);
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
