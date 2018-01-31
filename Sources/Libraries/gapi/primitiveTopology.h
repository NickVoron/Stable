// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	template<PrimitiveTopology pt> struct PrimitiveCount;

	template<> struct PrimitiveCount<POINTLIST> { static inline int eval(int indicesCount) { return indicesCount + 0; } };
	template<> struct PrimitiveCount<LINELIST> { static inline int eval(int indicesCount) { return indicesCount / 2; } };
	template<> struct PrimitiveCount<LINESTRIP> { static inline int eval(int indicesCount) { return indicesCount - 1; } };
	template<> struct PrimitiveCount<TRIANGLELIST> { static inline int eval(int indicesCount) { return indicesCount / 3; } };
	template<> struct PrimitiveCount<TRIANGLESTRIP> { static inline int eval(int indicesCount) { return indicesCount - 2; } };
	
	template<> struct PrimitiveCount<TOPOLOGY_3_POINT> { static inline int eval(int indicesCount) { return indicesCount / 3; } };

	template<PrimitiveTopology primitiveType>
	struct PrimitiveCountEvaluator 
	{
		static inline int eval(int indicesCount){ return PrimitiveCount<primitiveType>::eval(indicesCount); }
	};
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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