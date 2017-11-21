#pragma once


#include "config.h"

//включать только для объектов, зависящих от оперционной системы
#ifdef ART_WIN32

	#define _UNICODE
	#include <tchar.h>

	#ifndef _WIN32
		#define _WIN32
	#endif

	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0600
	#endif
							   
	#ifndef NO_WINSOCK2
		#include <winsock2.h>
	#endif 
	
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	
	#if defined(DEBUG) | defined(_DEBUG)
		#ifndef _CRTDBG_MAP_ALLOC
			#define _CRTDBG_MAP_ALLOC
			#define D3D_DEBUG_INFO
		#endif
	#endif

	#include <conio.h>
	

	#if defined(_MSC_VER)
		#include <crtdbg.h>
		#include <tchar.h>
		#define MATH_INLINE __forceinline
	#endif

	#if defined(_MSC_VER) || defined(__MINGW32__)
	#define mkdir(p,m) _mkdir(p)
		#if (_MSC_VER < 1900) 
			#define snprintf _snprintf
			#define vsnprintf(b,c,f,a) _vsnprintf(b,c,f,a)
		#endif
	#endif

	#undef min	//resolve conflict with STL
	#undef max

	#ifndef	_CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#ifndef	_CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE// затыкаем варнинги компилятора на тему использования небезопасных функций C
	#endif

#endif

#if defined(__MINGW32__)
#define MATH_INLINE inline
typedef unsigned int DWORD
#endif

#include "autolink.h"

#ifdef ART_WINCE
	//#define _WIN32_WINNT 0x0500
	#include <windows.h>
	#undef min	//resolve conflict with STL
	#undef max
#endif

#ifdef ART_LINUX_INTEL
	#include <time.h>
#endif

#ifdef MAC_OS_X_INTEL
	#define DWORD int
	#define MATH_INLINE inline
	#include <time.h>
#endif

#ifdef ARTY_ANDROID
	#include <jni.h>
	#include <errno.h>
	
	#include <string.h>
	#include <unistd.h>
	#include <sys/resource.h>
	
	#include <android/log.h>
#endif
