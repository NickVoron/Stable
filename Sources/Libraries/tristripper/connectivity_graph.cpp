// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//










#include "detail/connectivity_graph.h"

#include <algorithm>




namespace triangle_stripper {

	namespace detail {




namespace
{

	class tri_edge : public triangle_edge
	{
	public:
		tri_edge(index A, index B, size_t TriPos)
			: triangle_edge(A, B), m_TriPos(TriPos) { }

		size_t TriPos() const { return m_TriPos; }

	private:
		size_t	m_TriPos;
	};


	class cmp_tri_edge_lt
	{
	public:
		bool operator() (const tri_edge & a, const tri_edge & b) const;
	};


	typedef std::vector<tri_edge> edge_map;


	void LinkNeighbours(graph_array<triangle> & Triangles, const edge_map & EdgeMap, const tri_edge Edge);

}




void make_connectivity_graph(graph_array<triangle> & Triangles, const indices & Indices)
{
	assert(Triangles.size() == (Indices.size() / 3));

	
	for (size_t i = 0; i < Triangles.size(); ++i)
		Triangles[i] = triangle(Indices[i * 3 + 0], Indices[i * 3 + 1], Indices[i * 3 + 2]);

	
	edge_map EdgeMap;
	EdgeMap.reserve(Triangles.size() * 3);

	for (size_t i = 0; i < Triangles.size(); ++i) {

		const triangle & Tri = * Triangles[i];

		EdgeMap.push_back(tri_edge(Tri.A(), Tri.B(), i)); 
		EdgeMap.push_back(tri_edge(Tri.B(), Tri.C(), i)); 
		EdgeMap.push_back(tri_edge(Tri.C(), Tri.A(), i)); 
	}

	std::sort(EdgeMap.begin(), EdgeMap.end(), cmp_tri_edge_lt());

	
	for (size_t i = 0; i < Triangles.size(); ++i) {

		const triangle & Tri = * Triangles[i];

		LinkNeighbours(Triangles, EdgeMap, tri_edge(Tri.B(), Tri.A(), i)); 
		LinkNeighbours(Triangles, EdgeMap, tri_edge(Tri.C(), Tri.B(), i)); 
		LinkNeighbours(Triangles, EdgeMap, tri_edge(Tri.A(), Tri.C(), i)); 
	}
}



namespace
{
	
	inline bool cmp_tri_edge_lt::operator() (const tri_edge & a, const tri_edge & b) const
	{
		const index A1 = a.A();
		const index B1 = a.B();
		const index A2 = b.A();
		const index B2 = b.B();

		if ((A1 < A2) || ((A1 == A2) && (B1 < B2)))
			return true;
		else
			return false;
	}


	void LinkNeighbours(graph_array<triangle> & Triangles, const edge_map & EdgeMap, const tri_edge Edge)
	{
		
		edge_map::const_iterator it = std::lower_bound(EdgeMap.begin(), EdgeMap.end(), Edge, cmp_tri_edge_lt());

		
		
		
		for (; (it != EdgeMap.end()) && (Edge == (* it)); ++it)
			Triangles.insert_arc(Edge.TriPos(), it->TriPos());

		
	}

}




	} 

} 





// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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