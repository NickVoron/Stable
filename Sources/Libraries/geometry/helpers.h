// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "mesh/mesh.h"

namespace Geometry
{
	template<class VertexType, class IndexType = unsigned int>
	struct Types
	{
		using Index = IndexType;
		using Vertex = VertexType;
		using Mesh = Geometry::Mesh<Vertex, IndexType> ;
		using IS = typename Mesh::IndexStreamType;
		using VS = typename Mesh::VertexStreamType;
	};

	template<class MeshVertexType, class InstanceVertexType, class IndexType = unsigned int>
	struct InstancedTypes
	{
		using Index = IndexType;
 		using MeshVertex = MeshVertexType;
		using InstanceVertex = InstanceVertexType;
 		typedef Loki::MakeTypelist<MeshVertex, InstanceVertex> VerticesList;
 
		using Mesh = Geometry::Mesh<MeshVertex, IndexType>;
		using IS = typename Mesh::IndexStreamType;
		using VS = typename Mesh::VertexStreamType;
 
 		typedef VertexStream<InstanceVertex> InstanceVS;
	};
}





// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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