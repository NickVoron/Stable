








#include "tri_stripper.h"

#include "detail/connectivity_graph.h"
#include "detail/policy.h"

#include <cassert>




namespace triangle_stripper {

	using namespace detail;




tri_stripper::tri_stripper(const indices & TriIndices)
	: m_Triangles(TriIndices.size() / 3), 
	  m_StripID(0),
	  m_FirstRun(true)
{
	SetCacheSize();
	SetMinStripSize();
	SetBackwardSearch();
	SetPushCacheHits();

	make_connectivity_graph(m_Triangles, TriIndices);
}



void tri_stripper::Strip(primitive_vector * out_pPrimitivesVector)
{
	assert(out_pPrimitivesVector);

	if (! m_FirstRun) {
		unmark_nodes(m_Triangles);
		ResetStripIDs();
		m_Cache.reset();
		m_TriHeap.clear();
		m_Candidates.clear();
		m_StripID = 0;

		m_FirstRun = false;
	}

	out_pPrimitivesVector->clear();

	InitTriHeap();

	Stripify();
	AddLeftTriangles();
	
	std::swap(m_PrimitivesVector, (* out_pPrimitivesVector));
}



void tri_stripper::InitTriHeap()
{
	m_TriHeap.reserve(m_Triangles.size());

	
	
	for (size_t i = 0; i < m_Triangles.size(); ++i)
		m_TriHeap.push(m_Triangles[i].out_size());

	
	m_TriHeap.lock();

	
	
	while ((! m_TriHeap.empty()) && (m_TriHeap.top() == 0))
		m_TriHeap.pop();
}



void tri_stripper::ResetStripIDs()
{
	for (triangle_graph::node_iterator it = m_Triangles.begin(); it != m_Triangles.end(); ++it)
		(**it).ResetStripID();
}



void tri_stripper::Stripify()
{
	while (! m_TriHeap.empty()) {

		
		const size_t HeapTop = m_TriHeap.position(0);
		m_Candidates.push_back(HeapTop);

		while (! m_Candidates.empty()) {

			
			const strip TriStrip = FindBestStrip();

			if (TriStrip.Size() >= m_MinStripSize)
				BuildStrip(TriStrip);
		}

		if (! m_TriHeap.removed(HeapTop))
			m_TriHeap.erase(HeapTop);

		
		while ((! m_TriHeap.empty()) && (m_TriHeap.top() == 0))
			m_TriHeap.pop();
	}
}



inline strip tri_stripper::FindBestStrip()
{
	
	const cache_simulator CacheBackup = m_Cache;

	policy Policy(m_MinStripSize, Cache());

	while (! m_Candidates.empty()) {

		const size_t Candidate = m_Candidates.back();
		m_Candidates.pop_back();

		
		if ((m_Triangles[Candidate].marked()) || (m_TriHeap[Candidate] == 0))
			continue;		

		
		for (size_t i = 0; i < 3; ++i) {

			const strip Strip = ExtendToStrip(Candidate, triangle_order(i));
			Policy.Challenge(Strip, m_TriHeap[Strip.Start()], m_Cache.hitcount());
			
			m_Cache = CacheBackup;
		}

		
		if (m_BackwardSearch) {

			for (size_t i = 0; i < 3; ++i) {

				const strip Strip = BackExtendToStrip(Candidate, triangle_order(i), false);
				Policy.Challenge(Strip, m_TriHeap[Strip.Start()], m_Cache.hitcount());
			
				m_Cache = CacheBackup;
			}

			for (size_t i = 0; i < 3; ++i) {

				const strip Strip = BackExtendToStrip(Candidate, triangle_order(i), true);
				Policy.Challenge(Strip, m_TriHeap[Strip.Start()], m_Cache.hitcount());
			
				m_Cache = CacheBackup;
			}
		}

	}

	return Policy.BestStrip();
}



strip tri_stripper::ExtendToStrip(const size_t Start, triangle_order Order)
{
	const triangle_order StartOrder = Order;
	
	
	m_Triangles[Start]->SetStripID(++m_StripID);
	AddTriangle(* m_Triangles[Start], Order, false);

	size_t Size = 1;
	bool ClockWise = false;

	
	for (tri_iterator Node = (m_Triangles.begin() + Start); 
		(Node != m_Triangles.end()) && (!Cache() || ((Size + 2) < CacheSize()));
		++Size) {

		const const_link_iterator Link = LinkToNeighbour(Node, ClockWise, Order, false);

		
		if (Link == Node->out_end()) {

			Node = m_Triangles.end();
			--Size;

		} else {

			Node = Link->terminal();
			(* Node)->SetStripID(m_StripID);
			ClockWise = ! ClockWise;

		}
	}

	return strip(Start, StartOrder, Size);
}



strip tri_stripper::BackExtendToStrip(size_t Start, triangle_order Order, bool ClockWise)
{
	
	m_Triangles[Start]->SetStripID(++m_StripID);
	BackAddIndex(LastEdge(* m_Triangles[Start], Order).B());
	size_t Size = 1;

	tri_iterator Node;

	
	for (Node = (m_Triangles.begin() + Start); 
		!Cache() || ((Size + 2) < CacheSize());
		++Size) {

		const const_link_iterator Link = BackLinkToNeighbour(Node, ClockWise, Order);

		
		if (Link == Node->out_end())
			break;

		else {
			Node = Link->terminal();
			(* Node)->SetStripID(m_StripID);
			ClockWise = ! ClockWise;
		}
	}

	
	
	
	
	if (ClockWise)
		return strip();

	if (Cache()) {
		m_Cache.merge(m_BackCache, Size);
		m_BackCache.reset();
	}

	return strip(Node - m_Triangles.begin(), Order, Size);
}



void tri_stripper::BuildStrip(const strip Strip)
{
	const size_t Start = Strip.Start();

	bool ClockWise = false;
	triangle_order Order = Strip.Order();

	
	m_PrimitivesVector.push_back(primitive_group());
	m_PrimitivesVector.back().Type = TRIANGLE_STRIP;
	AddTriangle(* m_Triangles[Start], Order, true);
	MarkTriAsTaken(Start);

	
	tri_iterator Node = (m_Triangles.begin() + Start);

	for (size_t Size = 1; Size < Strip.Size(); ++Size) {

		const const_link_iterator Link = LinkToNeighbour(Node, ClockWise, Order, true);

		assert(Link != Node->out_end());

		
		Node = Link->terminal();
		MarkTriAsTaken(Node - m_Triangles.begin());
		ClockWise = ! ClockWise;
	}
}



inline tri_stripper::const_link_iterator tri_stripper::LinkToNeighbour(const const_tri_iterator Node, const bool ClockWise, triangle_order & Order, const bool NotSimulation)
{
	const triangle_edge Edge = LastEdge(** Node, Order);

	for (const_link_iterator Link = Node->out_begin(); Link != Node->out_end(); ++Link) {

		
		const triangle & Tri = ** Link->terminal();

		
		if (NotSimulation || (Tri.StripID() != m_StripID)) {

			if (! Link->terminal()->marked()) {

				

				if ((Edge.B() == Tri.A()) && (Edge.A() == Tri.B())) {
					Order = (ClockWise) ? ABC : BCA;
					AddIndex(Tri.C(), NotSimulation);
					return Link;
				}

				else if ((Edge.B() == Tri.B()) && (Edge.A() == Tri.C())) {
					Order = (ClockWise) ? BCA : CAB;
					AddIndex(Tri.A(), NotSimulation);
					return Link;
				}

				else if ((Edge.B() == Tri.C()) && (Edge.A() == Tri.A())) {
					Order = (ClockWise) ? CAB : ABC;
					AddIndex(Tri.B(), NotSimulation);
					return Link;
				}
			}
		}

	}

	return Node->out_end();
}



inline tri_stripper::const_link_iterator tri_stripper::BackLinkToNeighbour(const_tri_iterator Node, bool ClockWise, triangle_order & Order)
{
	const triangle_edge Edge = FirstEdge(** Node, Order);

	for (const_link_iterator Link = Node->out_begin(); Link != Node->out_end(); ++Link) {

		
		const triangle & Tri = ** Link->terminal();

		
		if ((Tri.StripID() != m_StripID) && ! Link->terminal()->marked()) {

			

			if ((Edge.B() == Tri.A()) && (Edge.A() == Tri.B())) {
				Order = (ClockWise) ? CAB : BCA;
				BackAddIndex(Tri.C());
				return Link;
			}

			else if ((Edge.B() == Tri.B()) && (Edge.A() == Tri.C())) {
				Order = (ClockWise) ? ABC : CAB;
				BackAddIndex(Tri.A());
				return Link;
			}

			else if ((Edge.B() == Tri.C()) && (Edge.A() == Tri.A())) {
				Order = (ClockWise) ? BCA : ABC;
				BackAddIndex(Tri.B());
				return Link;
			}
		}

	}

	return Node->out_end();
}



void tri_stripper::MarkTriAsTaken(const size_t i)
{
	typedef triangle_graph::node_iterator tri_node_iter;
	typedef triangle_graph::out_arc_iterator tri_link_iter;

	
	m_Triangles[i].mark();

	
	if (! m_TriHeap.removed(i))
		m_TriHeap.erase(i);

	
	for (tri_link_iter Link = m_Triangles[i].out_begin(); Link != m_Triangles[i].out_end(); ++Link) {

		const size_t j = Link->terminal() - m_Triangles.begin();

		if ((! m_Triangles[j].marked()) && (! m_TriHeap.removed(j))) {
			size_t NewDegree = m_TriHeap.peek(j);
			NewDegree = NewDegree - 1;
			m_TriHeap.update(j, NewDegree);

			
			if (Cache() && (NewDegree > 0))
				m_Candidates.push_back(j);
		}
	}
}



inline triangle_edge tri_stripper::FirstEdge(const triangle & Triangle, const triangle_order Order)
{
	switch (Order)
	{
	case ABC:
		return triangle_edge(Triangle.A(), Triangle.B());

	case BCA:
		return triangle_edge(Triangle.B(), Triangle.C());

	case CAB:
		return triangle_edge(Triangle.C(), Triangle.A());

	default:
		assert(false);
		return triangle_edge(0, 0);
	}
}



inline triangle_edge tri_stripper::LastEdge(const triangle & Triangle, const triangle_order Order)
{
	switch (Order)
	{
	case ABC:
		return triangle_edge(Triangle.B(), Triangle.C());

	case BCA:
		return triangle_edge(Triangle.C(), Triangle.A());

	case CAB:
		return triangle_edge(Triangle.A(), Triangle.B());

	default:
		assert(false);
		return triangle_edge(0, 0);
	}
}



inline void tri_stripper::AddIndex(const index i, const bool NotSimulation)
{
	if (Cache())
		m_Cache.push(i, ! NotSimulation);

	if (NotSimulation)
		m_PrimitivesVector.back().Indices.push_back(i);
}



inline void tri_stripper::BackAddIndex(const index i)
{
	if (Cache())
		m_BackCache.push(i, true);
}



inline void tri_stripper::AddTriangle(const triangle & Tri, const triangle_order Order, const bool NotSimulation)
{
	switch (Order)
	{
	case ABC:
		AddIndex(Tri.A(), NotSimulation);
		AddIndex(Tri.B(), NotSimulation);
		AddIndex(Tri.C(), NotSimulation);
		break;

	case BCA:
		AddIndex(Tri.B(), NotSimulation);
		AddIndex(Tri.C(), NotSimulation);
		AddIndex(Tri.A(), NotSimulation);
		break;

	case CAB:
		AddIndex(Tri.C(), NotSimulation);
		AddIndex(Tri.A(), NotSimulation);
		AddIndex(Tri.B(), NotSimulation);
		break;
	}
}



inline void tri_stripper::BackAddTriangle(const triangle & Tri, const triangle_order Order)
{
	switch (Order)
	{
	case ABC:
		BackAddIndex(Tri.C());
		BackAddIndex(Tri.B());
		BackAddIndex(Tri.A());
		break;

	case BCA:
		BackAddIndex(Tri.A());
		BackAddIndex(Tri.C());
		BackAddIndex(Tri.B());
		break;

	case CAB:
		BackAddIndex(Tri.B());
		BackAddIndex(Tri.A());
		BackAddIndex(Tri.C());
		break;
	}
}



void tri_stripper::AddLeftTriangles()
{
	
	primitive_group Primitives;
	Primitives.Type = TRIANGLES;
	m_PrimitivesVector.push_back(Primitives);
	indices & Indices = m_PrimitivesVector.back().Indices;

	for (size_t i = 0; i < m_Triangles.size(); ++i)
		if (! m_Triangles[i].marked()) {
			Indices.push_back(m_Triangles[i]->A());
			Indices.push_back(m_Triangles[i]->B());
			Indices.push_back(m_Triangles[i]->C());
		}

	
	if (Indices.size() == 0)
		m_PrimitivesVector.pop_back();
}



inline bool tri_stripper::Cache() const
{
	return (m_Cache.size() != 0);
}



inline size_t tri_stripper::CacheSize() const
{
	return m_Cache.size();
}




} 
