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




