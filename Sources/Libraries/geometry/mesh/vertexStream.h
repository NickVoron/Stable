// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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
	auto index = this->size();
	resize(this->size() + 1);
	if(addedVertex)	*addedVertex = &this->vertices[index];
	return index;
}

template<class Vertex>
std::size_t VertexStream<Vertex>::addVertex(const Vertex& v)
{
	auto index = this->size();
	resize(index + 1);

	this->vertices[index] = v;
	return index;
}

template<class Vertex>
Vertex& VertexStream<Vertex>::getLastVertex() {return this->vertices[this->size() - 1];}

template<class Vertex>
void VertexStream<Vertex>::addVertices(Vertex** addedVertices, std::size_t count)
{
	resize(this->size() + count);
	if (addedVertices)
	{
		*addedVertices = &this->vertices[this->size()];
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
		this->vertices[i].template assign<SelectedList>( vs2[i] );
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



}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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