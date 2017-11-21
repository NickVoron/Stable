#pragma once

#ifdef WIN32

#include "dx9.h"
#include "vertexDeclaration.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "dynamicMesh.h"
#include "dip.h"
#include "query.h"
#include "cubemap.h"
#include "stats.h"
#include "instancedRenderer.h"

#ifdef DEFINE_SDK_LIB 
#pragma DEFINE_SDK_LIB(dx9)
#endif


#ifdef _WIN64
#pragma comment(lib, "x64/d3d9.lib")
#pragma comment(lib, "x64/d3dx9.lib")
#else
#pragma comment(lib, "x86/d3d9.lib")
#pragma comment(lib, "x86/d3dx9.lib")
#endif

#endif // WIN32


