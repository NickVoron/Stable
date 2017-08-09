// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vertexStream.h"
#include "indexStream.h"

namespace Geometry
{


template<class Vertex> struct TrianglePtr;
template<class Vertex> struct Triangle;

template<class Vertex>
struct Triangle
{
	Triangle(){}
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c)
	{
		v[0] = a;
		v[1] = b;
		v[2] = c;
	}

	Triangle( const TrianglePtr<Vertex>& t )
	{
		v[0] = t[0];
		v[1] = t[1];
		v[2] = t[2];
	}

	inline Vertex& operator[](unsigned int idx){ return v[idx]; }
	inline const Vertex& operator[](unsigned int idx) const{ return v[idx]; }


	Vertex v[3];
};


template<class Vertex>
struct TrianglePtr
{
	TrianglePtr() { v[0] = v[1] = v[2] = 0; }
	TrianglePtr(Vertex& a, Vertex& b, Vertex& c)
	{
		v[0] = &a;
		v[1] = &b;
		v[2] = &c;
	}

	TrianglePtr( Triangle<Vertex>& t )
	{
		v[0] = &t[0];
		v[1] = &t[1];
		v[2] = &t[2];
	}

	inline Vertex& operator[](unsigned int idx){ return *v[idx]; }
	inline const Vertex& operator[](unsigned int idx) const{ return *v[idx]; }


	Vertex* v[3];
};

template< template <class> class TrgType, class VertexT>
float area(const TrgType<VertexT>& t);

template<class VertexT>
float area(const Triangle<VertexT>& t)
{
	const POS3& v0 = t.v[0].pos3();
	const POS3& v1 = t.v[1].pos3();
	const POS3& v2 = t.v[2].pos3();

	return Base::MathUtils::area3D(v0, v1, v2);
}

template<class VertexT>
float area(const TrianglePtr<VertexT>& t)
{
	const POS3& v0 = t.v[0]->pos3();
	const POS3& v1 = t.v[1]->pos3();
	const POS3& v2 = t.v[2]->pos3();

	return Base::MathUtils::area3D(v0, v1, v2);
}

template< template <class> class TrgType, class VertexT>
NORMAL normal(const TrgType<VertexT>& t);

template<class VertexT>
NORMAL normal(const Triangle<VertexT>& t)
{
	const POS3& v0 = t.v[0].pos3();
	const POS3& v1 = t.v[1].pos3();
	const POS3& v2 = t.v[2].pos3();

	return Base::MathUtils::calculateNormal(v0, v1, v2);
}

template<class VertexT>
NORMAL normal(const TrianglePtr<VertexT>& t)
{
	const POS3& v0 = t.v[0]->pos3();
	const POS3& v1 = t.v[1]->pos3();
	const POS3& v2 = t.v[2]->pos3();

	return Base::MathUtils::calculateNormal(v0, v1, v2);
}

template< template <class> class TrgType, class VertexT>
bool isDegenerate(const TrgType<VertexT>& t);

template<class VertexT>
bool isDegenerate(const TrianglePtr<VertexT>& t)
{
	return Geometry::area(t) < 1e-10;
}

template<class VertexT>
bool isDegenerate(const Triangle<VertexT>& t)
{
	return Geometry::area(t) < 1e-10;
}






























template<class Vertex_, class IndexType_ = unsigned int>
class Mesh
{
public:
	static const bool INDEXED_DATA = true;

	typedef IndexType_				IndexType;
	typedef Vertex_					Vertex;

	typedef Triangle<Vertex>		Triangle;
	typedef TrianglePtr<Vertex>		TrianglePtr;
	typedef VertexStream<Vertex>	VertexStreamType;
	typedef IndexStream<IndexType>	IndexStreamType;

	Mesh(){};
	Mesh(const Mesh& m):vs(m.vs), is(m.is){};

	void getTriangle(int faceIdx, Triangle& trg) const
	{
		int base = faceIdx*3;
		trg[0] = vs[ is[ base + 0 ] ];
		trg[1] = vs[ is[ base + 1 ] ];
		trg[2] = vs[ is[ base + 2 ] ];
	}

	void getTrianglePtr(int faceIdx, TrianglePtr& trg)
	{
		int base = faceIdx*3;
		trg.v[0] = &vs[ is[ base + 0 ] ];
		trg.v[1] = &vs[ is[ base + 1 ] ];
		trg.v[2] = &vs[ is[ base + 2 ] ];
	}

















	
	VertexStreamType&	vertices(){return vs;}
	IndexStreamType& indices(){return is;}
	const VertexStreamType& vertices()const{return vs;}
	const IndexStreamType& indices()const{return is;}

	inline void clear()
	{
		vs.clear();
		is.clear();
	}

	inline bool empty() const
	{
		return is.size() == 0;
	}

	template<class SelectedVertexElems, class SMesh>
	inline void assign(const SMesh& smesh)
	{
		vs.assign<SelectedVertexElems>(smesh.vertices());
		is.assign( smesh.indices() );
	}

	template<class SMesh>
	inline void maxAssign(const SMesh& smesh)
	{
		vs.maxAssign(smesh.vertices());
		is.assign( smesh.indices() );
	}

	Mesh& operator=(const Mesh& mesh)
	{
		vs = mesh.vs;
		is = mesh.is;

		return *this;
	}

	std::size_t getFacesCount() const	{ return is.size() / 3; }
	std::size_t getVerticesCount() const { return vs.size(); }

	std::size_t memory() const { return vs.memory() + is.memory(); }

private:

	VertexStreamType vs;
	IndexStreamType is;
};


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