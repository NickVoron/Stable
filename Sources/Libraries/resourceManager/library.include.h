#pragma once

#ifdef WIN32
#define NET_RES_DRIVER
#endif

#include "store.h"
#include "resourceSet.h"
#include "resource.h"
#include "dependencies.h"
#include "resourceLoader.h"

#ifdef DEFINE_SDK_LIB
#pragma DEFINE_SDK_LIB(resourceManager)
#endif

