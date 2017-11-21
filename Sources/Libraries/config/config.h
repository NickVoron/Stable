#pragma once

#if defined(WIN32) && !defined(WINCE)
#include "config/win32.h"
#define ART_WIN32
#endif

#if defined(WIN32) && defined(WINCE)
#include "config/winCE.h"
#define ART_WINCE
#endif

#if defined(__linux__) && defined(__i386__)
#include "config/linux.h"
#define ART_LINUX_INTEL
#endif
