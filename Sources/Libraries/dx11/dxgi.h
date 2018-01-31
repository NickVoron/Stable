// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <dxgi.h>

namespace dxgi
{
	template<typename BaseType, int channelsCount> struct FormatFind;

	template<> struct FormatFind<float, 1>			{ static const DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT;			};
	template<> struct FormatFind<float, 2>			{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32_FLOAT;		};
	template<> struct FormatFind<float, 3>			{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_FLOAT;	};
	template<> struct FormatFind<float, 4>			{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;	};

	template<> struct FormatFind<unsigned char, 1>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R8_UINT;			};
	template<> struct FormatFind<unsigned char, 2>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R8G8_UINT;			};
	template<> struct FormatFind<unsigned char, 4>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UINT;		};

	template<> struct FormatFind<unsigned short, 1> { static const DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;			};
	template<> struct FormatFind<unsigned short, 2> { static const DXGI_FORMAT format = DXGI_FORMAT_R16G16_UINT;		};
	template<> struct FormatFind<unsigned short, 4> { static const DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_UINT;	};

	template<> struct FormatFind<unsigned int, 1>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R32_UINT;			};
	template<> struct FormatFind<unsigned int, 2>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32_UINT;		};
	template<> struct FormatFind<unsigned int, 3>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32B32_UINT;		};
	template<> struct FormatFind<unsigned int, 4>	{ static const DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_UINT;	};
}








// Copyright (C) 2012-2013 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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