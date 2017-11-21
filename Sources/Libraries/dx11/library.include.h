#pragma once

#ifdef WIN32
#include "dx11.h"

#ifdef DEFINE_SDK_LIB
#pragma DEFINE_SDK_LIB(dx11)
#endif

#ifdef _WIN64
#pragma comment(lib, "x64/d3d11.lib")
#pragma comment(lib, "x64/d3dx11.lib")
#pragma comment(lib, "x64/dxgi.lib")
#pragma comment(lib, "x64/dxguid.lib")
#pragma comment(lib, "x64/d3dcompiler.lib")
#else
#pragma comment(lib, "x86/d3d11.lib")
#pragma comment(lib, "x86/d3dx11.lib")
#pragma comment(lib, "x86/dxgi.lib")
#pragma comment(lib, "x86/dxguid.lib")
#pragma comment(lib, "x86/d3dcompiler.lib")
#endif
#endif