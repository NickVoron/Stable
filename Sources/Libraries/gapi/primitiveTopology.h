#pragma once

#include "id.h"

namespace gapi
{						   
	enum PrimitiveTopology
	{
		POINTLIST		= 1,
		LINELIST		= 2,
		LINESTRIP		= 3,
		TRIANGLELIST	= 4,
		TRIANGLESTRIP	= 5,

		TOPOLOGY_1_POINT = 33,
		TOPOLOGY_2_POINT = 34,
		TOPOLOGY_3_POINT = 35,
		TOPOLOGY_4_POINT = 36,
	};

	template<Id id, PrimitiveTopology topology>	struct Topology;

	template<PrimitiveTopology primitiveType>
	struct PrimitiveCountEvaluator 
	{
		template<PrimitiveTopology pt> struct impl;

		template<> struct impl<POINTLIST>		{ static inline int eval(int indicesCount) { return indicesCount + 0; } };
		template<> struct impl<LINELIST>		{ static inline int eval(int indicesCount) { return indicesCount / 2; } };
		template<> struct impl<LINESTRIP>		{ static inline int eval(int indicesCount) { return indicesCount - 1; } };
		template<> struct impl<TRIANGLELIST>	{ static inline int eval(int indicesCount) { return indicesCount / 3; } };
		template<> struct impl<TRIANGLESTRIP>	{ static inline int eval(int indicesCount) { return indicesCount - 2; } };
		//template<> struct impl<TRIANGLEFAN>	{ static inline int eval(int indicesCount) { return indicesCount - 2; } };

		template<> struct impl<TOPOLOGY_3_POINT>	{ static inline int eval(int indicesCount) { return indicesCount / 3; } };

		static inline int eval(int indicesCount){ return impl<primitiveType>::eval(indicesCount); }
	};
}