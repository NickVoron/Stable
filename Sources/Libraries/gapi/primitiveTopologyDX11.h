#pragma once

#include "gapi.h"			  
#include "primitiveTopology.h"

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