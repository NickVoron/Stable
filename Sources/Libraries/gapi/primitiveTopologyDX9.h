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