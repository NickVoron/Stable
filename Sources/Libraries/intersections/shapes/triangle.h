#pragma once

#include <iostream>
#include <algorithm>

#include "math/library.include.h"
#include "newmath/library.include.h"

namespace Geometry
{
	template<class Vertex_, class IndexType_>
	class Mesh;

	template<class VertexType, class IndexType>
	void mergeMeshes(Mesh<VertexType, IndexType>& resMesh, const Mesh<VertexType, IndexType>& addMesh);

	template<class Mesh>
	void generateNormals(Mesh& mesh);
}

namespace intersections
{

	struct Triangle
	{
		Triangle() {}
		Triangle(const nm::Vector3& v0, const nm::Vector3& v1, const nm::Vector3& v2)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
		}

		nm::Vector3 vertices[3];

		inline nm::Vector3 ab() const { return vertices[1] - vertices[0]; }
		inline nm::Vector3 ac() const { return vertices[2] - vertices[0]; }
	};


	template<class VertexType, class IndexType>
	void fillMesh(const Triangle& tr, Geometry::Mesh<VertexType, IndexType>& mesh)
	{
		auto& vs = mesh.vertices();
		auto& is = mesh.indices();

		vs.clear();
		vs.resize(3);

		vs[0].pos3() = tr.vertices[0];
		vs[1].pos3() = tr.vertices[1];
		vs[2].pos3() = tr.vertices[2];


		vs[0].template uv2<0>() = Vector2(0, 1);
		vs[1].template uv2<0>() = Vector2(1, 1);
		vs[2].template uv2<0>() = Vector2(1, 0);
		
		is.clear();
		is.resize(3);
		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

	}

	template<class VertexType, class IndexType>
	void addToMesh(const Triangle& tr, Geometry::Mesh<VertexType, IndexType>& mesh)
	{
		typedef Geometry::Mesh<VertexType, IndexType> Mesh;
		Mesh quadMesh;

		fillMesh(tr, quadMesh);
		Geometry::mergeMeshes(mesh, quadMesh);
	}



}