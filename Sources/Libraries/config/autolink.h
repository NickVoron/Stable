#pragma once

#if defined(DEBUG) | defined(_DEBUG)
#define CONFIGURATION_NAME "Debug"
#else
#define CONFIGURATION_NAME "Release"
#endif

#if defined(_MSC_VER)
#ifndef DEFINE_SDK_LIB
#define DEFINE_SDK_LIB(LIB) comment(lib, #LIB "." CONFIGURATION_NAME "." PLATFORM_NAME ".lib")
#endif
#endif


