// Copyright (C) 2012 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//







































#if defined(max)
#error "'max' macro defined! It's against the C++ standard. Please use 'std::max' instead (undefine 'max' macro if it was defined in another library)."
#endif


#if defined(min)
#error "'min' macro defined! It's against the C++ standard. Please use 'std::min' instead (undefine 'min' macro if it was defined in another library)."
#endif



#ifndef TRI_STRIPPER_HEADER_GUARD_TRI_STRIPPER_H
#define TRI_STRIPPER_HEADER_GUARD_TRI_STRIPPER_H

#include "public_types.h"

#include "detail/cache_simulator.h"
#include "detail/graph_array.h"
#include "detail/heap_array.h"
#include "detail/types.h"




namespace triangle_stripper {




class tri_stripper
{
public:

	tri_stripper(const indices & TriIndices);

	void Strip(primitive_vector * out_pPrimitivesVector);

	
	
	
	void SetCacheSize(size_t CacheSize = 10);

	
	
	void SetMinStripSize(size_t MinStripSize = 2);

	
	
	
	
	
	
	void SetBackwardSearch(bool Enabled = false);
	
	
	
	
	
	void SetPushCacheHits(bool Enabled = true);

	

private:

	typedef detail::graph_array<detail::triangle> triangle_graph;
	typedef detail::heap_array<size_t, std::greater<size_t> > triangle_heap;
	typedef std::vector<size_t> candidates;
	typedef triangle_graph::node_iterator tri_iterator;
	typedef triangle_graph::const_node_iterator const_tri_iterator;
	typedef triangle_graph::out_arc_iterator link_iterator;
	typedef triangle_graph::const_out_arc_iterator const_link_iterator;

	void InitTriHeap();
	void Stripify();
	void AddLeftTriangles();
	void ResetStripIDs();

	detail::strip FindBestStrip();
	detail::strip ExtendToStrip(size_t Start, detail::triangle_order Order);
	detail::strip BackExtendToStrip(size_t Start, detail::triangle_order Order, bool ClockWise);
	const_link_iterator LinkToNeighbour(const_tri_iterator Node, bool ClockWise, detail::triangle_order & Order, bool NotSimulation);
	const_link_iterator BackLinkToNeighbour(const_tri_iterator Node, bool ClockWise, detail::triangle_order & Order);
	void BuildStrip(const detail::strip Strip);
	void MarkTriAsTaken(size_t i);
	void AddIndex(index i, bool NotSimulation);
	void BackAddIndex(index i);
	void AddTriangle(const detail::triangle & Tri, detail::triangle_order Order, bool NotSimulation);
	void BackAddTriangle(const detail::triangle & Tri, detail::triangle_order Order);

	bool Cache() const;
	size_t CacheSize() const;

	static detail::triangle_edge FirstEdge(const detail::triangle & Triangle, detail::triangle_order Order);
	static detail::triangle_edge LastEdge(const detail::triangle & Triangle, detail::triangle_order Order);

	primitive_vector			m_PrimitivesVector;
	triangle_graph				m_Triangles;
	triangle_heap				m_TriHeap;
	candidates					m_Candidates;
	detail::cache_simulator		m_Cache;
	detail::cache_simulator		m_BackCache;
	size_t						m_StripID;
	size_t						m_MinStripSize;
	bool						m_BackwardSearch;
	bool						m_FirstRun;
};









inline void tri_stripper::SetCacheSize(const size_t CacheSize)
{
	m_Cache.resize(CacheSize);
	m_BackCache.resize(CacheSize);
}


inline void tri_stripper::SetMinStripSize(const size_t MinStripSize)
{
	if (MinStripSize < 2)
		m_MinStripSize = 2;
	else
		m_MinStripSize = MinStripSize;
}


inline void tri_stripper::SetBackwardSearch(const bool Enabled)
{
	m_BackwardSearch = Enabled;
}



inline void tri_stripper::SetPushCacheHits(bool Enabled)
{
	m_Cache.push_cache_hits(Enabled);
}




} 




#endif 




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