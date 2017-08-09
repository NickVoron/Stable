// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "primitiveTopology.h"

#include <d3d9.h>

namespace gapi
{

	template<> struct Topology<DX9, POINTLIST>		{ static const D3DPRIMITIVETYPE topology = D3DPT_POINTLIST;		};
	template<> struct Topology<DX9, LINELIST>		{ static const D3DPRIMITIVETYPE topology = D3DPT_LINELIST;		};
	template<> struct Topology<DX9, LINESTRIP>		{ static const D3DPRIMITIVETYPE topology = D3DPT_LINESTRIP;		};
	template<> struct Topology<DX9, TRIANGLELIST>	{ static const D3DPRIMITIVETYPE topology = D3DPT_TRIANGLELIST;	};
	template<> struct Topology<DX9, TRIANGLESTRIP>	{ static const D3DPRIMITIVETYPE topology = D3DPT_TRIANGLESTRIP;	};

	template<D3DPRIMITIVETYPE topology>			struct Topology9;
	template<D3DPRIMITIVETYPE topology>			struct PrimitiveCountEvaluator9 { static inline int eval(int indicesCount) { return PrimitiveCountEvaluator< Topology9<topology>::topology >::eval(indicesCount); } };

 	template<>	struct Topology9<D3DPT_POINTLIST>		{ static const PrimitiveTopology topology = POINTLIST;		};
 	template<>	struct Topology9<D3DPT_LINELIST>		{ static const PrimitiveTopology topology = LINELIST;		};
 	template<>	struct Topology9<D3DPT_LINESTRIP>		{ static const PrimitiveTopology topology = LINESTRIP;		};
 	template<>	struct Topology9<D3DPT_TRIANGLELIST>	{ static const PrimitiveTopology topology = TRIANGLELIST;	};
 	template<>	struct Topology9<D3DPT_TRIANGLESTRIP>	{ static const PrimitiveTopology topology = TRIANGLESTRIP;	};
	

}



// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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