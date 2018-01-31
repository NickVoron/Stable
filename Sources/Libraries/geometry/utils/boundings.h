// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "intersections/library.include.h"

#ifdef USE_WINDOWS
#include <d3dx9.h>
#endif

namespace Geometry
{

	template<class Vertex> inline Vector3 extractPos(const Vertex& vtx) { return Vector3(vtx.pos3().x, vtx.pos3().y, vtx.pos3().z); }
	template<> inline Vector3 extractPos(const Vector3& vtx) { return vtx; }

	template<class Vertex>
	void computeBoundBox(const Vertex* vertices, std::size_t count, intersections::AABB& bb)
	{
		if (count > 0)
		{
			auto min = extractPos(vertices[0]);
			auto max = min;

			for (std::size_t i = 1; i < count; ++i)
			{
				auto pos = extractPos(vertices[i]);

				if (min.x > pos.x) min.x = pos.x;
				if (min.y > pos.y) min.y = pos.y;
				if (min.z > pos.z) min.z = pos.z;

				if (max.x < pos.x) max.x = pos.x;
				if (max.y < pos.y) max.y = pos.y;
				if (max.z < pos.z) max.z = pos.z;
			}

			bb.bmin.x = min.x;
			bb.bmin.y = min.y;
			bb.bmin.z = min.z;

			bb.bmax.x = max.x;
			bb.bmax.y = max.y;
			bb.bmax.z = max.z;
		}
	}


template<class Vertex, class Index>
void computeBoundBox(const VertexStream<Vertex>& vertices, const IndexStream<Index>& is, intersections::AABB& bb)
{
	if (is.size() > 0)
	{
		POS3 min = vertices[ is[0] ].pos3();
		POS3 max = vertices[ is[0] ].pos3();

		for (std::size_t i = 1; i < is.size(); ++i)
		{
			const POS3& pos = vertices[ is[i] ].pos3();

			if (min.x > pos.x) min.x = pos.x;
			if (min.y > pos.y) min.y = pos.y;
			if (min.z > pos.z) min.z = pos.z;

			if (max.x < pos.x) max.x = pos.x;
			if (max.y < pos.y) max.y = pos.y;
			if (max.z < pos.z) max.z = pos.z;
		}

		bb.bmin.x = min.x;
		bb.bmin.y = min.y;
		bb.bmin.z = min.z;

		bb.bmax.x = max.x;
		bb.bmax.y = max.y;
		bb.bmax.z = max.z;
	}
}

template<class Vertex, class Index>
void computeBoundBox(const VertexStream<Vertex>& vertices, const IndexStream<Index>& is, const nm::matrix4& transform, intersections::AABB& bb)
{
#ifdef USE_WINDOWS
	std::size_t newSize = is.size();

	if(newSize > 0)
	{
		D3DXVECTOR3 min, max;
		Base::MathUtils::ConvertVectors3(min, vertices[ is[0] ].pos3());
		Base::MathUtils::ConvertVectors3(max, vertices[ is[0] ].pos3());
		D3DXVec3TransformCoord(&min, &min, &transform);
		D3DXVec3TransformCoord(&max, &max, &transform);

		for (std::size_t i = 1; i < newSize; ++i)
		{
			D3DXVECTOR3 pos;
			Base::MathUtils::ConvertVectors3(pos, vertices[ is[i] ].pos3());
			D3DXVec3TransformCoord(&pos, &pos, &transform);
			
			min.x = std::min(min.x, pos.x);
			min.y = std::min(min.y, pos.y);
			min.z = std::min(min.z, pos.z);

			max.x = std::max(max.x, pos.x);
			max.y = std::max(max.y, pos.y);
			max.z = std::max(max.z, pos.z);
		}

		Base::MathUtils::ConvertVectors3(bb.bmin, min);
		Base::MathUtils::ConvertVectors3(bb.bmax, max);
	}
#endif
}

template<class Vertex, class Index>
void computeBoundSphere(const VertexStream<Vertex>& vertices, const IndexStream<Index>& is, const nm::matrix4& transform, intersections::Sphere& bs)
{
 	intersections::Sphere s0;
 
  	intersections::AABB aabb;
  	computeBoundBox(vertices, is, transform, aabb);
  	s0.center = aabb.center();
  	s0.radius = nm::magnitude(aabb.size()) * 0.5f;
}


template<class Vertex, class Index>
void computeBoundSphere(const VertexStream<Vertex>& vertices, const IndexStream<Index>& is, intersections::Sphere& bs)
{
#ifdef USE_WINDOWS
	D3DXMATRIX identitiTransform;
	dxmath::FillDXMatrix(identitiTransform, State::Identity(), 1);
	computeBoundSphere(vertices, is, identitiTransform, bs);
#endif
}

template<class Vertex>
void computeBoundSphere(const VertexStream<Vertex>& vertices, intersections::Sphere& bs)
{
	intersections::Sphere s0;
	intersections::AABB aabb;
	computeBoundBox(&vertices[0], vertices.size(), aabb);
	s0.center = aabb.center();
	s0.radius = nm::magnitude(aabb.size()) * 0.5f;
}

inline void mergeBoundSpheres(const intersections::Sphere& s1, const intersections::Sphere& s2, intersections::Sphere& bs)
{

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