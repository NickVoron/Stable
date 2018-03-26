// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "config/platform.h"
#include <iostream>
#include "commonAppModules/library.include.h"
#include "applib_Params/library.include.h"
#include "applib_Main/library.include.h"

#include <boost/filesystem.hpp>

#include "defaultLogs/library.include.h"
#include "streaming/library.include.h"
#include "stream/library.include.h"
#include "stuff/library.include.h"
#include "strings/library.include.h"

#ifdef WIN32
#include <Windows.h>
#endif


#include <d3dx9.h>

#include "appModule/library.include.h"
#include "cameraSystem/library.include.h"
#include "inputSysWin/library.include.h"
#include "settingsRegistry/library.include.h"
#include "defaultLogs/library.include.h"
#include "windowsGAPI/library.include.h"
#include "viewports/library.include.h"
#include "profiler/library.include.h"
#include "dx9/library.include.h"
#include "dx11/library.include.h"
#include "commandProcessor/library.include.h"
#include "componentModel/library.include.h"
#include "debugDrawComponents/library.include.h"
#include "commonComponents/library.include.h"	
#include "componentModelUtils/library.include.h"
#include "libfreeimage/library.include.h"


#include <stdio.h>

#include "math/library.include.h"
#include "libopenal/library.include.h"


#include "defaultLogs/library.include.h"
#include "componentModel/library.include.h"


#include "defaultLogs/library.include.h"
#include "componentModel/library.include.h"
#include "profiler/library.include.h"


#include <type_traits>

#include "defaultLogs/library.include.h"

#include "memory/library.include.h"
#include "Loki/library.include.h"
#include "stuff/library.include.h"
#include "strings/library.include.h"
#include "expressions/library.include.h"


#include "componentModelConfigurator/library.include.h"
#include "newmath/library.include.h"
#include "math/library.include.h"
#include "resourceUtils/library.include.h"
#include "expressions/library.include.h"
#include "stuff/library.include.h"

#include <map>
#include <iostream>


#include "commonComponents/library.include.h"


#include <boost/filesystem.hpp>
#include <boost/any.hpp>

#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>

#include <utility>

#include "defaultLogs/library.include.h"
#include "Loki/library.include.h"
#include "math/library.include.h"
#include "image/library.include.h"
#include "stuff/library.include.h"
#include "expressions/library.include.h"
#include "reflection/library.include.h"


#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>

#include <utility>

#include "defaultLogs/library.include.h"
#include "Loki/library.include.h"
#include "math/library.include.h"
#include "image/library.include.h"
#include "stuff/library.include.h"
#include <boost/any.hpp>


#include <boost/filesystem.hpp>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <iostream>

#include <utility>

#include "defaultLogs/library.include.h"
#include "Loki/library.include.h"
#include "math/library.include.h"
#include "image/library.include.h"
#include "stuff/library.include.h"
#include <boost/any.hpp>


#include <d3d9.h>

#include "stuff/library.include.h"
#include "containers/library.include.h"


#include "defaultLogs/library.include.h"
#include "memory/library.include.h"
#include "textFile/library.include.h"
#include "gapi/library.include.h"
#include "glew/library.include.h"


#include "defaultLogs/library.include.h"
#include "memory/library.include.h"
#include "textFile/library.include.h"
#include "gapi/library.include.h"
#include "glew/library.include.h"


#include "CameraSystem/library.include.h"


#include "math/library.include.h"
#include "defaultLogs/library.include.h"


#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include <boost/tokenizer.hpp>

#include "stuff/library.include.h"


#if defined(USE_WINDOWS)

	#define WIN32_LEAN_AND_MEAN
	#include <stdio.h>
	#include <tchar.h>

	#include "dx9/library.include.h"
	#include "dx11/library.include.h"

#endif

#include "defaultLogs/library.include.h"
#include "newmath/library.include.h"

#include "stuff/stringUtils.h"

#include "gapi/library.include.h"
#include "math/library.include.h"
#include "intersections/library.include.h"
#include "geometry/library.include.h"
#include "viewports/library.include.h"
#include "image/library.include.h"


#include <iostream>
#include <exception>
#include <list>
#include <deque>
#include <string>

#include "stuff/library.include.h"


#include "utfcvt/library.include.h"
#include "stream/library.include.h"
#include "stuff/library.include.h"


#include "geometry/library.include.h"
#include "font/library.include.h"


#include "math/library.include.h"
#include "utfcvt/library.include.h"
#include "stuff/library.include.h"
#include "dx9/library.include.h"



#include <iostream>
#include <algorithm>
#include <vector>

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>

#include "nvtristrip/NvTriStrip.h"
#include "math/library.include.h"
#include "stream/library.include.h"
#include "memory/library.include.h"
#include "intersections/library.include.h"


#include <map>

#include "resourceFont/library.include.h"
#include "resourceAtlas/library.include.h"

#include "debugDrawing/library.include.h"


#include <vector>
#include <string>
#include <map>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <math.h>

#include "stuff/library.include.h"
#include "exceptions/library.include.h"
#include "imgui/library.include.h"


#if defined(USE_WINDOWS)
#include "dx11/library.include.h"
#include "gapi/library.include.h"
#endif


#include <iostream>
#include <algorithm>

#include "defaultLogs/library.include.h"
#include "newmath/library.include.h"
#include "math/library.include.h"
#include "debugDrawing/library.include.h"


#include <math.h>
#include <stdlib.h>
#include <limits>
#include <ostream>
#include <float.h>

#include "memory/library.include.h"


#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>



#include <string>
#include "containers/fixedArray.h"
#include "memory/library.include.h"


#include "stream/library.include.h"


#include <vector>

#include "rectAtlas/library.include.h"
#include "image/library.include.h"
#include "imageD3D/library.include.h"
#include "libpsd/library.include.h"
#include "stream/library.include.h"


#include "unigui/rect.h"


#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

#include <stdlib.h>
#include <stdio.h>

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>

#include "hash/library.include.h"
#include "zlib/library.include.h"
#include "rijndael/library.include.h"
#include "memory/library.include.h"


#include <string>
#include "defaultLogs/library.include.h"
#include "streaming/library.include.h"


#include <assert.h>
#include <time.h>
#include <stdio.h>

#include <sys/types.h>

#include <algorithm>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <set>
#include <sstream>
#include <iomanip>

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

#include <boost/tokenizer.hpp>

#include "exceptions/library.include.h"
#include "math/library.include.h"
#include "stream/library.include.h"
#include "containers/library.include.h"


#include "stuff/library.include.h"


#include <vector>

#include "image/library.include.h"
#include "unigui/library.include.h"
#include "curve/library.include.h"

#if defined(USE_WINDOWS)
#include "dx9/library.include.h"
#include "dx11/library.include.h"
#include "resourceTexture/library.include.h"
#include "resourceAtlas/library.include.h"
#include "resourceFont/library.include.h"
#endif

#include "immediateDraw/library.include.h"


#include "expressions/library.include.h"


#include "opengles/library.include.h"
#include "opengl/library.include.h"
#include "dx11/library.include.h"

#include "windowsGUI/library.include.h"


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


#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <fstream>


#define UTP_DEBUG_LOGGING 0


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>
#include <exception>
#include <sstream>
#include <algorithm>

#include <assert.h>
#include <float.h>
#include <errno.h>
#include <limits.h>
#include <string.h>


#define UDT_EXPORTS

#ifdef DEFINE_SDK_LIB
#pragma DEFINE_SDK_LIB(pch)
#endif













// Copyright (C) 2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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