// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once


#include "config.h"


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

	#define BOOST_NO_CXX11_NOEXCEPT
	
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

	#undef min	
	#undef max

	#ifndef	_CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#ifndef	_CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE
	#endif

#endif

#if defined(__MINGW32__)
#define MATH_INLINE inline
typedef unsigned int DWORD
#endif

#include "autolink.h"

#ifdef ART_WINCE
	
	#include <windows.h>
	#undef min	
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




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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