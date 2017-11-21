#pragma once
#include <algorithm>

#include "newmath/library.include.h"
#include "math/library.include.h"

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

	// Quad
	//  v3-------v2
	//  | tr2   /|
	//  |    /   |
	//  | /  tr1 |
	//  v0-------v1
	// tr1: 0,1,2;  t2: 0,2,3;

	struct Quad
	{
		const static unsigned int VERTICES_COUNT = 4;

		Quad() {}
		Quad(const nm::Vector3& v0, const nm::Vector3& v1, const nm::Vector3& v2, const nm::Vector3& v3)
		{
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			vertices[3] = v3;
		}

		static Quad create(const State& state, const Vector2& halfSize);

		void transform(const State& state, float scale);
		nm::Vector3 vertices[4];
	};

	template<class VertexType, class IndexType>
	void fillMesh(const Quad& quad, Geometry::Mesh<VertexType, IndexType>& mesh)
	{
		auto& vs = mesh.vertices();
		auto& is = mesh.indices();

		vs.clear();
		vs.resize(4);
		vs[0].pos3() = quad.vertices[0];
		vs[1].pos3() = quad.vertices[1];
		vs[2].pos3() = quad.vertices[2];
		vs[3].pos3() = quad.vertices[3];


		vs[0].template uv2<0>() = Vector2(0, 1);
		vs[1].template uv2<0>() = Vector2(1, 1);
		vs[2].template uv2<0>() = Vector2(1, 0);
		vs[3].template uv2<0>() = Vector2(0, 0);

		is.clear();
		is.resize(6);
		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		is[3] = 2;
		is[4] = 3;
		is[5] = 0;

		Geometry::generateNormals(mesh);
	}


	template<class VertexType, class IndexType>
	void addToMesh(const Quad& quad , Geometry::Mesh<VertexType, IndexType>& mesh)
	{
		typedef Geometry::Mesh<VertexType, IndexType> Mesh;
		Mesh quadMesh;

		fillMesh(quad, quadMesh);
		Geometry::mergeMeshes(mesh, quadMesh);
	}
}
