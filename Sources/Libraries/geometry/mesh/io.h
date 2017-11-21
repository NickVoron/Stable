#pragma once

#include "stream/library.include.h"
#include "mesh.h"

namespace Geometry
{

//VertexStream
template<class V>
stream::ostream& operator <<(stream::ostream& s, const Geometry::VertexStream<V>& vs)
{
	std::size_t size = vs.size(); s << size;
	if(size > 0)
	{
		s.write(&vs[0], vs.memory());
	}
	
	return s;
}

template<class V>
stream::istream& operator >>(stream::istream& s, Geometry::VertexStream<V>& vs)
{
	std::size_t size; s >> size;
	vs.resize(size);
	if(size > 0) 
	{
		s.read(&vs[0], vs.memory());
	}

	return s;
}

//IndexStream
template<class I>
stream::ostream& operator <<(stream::ostream& s, const Geometry::IndexStream<I>& is)
{
	std::size_t size = is.size(); s << size;
	if(size > 0)
	{
		s.write(&is[0], is.memory());
	}
	
	s << is.getMaxIndexValue();
	return s;
}

template<class I>
stream::istream& operator >>(stream::istream& s, Geometry::IndexStream<I> & is)
{
	std::size_t size; s >> size;
	is.resize(size);
	if(size > 0) 
	{
		s.read(&is[0], is.memory());
	}

	int maxIdxVal; s >> maxIdxVal;
	is.setMaxValue(maxIdxVal);
	return s;
}

//Mesh
template<class V, class I> stream::ostream& operator <<(stream::ostream& s, const Geometry::Mesh<V, I>& m)	{	return s << m.vertices() << m.indices();	}
template<class V, class I> stream::istream& operator >>(stream::istream& s, Geometry::Mesh<V, I>& m)		{	return s >> m.vertices() >> m.indices();	}
}
