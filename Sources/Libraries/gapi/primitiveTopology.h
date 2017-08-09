// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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

	template<PrimitiveTopology primitiveType>
	struct PrimitiveCountEvaluator 
	{
		template<PrimitiveTopology pt> struct impl;

		template<> struct impl<POINTLIST>		{ static inline int eval(int indicesCount) { return indicesCount + 0; } };
		template<> struct impl<LINELIST>		{ static inline int eval(int indicesCount) { return indicesCount / 2; } };
		template<> struct impl<LINESTRIP>		{ static inline int eval(int indicesCount) { return indicesCount - 1; } };
		template<> struct impl<TRIANGLELIST>	{ static inline int eval(int indicesCount) { return indicesCount / 3; } };
		template<> struct impl<TRIANGLESTRIP>	{ static inline int eval(int indicesCount) { return indicesCount - 2; } };
		

		template<> struct impl<TOPOLOGY_3_POINT>	{ static inline int eval(int indicesCount) { return indicesCount / 3; } };

		static inline int eval(int indicesCount){ return impl<primitiveType>::eval(indicesCount); }
	};
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