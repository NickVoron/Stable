#pragma once

#include <vector>

#include "vertex.h"

#include "memory/library.include.h"
#include "containers/alignedArray.h"

namespace Geometry
{

template<class IndexType> struct Face;
template<class VertexType> struct Triangle;

template<class Vertex_>
class VertexStreamBase
{
public:
	static const bool INDEXED_DATA = false;

	typedef Vertex_ Vertex;
	typedef std::pair<Vertex*, unsigned int> VertexPair;

	VertexStreamBase():vertices(0), sz(0){};
	VertexStreamBase(void* data, std::size_t verticesCount) : sz(verticesCount), vertices((Vertex*)data){}

	Vertex& operator[](std::size_t i);
	const Vertex& operator[](std::size_t i) const;

	auto begin() { return vertices; }
	auto end() { return vertices + sz; }

	auto begin() const { return vertices; }
	auto end() const { return vertices + sz; }


	auto size() const	{ return sz; }
	auto memory() const	{ return size() * sizeof(Vertex);}

	template<class IndexType>
	Triangle<Vertex> triangle( const Face<IndexType>& f ) const
	{
		return Triangle<Vertex>(vertices[f.a], vertices[f.b], vertices[f.c]);
	}

protected:
	Vertex* vertices;
	std::size_t sz;
};

template<class Vertex_>
class VertexStream : public VertexStreamBase<Vertex_>
{
public:
	typedef Vertex_ Vertex;
	VertexStream(){}
	VertexStream(const VertexStream<Vertex>& vs);
	~VertexStream();

	void clear();

	//
	std::size_t addVertex(Vertex** addedVertex);
	std::size_t addVertex(const Vertex& v);

	Vertex& getLastVertex();

	void addVertices(Vertex** addedVertices, std::size_t count);
	void addVertices(const Vertex* v, std::size_t count);

	VertexStream<Vertex>& operator=(const VertexStream<Vertex>& vs);

	template<class SelectedList, class Vertex2>
	void assign(const VertexStream<Vertex2>& vs2);

	template<class Vertex2>
	void maxAssign(const VertexStream<Vertex2>& vs2);

	void resize(std::size_t s);
	void reserve(std::size_t s);
	std::size_t capacity() const;

private:
	void update();
	AlignedArray<Vertex, 16> data;
};

//
//
//
template<class Vertex>
Vertex& VertexStreamBase<Vertex>::operator[](std::size_t i)
{
	return vertices[i];
}

template<class Vertex>
const Vertex& VertexStreamBase<Vertex>::operator[](std::size_t i) const
{
	return vertices[i];
}

//
//
//
template<class Vertex>
void VertexStream<Vertex>::update()
{
	this->vertices = data.size() > 0 ? &data[0] : 0;
	this->sz = data.size();
}

template<class Vertex>
VertexStream<Vertex>::~VertexStream()
{ 
	clear();
}

template<class Vertex>
void VertexStream<Vertex>::clear()
{
	data.clear();
	update();
}

template<class Vertex>
VertexStream<Vertex>::VertexStream(const VertexStream<Vertex>& vs)
{
	operator=(vs);
};

template<class Vertex>
std::size_t VertexStream<Vertex>::capacity() const
{
	return data.capacity();
}

template<class Vertex>
std::size_t VertexStream<Vertex>::addVertex(Vertex** addedVertex)
{
	auto index = size();
	resize(size() + 1);
	if(addedVertex)	*addedVertex = &vertices[index];				
	return index;
}

template<class Vertex>
std::size_t VertexStream<Vertex>::addVertex(const Vertex& v)
{
	std::size_t index = size();
	resize(size() + 1);

	vertices[index] = v;
	return index;
}

template<class Vertex>
Vertex& VertexStream<Vertex>::getLastVertex() {return vertices[size - 1];}

template<class Vertex>
void VertexStream<Vertex>::addVertices(Vertex** addedVertices, std::size_t count)
{
	resize(size() + count);
	if (addedVertices)
	{
		*addedVertices = &vertices[size()];
	}
}

template<class Vertex>
void VertexStream<Vertex>::addVertices(const Vertex* v, std::size_t count)
{
	auto csz = this->size();
	resize(csz + count);
	mem::memcpy(&this->vertices[csz], v, count * sizeof(Vertex));
}

template<class Vertex>
VertexStream<Vertex>& VertexStream<Vertex>::operator=(const VertexStream<Vertex>& vs)
{
	auto s = vs.size();
	resize(s);
	if(s > 0)
	{
		mem::memcpy(this->vertices, vs.vertices, this->memory());
	}
	return *this;
}

template<class Vertex>
void VertexStream<Vertex>::resize(std::size_t s)
{
	data.resize(s);
	update();
}

template<class Vertex>
void VertexStream<Vertex>::reserve(std::size_t s)
{
	data.reserve(s);
}

template<class Vertex>
template<class SelectedList, class Vertex2>
void VertexStream<Vertex>::assign(const VertexStream<Vertex2>& vs2)
{
	resize(vs2.size());

	for (std::size_t i = 0; i < vs2.size(); ++i)
	{
		vertices[i].assign<SelectedList>( vs2[i] );
	}
}

template<class Vertex>
template<class Vertex2>
void VertexStream<Vertex>::maxAssign(const VertexStream<Vertex2>& vs2)
{
	resize(vs2.size());

	for (std::size_t i = 0; i < vs2.size(); ++i)
	{
		this->vertices[i].maxAssign( vs2[i] );
	}
}


//
template<class VS, class POS>
typename VS::VertexPair findVertex(const VS& vs, const POS& p, float threshold)
{
	float thresholdSq = threshold*threshold;

	auto s = vertices.size();
	for(unsigned int i = 0; i < s; ++i)
	{
		float distSq = (vertices[i].get<POS>() - p).magnitudeSq();
		if(distSq <= thresholdSq)
		{
			return std::make_pair(&vertices[i], i);
		}
	}

	return std::make_pair((Vertex*)0, 7777777);
}

}