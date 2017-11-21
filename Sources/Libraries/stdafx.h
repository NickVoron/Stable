#pragma once
#include "config/platform.h"

#include <malloc.h>
#include <math.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>

#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <sstream>
#include <algorithm>
#include <limits>
#include <memory>
#include <exception>
#include <iostream>
#include <sstream>
#include <mutex>
#include <thread>
#include <utility>
#include <type_traits>

#include <d3dx9.h>
#include <d3dx11.h>

#if ENABLE_OPENGL_ES
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <boost/filesystem.hpp>

