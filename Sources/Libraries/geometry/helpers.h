#pragma once

#include "mesh/mesh.h"

namespace Geometry
{
	template<class Vertex, class IndexType = unsigned int>
	struct Types
	{
 		typedef IndexType IndexType;
 		typedef Vertex Vertex;
 		typedef Geometry::Mesh<Vertex, IndexType> Mesh;
 		typedef typename Mesh::IndexStreamType IS;
 		typedef typename Mesh::VertexStreamType VS;
	};

	template<class MeshVertex, class InstanceVertex, class IndexType = unsigned int>
	struct InstancedTypes
	{
 		typedef IndexType Index;
 		typedef MeshVertex MeshVertex;
 		typedef InstanceVertex InstanceVertex;
 		typedef Loki::MakeTypelist<MeshVertex, InstanceVertex> VerticesList;
 
 		typedef Geometry::Mesh<MeshVertex, Index> Mesh;
 		typedef typename Mesh::IndexStreamType IS;
 		typedef typename Mesh::VertexStreamType VS;
 
 		typedef typename Geometry::VertexStream<InstanceVertex> InstanceVS;
	};
}

