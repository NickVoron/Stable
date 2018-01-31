// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "gapi.h"			  
#include "primitiveTopology.h"

#if defined(USE_WINDOWS)
#include <d3d11.h>

namespace gapi
{	   
	template<> struct Topology<DX11, POINTLIST>		{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;	};
	template<> struct Topology<DX11, LINELIST>		{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;		};
	template<> struct Topology<DX11, LINESTRIP>		{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;	};
	template<> struct Topology<DX11, TRIANGLELIST>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	};
	template<> struct Topology<DX11, TRIANGLESTRIP>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;};

	template<> struct Topology<DX11, TOPOLOGY_1_POINT>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST;	};
	template<> struct Topology<DX11, TOPOLOGY_2_POINT>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST;	};
	template<> struct Topology<DX11, TOPOLOGY_3_POINT>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;	};
	template<> struct Topology<DX11, TOPOLOGY_4_POINT>	{ static const D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;	};

	template<D3D_PRIMITIVE_TOPOLOGY topology>	struct Topology11;
	template<D3D_PRIMITIVE_TOPOLOGY topology>	struct PrimitiveCountEvaluator11 { static inline int eval(int indicesCount) { return PrimitiveCountEvaluator< Topology11<topology>::topology >::eval(indicesCount); } };

 	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_POINTLIST>		{ static const PrimitiveTopology topology = POINTLIST;		};
 	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_LINELIST>		{ static const PrimitiveTopology topology = LINELIST;		};
 	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP>		{ static const PrimitiveTopology topology = LINESTRIP;		};
 	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST>	{ static const PrimitiveTopology topology = TRIANGLELIST;	};
 	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP>	{ static const PrimitiveTopology topology = TRIANGLESTRIP;	};
	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST>	{ static const PrimitiveTopology topology = TOPOLOGY_1_POINT;	};
	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST>	{ static const PrimitiveTopology topology = TOPOLOGY_2_POINT;	};
	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST>	{ static const PrimitiveTopology topology = TOPOLOGY_3_POINT;	};
	template<>	struct Topology11<D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST>	{ static const PrimitiveTopology topology = TOPOLOGY_4_POINT;	};
}

#endif



// Copyright (C) 2012-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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