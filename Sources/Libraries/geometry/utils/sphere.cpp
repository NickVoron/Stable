// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "bodies.h"
#include "meshOperations.h"


namespace Geometry {
namespace Generation {
namespace Private	{

namespace
{
	class SphereGenerator
	{

	public:
		SphereGenerator(BaseVertexStream& vs, BaseIndexStream& is, int subdivideCount, float radius);

	private:
		void subdivide(BaseVertexStream& vs, BaseIndexStream& is, float radius);
		int search_midpoint (BaseVertexStream& vs, BaseIndexStream& is, float radius, int index_start, int index_end);

		void copyArrays(BaseVertexStream& vs, BaseIndexStream& is,  float* vertices, int* faces, int verticesCount, int facesCount, float radius);

		void init_icosahedron(BaseVertexStream& vs, BaseIndexStream& is,  float radius);

		int n_vertices;
		int n_faces;
		int n_edges;
		int edge_walk;

		typedef std::vector<int> IntVec;

		IntVec start; 
		IntVec end; 
		IntVec midpoint;
	};

	void SphereGenerator::copyArrays(BaseVertexStream& vs, BaseIndexStream& is, float* p_vertices, int* p_faces, int verticesCount, int facesCount, float radius)
	{
		int indicesCount = facesCount * 3;

		vs.resize(verticesCount);
		is.resize(facesCount * 3);

		for(int i = 0; i < verticesCount; ++i)
		{
			POS3 pos;
			pos.x = p_vertices[3*i + 0];
			pos.y = p_vertices[3*i + 1];
			pos.z = p_vertices[3*i + 2];

			pos.normalize();

			vs[i].pos3(pos * radius);
		}

		for(int i = 0; i < indicesCount; ++i)
		{
			is[i] = p_faces[i];
		}

		is.updateMaxValue();
	}

	void SphereGenerator::init_icosahedron(BaseVertexStream& vs, BaseIndexStream& is, float radius) 
	{ 
		float t		= (1+sqrt(5.0f))/2;
		float tau	= t/sqrt(1+t*t);
		float one	= 1/sqrt(1+t*t);

		float icosahedron_vertices[] = { tau, one, 0.0,
			-tau, one, 0.0,
			-tau, -one, 0.0,
			tau, -one, 0.0,
			one, 0.0 ,  tau,
			one, 0.0 , -tau,
			-one, 0.0, -tau,
			-one, 0.0, tau,
			0.0 , tau, one,
			0.0 , -tau, one,
			0.0 , -tau, -one,
			0.0 , tau, -one};

		int icosahedron_faces[] = { 4, 8, 7,
			4, 7, 9,
			5, 6, 11,
			5, 10, 6,
			0, 4, 3,
			0, 3, 5,
			2, 7, 1,
			2, 1, 6,
			8, 0, 11,
			8, 11, 1,
			9, 10, 3,
			9, 2, 10,
			8, 4, 0,
			11, 0, 5,
			4, 9, 3,
			5, 3, 10,
			7, 8, 1,
			6, 1, 11,
			7, 2, 9,
			6, 10, 2};

		n_vertices = 12; 
		n_faces = 20;
		n_edges = 30;

		copyArrays(vs, is, icosahedron_vertices, icosahedron_faces, n_vertices, n_faces, radius);
	} 

	int SphereGenerator::search_midpoint (BaseVertexStream& vs, BaseIndexStream& is, float radius, int index_start, int index_end) 
	{ 
		for (int i=0; i < edge_walk; ++i)
		{
			if ((start[i] == index_start && end[i] == index_end) || (start[i] == index_end && end[i] == index_start)) 
			{
				int res = midpoint[i];

				
				start[i]    = start	  [edge_walk-1];
				end[i]      = end	  [edge_walk-1];
				midpoint[i] = midpoint[edge_walk-1];
				edge_walk--;

				return res; 
			}
		}

		
		start	[edge_walk] = index_start;
		end		[edge_walk] = index_end; 
		midpoint[edge_walk] = n_vertices; 

		
		POS3 newPos = (vs[index_start].pos3() + vs[index_end].pos3()) / 2.0;
		newPos.normalize();
		newPos *= radius;

		vs[n_vertices].pos3() = newPos;

		n_vertices++;
		edge_walk++;
		return midpoint[edge_walk-1];
	} 

	void SphereGenerator::subdivide(BaseVertexStream& vs, BaseIndexStream& is, float radius) 
	{ 
		int n_vertices_new = n_vertices+2*n_edges; 
		int n_faces_new = 4*n_faces; 

		edge_walk = 0; 
		n_edges		= 2*n_vertices + 3*n_faces;

		start.resize(n_edges);		
		end.resize(n_edges);		
		midpoint.resize(n_edges);	

		BaseIndexStream oldIndices;
		oldIndices.assign(is);

		vs.resize(n_vertices_new);
		is.resize(n_faces_new * 3);
		n_faces_new = 0; 

		for (int i=0; i< n_faces; i++) 
		{ 
			Face<unsigned int> oldFace = oldIndices.getFace(i);

			int ab_midpoint = search_midpoint (vs, is, radius, oldFace.b, oldFace.a); 
			int bc_midpoint = search_midpoint (vs, is, radius, oldFace.c, oldFace.b); 
			int ca_midpoint = search_midpoint (vs, is, radius, oldFace.a, oldFace.c);

			FaceRef<unsigned int> face1 = is.getFaceRef(n_faces_new + 0);
			FaceRef<unsigned int> face2 = is.getFaceRef(n_faces_new + 1);
			FaceRef<unsigned int> face3 = is.getFaceRef(n_faces_new + 2);
			FaceRef<unsigned int> face4 = is.getFaceRef(n_faces_new + 3);

			face1.a = oldFace.a; 
			face1.b = ab_midpoint; 
			face1.c = ca_midpoint; 

			face2.a = ca_midpoint; 
			face2.b = ab_midpoint; 
			face2.c = bc_midpoint; 

			face3.a = ca_midpoint; 
			face3.b = bc_midpoint; 
			face3.c = oldFace.c; 

			face4.a = ab_midpoint; 
			face4.b = oldFace.b; 
			face4.c = bc_midpoint;

			n_faces_new += 4;
		} 

		n_faces = n_faces_new;
	}

	SphereGenerator::SphereGenerator(BaseVertexStream& vs, BaseIndexStream& is, int subdivideCount, float radius): start(0), end(0), midpoint(0)
	{
		init_icosahedron(vs, is, radius);

		for (int i = 0; i < subdivideCount; i++) 
		{
			subdivide(vs, is, radius);
		}

		is.updateMaxValue();
	}
}

void generateSphere(BaseVertexStream& vs, BaseIndexStream& is, float radius, int subdiv)
{
	vs.resize(0);
	is.resize(0);
 
  	SphereGenerator generator(vs, is, subdiv, radius);
  	generateNormals(vs, is);
}

void generateSphere(BaseMesh& mesh, float radius, int subdiv)
{
	generateSphere(mesh.vertices(), mesh.indices(), radius, subdiv);
}


}
}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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