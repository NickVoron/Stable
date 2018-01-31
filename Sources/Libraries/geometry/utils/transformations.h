// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../mesh/mesh.h"
#include "math/library.include.h"

namespace Geometry {
namespace Transformations {

inline void transform(POS3& pos, const Matrix4& m)
{
	pos = (*((Vector3*)&pos.x)) * m;
}

inline void translate(POS3& pos, const Vector3& t)
{
	pos.x += t.x;
	pos.y += t.y;
	pos.z += t.z;
}

inline void rotate(POS3& pos, const Vector3& r)
{
	Matrix4 m;
	m.Identity();
	m.Rotate(r);

	transform(pos, m);
}

inline void scale(POS3& pos, const Vector3& s)
{
	pos.x *= s.x;
	pos.y *= s.y;
	pos.z *= s.z;
}


template<class Vertex>
inline void transform(Vertex& v, const Matrix4& m, const Matrix3& m3)
{
	auto& p = v.pos3();
	auto& n = v.normal();
	auto& t = v.tangent();

	p = m	* (*((Vector3*) &p.x));
	n = m3	* (*((Vector3*) &n.x));
	t = m3	* (*((Vector3*) &t.x));
}


template<class Vertex>
inline void transform(VertexStream<Vertex>& vs, const Matrix4& m)
{
	Matrix3 m3;	m.GetMatrix3(m3);

	for(auto& v : vs)
	{
		transform(v, m, m3);
	}
}

template<class Vertex>
inline void transform(Vertex& v, const Matrix4& m)
{
	Matrix3 m3;	m.GetMatrix3(m3);

	transform(v, m, m3);
}




template<class Vertex>
inline void translate(VertexStream<Vertex>& vs, const Vector3& t)
{
	unsigned int size = vs.size();
	for(unsigned int i = 0; i < size; ++i)
	{
		translate(vs[i].pos3(), t);		
	}
}

template<class Vertex>
inline void rotate(VertexStream<Vertex>& vs, const Vector3& r)
{
	Matrix4 m;
	m.Identity();
	m.Rotate(r);

	transform(vs, m);
}

template<class Vertex>
inline void scale(VertexStream<Vertex>& vs, const Vector3& s)
{
	for (std::size_t i = 0; i < vs.size(); ++i)
	{
		scale(vs[i].pos3(), s);		
	}
}


template<class Vertex, class Index>
inline void transform(Mesh<Vertex, Index>& mesh, const Matrix4& m)
{
	transform(mesh.vertices(), m );
}

template<class Vertex, class Index>
inline void translate(Mesh<Vertex, Index>& mesh, const Vector3& t)
{
	translate(mesh.vertices(), t );
}

template<class Vertex, class Index>
inline void rotate(Mesh<Vertex, Index>& mesh, const Vector3& r)
{
	rotate(mesh.vertices(), r );
}

template<class Vertex, class Index>
inline void scale(Mesh<Vertex, Index>& mesh, const Vector3& s)
{
	scale(mesh.vertices(), s );
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