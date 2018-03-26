// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../mesh/mesh.h"

#include "intersections/library.include.h"

namespace Geometry
{

#undef min
#undef max

enum BoundCylinderType
{
	BCT_BOTTOM_ALIGNED,
};

template<class VertexType, class IndexType>
void evaluateAABB(intersections::AABB& res, const VertexStream<VertexType>& vs, IndexType i)
{
	auto& pos = vs[i].pos3();

	res.bmin.x = std::min(pos.x, res.bmin.x);
	res.bmin.y = std::min(pos.y, res.bmin.y);
	res.bmin.z = std::min(pos.z, res.bmin.z);

	res.bmax.x = std::max(pos.x, res.bmax.x);
	res.bmax.y = std::max(pos.y, res.bmax.y);
	res.bmax.z = std::max(pos.z, res.bmax.z);
}

template<class VertexType, class IndexType>
void evaluateAABB(intersections::AABB& res, const VertexStream<VertexType>& vs, const Face<IndexType>& f)
{
	evaluateAABB(res, vs, f.a);
	evaluateAABB(res, vs, f.b);
	evaluateAABB(res, vs, f.c);
}

struct BottomAlignedBoundCylinder
{
	BottomAlignedBoundCylinder():yMin(0.0f), yMax(0.0f), radius(0.0f){}
	float h() const {return yMax - yMin;}

	float yMin;
	float yMax;
	float radius;
};







inline void maxBoundCylinder(const BottomAlignedBoundCylinder& c1, const BottomAlignedBoundCylinder& c2, BottomAlignedBoundCylinder& res)
{
	res.yMax = std::max(c1.yMax, c2.yMax);
	res.yMin = std::min(c1.yMin, c2.yMin);
	res.radius = std::max(c1.radius, c2.radius);
}

template<class Vertex>
NORMAL calculateNormal(const Vertex& a, const Vertex& b, const Vertex& c)
{
	POS3 ac = c.pos3() - a.pos3();
	POS3 ab = b.pos3() - a.pos3();

	NORMAL n;
	nm::cross(ab, ac, n);
	n.normalize();
	return n;
}


template<class VertexType, class VertexComponent>
void getInterpolatedTrg(VertexComponent& val, const TrianglePtr<VertexType>& t, float u, float v)
{
	static_assert( (IsVertexHas<VertexType, VertexComponent>::value), "IsVertexHas<VertexType, VertexComponent>::value" );
	const VertexComponent& v0 = t.v[0]->template get<VertexComponent>();
	const VertexComponent& v1 = t.v[1]->template get<VertexComponent>();
	const VertexComponent& v2 = t.v[2]->template get<VertexComponent>();
	val = v0 + (v1 - v0)*u + (v2 - v0)*v;
}


template<class VectorT0, class VectorT1>
void getInterpolated(VectorT0& val, VectorT1* t, float u, float v)
{
	val = t[0] + (t[1] - t[0])*u + (t[2] - t[0])*v;
}

template<class VertexType>
void getInterpolatedVertex(VertexType& res, const VertexType& v1, const VertexType& v2, float d)
{

}


template<class VertexType, class IndexType>
unsigned int mergeVertices(Mesh<VertexType, IndexType>& mesh)
{
	typedef Mesh<VertexType, IndexType> Mesh;

	unsigned int result = 0;

	auto& vs = mesh.vertices();
	auto& is = mesh.indices();
	auto oldIs = is;

	std::vector<bool> removed;
	int vc = vs.getSize();

	removed.resize(vc);
	for(int i = 0; i < vc; ++i)
	{
		if(removed[i]) continue;

		for(int j = i + 1; j < vc; ++j)
		{
			if(removed[j]) continue;

			if( vs[i].equal(vs[j]) )
			{
				for(int k = 0; k < is.getSize(); ++k)
				{
					if(oldIs[k] == j) is[k] = i;
				}
				removed[j] = true;
				++result;				
			}
		}
	}

	oldIs = is;
	std::vector<typename Mesh::Vertex> newVs;
	std::vector<bool> added( vs.getSize() );
	for(int i = 0; i < is.getSize(); ++i)
	{
		int idxVal = oldIs[i];
		if(!added[ idxVal ])
		{
			added[idxVal] = true;
			newVs.push_back( vs[idxVal] );
			int newIdxVal = newVs.size() - 1;
			for(int j = 0; j < is.getSize(); ++j)
			{
				if(oldIs[j] == idxVal) is[j] = newIdxVal; 
			}
		}
	}

	vs.setSize(newVs.size());

	for(unsigned int i = 0; i < newVs.size(); ++i)
	{
		vs[i] = newVs[i];
	}
	return result;
}

template<class VertexType, class IndexType>
std::vector<std::vector<VertexType*>> equalVertices(Mesh<VertexType, IndexType>& mesh)
{
	auto& vs = mesh.vertices();
	auto size = vs.size();

	std::vector<bool> finded(size);
	std::vector<std::vector<VertexType*>> result(size);

	for (unsigned int i = 0; i < size; ++i)
	{
		if (!finded[i])
		{
			auto& vtx0 = vs[i];
			for (unsigned int j = i; j < size; ++j)
			{
				auto& vtx1 = vs[j];
				if (vtx1.pos3().equal(vtx0.pos3()))
				{
					finded[j] = true;
					result[i].push_back(&vtx1);
				}
			}
		}
	}

	return result;
}


template<class BasisVectorType, class VertexType, class IndexType>
void smoothBasis(Mesh<VertexType, IndexType>& mesh)
{
	for (auto& vertices : equalVertices(mesh))
	{
		if (!vertices.empty())
		{
			BasisVectorType result(0.0f, 0.0f, 0.0f);
			for (auto& vtx : vertices)
			{
				result += vtx->template get<BasisVectorType>();
			}

			result.normalize();

			for (auto& vtx : vertices)
			{
				vtx->template get<BasisVectorType>() = result;
			}
		}
	}
}

template<class VertexType, class IndexType>
void optimize(VertexStream<VertexType>& vs, IndexStream<IndexType>& is)
{















































































}

template<class VertexType, class IndexType>
void optimize(Mesh<VertexType, IndexType>& mesh)
{
	optimize(mesh.vertices(), mesh.indices());
}


template<class VertexType, class IndexType>
void generateNormals(VertexStream<VertexType>& vs, IndexStream<IndexType>& is)
{
	NORMAL zeroNormal = NORMAL(0.0f, 0.0f, 0.0f);

	auto verticesCount = vs.size();
	for(std::size_t i = 0; i < verticesCount; ++i)
	{
		vs[i].normal() = zeroNormal;	
	}

	auto facesCount = is.size() / 3;
	for(std::size_t i = 0; i < facesCount; ++i)
	{
		Face<IndexType> f = is.getFace(i);
		VertexType& a = vs[f.a];
		VertexType& b = vs[f.b];
		VertexType& c = vs[f.c];
		NORMAL n = calculateNormal(a, b, c);
		a.normal() += n;
		b.normal() += n;
		c.normal() += n;
		a.normal().normalize();
		b.normal().normalize();
		c.normal().normalize();
	}
}

template<class VertexType>
void invertNormals(VertexStream<VertexType>& vs)
{
	std::size_t verticesCount = vs.size();
	for (std::size_t i = 0; i < verticesCount; ++i)
	{
		vs[i].normal() *= -1;
	}
}

template<class Mesh>
void generateNormals(Mesh& mesh)
{
	generateNormals(mesh.vertices(), mesh.indices());
}

template<class VertexType, class IndexType>
void fillNormals(Mesh<VertexType, IndexType>& mesh, const NORMAL& n)
{
	NORMAL norm = n;
	auto verticesCount = mesh.getVerticesCount();
	auto& vs = mesh.vertices();
	for(std::size_t i = 0; i < verticesCount; ++i)
	{
		vs[i].normal() = norm;	
	}
}

template<int uvIndex, class VertexType, class UV2Type>
bool uvInTriangle(const UV2Type& uv, const Geometry::TrianglePtr<VertexType>& t)
{
	float s1 = nm::point2ToLineSegmentDistance(uv, t[0].template uv2<uvIndex>(), t[1].template uv2<uvIndex>());
	float s2 = nm::point2ToLineSegmentDistance(uv, t[1].template uv2<uvIndex>(), t[2].template uv2<uvIndex>());
	if(s2*s1 <= 0) return false;
	float s3 = nm::point2ToLineSegmentDistance(uv, t[2].template uv2<uvIndex>(), t[0].template uv2<uvIndex>());
	if(s3*s2 <= 0) return false;

	return true;
}

template<class VertexType, class IndexType>
void generateDoubleSided(Mesh<VertexType, IndexType>& mesh)
{
	mesh.mergeVertices(0.001f);
	auto fc = mesh.getFacesCount();
	auto& vs = mesh.vertices();

	IndexStream<IndexType> shuffledIs;

	for(unsigned int i = 0; i < fc; ++i)
	{
		auto face = mesh.getTriangle(i);
		shuffledIs.addIndex( vs.addVertex( vs[face.a] ) );
		shuffledIs.addIndex( vs.addVertex( vs[face.b] ) );
		shuffledIs.addIndex( vs.addVertex( vs[face.c] ) );
	}

	mesh.indices().addIndexStream(shuffledIs);
}

template<class VertexStream, class IndexStream>
void computeBoundCylinder(const VertexStream& vertices, const IndexStream& is, BottomAlignedBoundCylinder& bc)
{
	unsigned int newSize = is.size();
	if(newSize > 0)
	{
		POS3 pos0 = vertices[ is[0] ].pos3();

		float minY = pos0.y;
		float maxY = pos0.y;

		pos0.y = 0.0f;

		float radius = pos0.magnitudeSq();

		for (unsigned int i = 1; i < newSize; ++i)
		{
			POS3 pos = vertices[ is[i] ].pos3();

			minY = std::min(minY, pos.y);
			maxY = std::max(maxY, pos.y);

			pos.y = 0.0f;

			float lenght = pos.magnitudeSq();

			radius = std::max(radius, lenght);
		}

		bc.yMax = maxY;
		bc.yMin = minY;
		bc.radius = sqrtf(radius);		
	}
}
































































template<class VertexType, class IndexType>
void mergeMeshes(VertexStream<VertexType>& resVS, IndexStream<IndexType>& resIS, const VertexStream<VertexType>& addVS, const IndexStream<IndexType>& addIS)
{
	std::size_t baseVal = resVS.size();
	resVS.addVertices(&addVS[0], addVS.size());
	resIS.updateMaxValue();
	
	std::size_t start = resIS.size();
	std::size_t resISSize = start + addIS.size();
	resIS.resize(resISSize);
	
	for (std::size_t i = start, j = 0; i < resISSize; ++i, ++j)
	{
		resIS[i] = (unsigned int)(addIS[j] + baseVal);
	}

	resIS.updateMaxValue();
}

template<class VertexType, class IndexType>
void mergeMeshes(Mesh<VertexType, IndexType>& resMesh, const Mesh<VertexType, IndexType>& addMesh)
{
	mergeMeshes(resMesh.vertices(), resMesh.indices(), addMesh.vertices(), addMesh.indices() );
}




template<class VertexType, class IndexType>
void compact(const VertexStream<VertexType>& srcv, const IndexStream<IndexType>& srci, VertexStream<VertexType>& dstv, IndexStream<IndexType>& dsti)
{
	int mini = INT_MAX;
	int maxi = INT_MIN;

	for (int idx : srci)
	{
		if (mini > idx) mini = idx;
		if (maxi < idx) maxi = idx;
	}
		
	std::vector<int> used(maxi - mini + 1, -1);
	std::vector<int> diff(used.size(), 0);

	for (std::size_t i = 0; i < srci.size(); ++i)
	{
		used[srci[i] - mini] = srci[i];
	}	

	int vcount = 0;
	int currdiff = 0;
	for (std::size_t i = 0; i < used.size(); ++i)
	{
		(used[i] < 0) ? --currdiff : ++vcount;
		diff[i] = currdiff;
	}
	
	dstv.resize(vcount);
	dsti.resize(srci.size());
	
	for (std::size_t i = 0; i < srci.size(); ++i)
	{
		int di = srci[i] - mini;
		dsti[i] = di + diff[di];

		dstv[dsti[i]] = srcv[used[srci[i] - mini]];
	}
}




template<class IndexType>
void extract(const IndexStream<IndexType>& source, const std::vector<int>& triangles0, IndexStream<IndexType>& res)
{
	std::size_t targetSize = triangles0.size() * 3;

	ENFORCE(source.size() >= targetSize);

	res.resize(targetSize);
	for (std::size_t i = 0; i < triangles0.size(); ++i)
	{
		res[i * 3 + 0] = source[triangles0[i] * 3 + 0];
		res[i * 3 + 1] = source[triangles0[i] * 3 + 1];
		res[i * 3 + 2] = source[triangles0[i] * 3 + 2];
	}
}

template<class VertexType, class IndexType>
void extract(const VertexStream<VertexType>& srcv, const IndexStream<IndexType>& srci, const std::vector<int>& triangles, VertexStream<VertexType>& dstv, IndexStream<IndexType>& dsti)
{
	IndexStream<IndexType> extracted;
	extract(srci, triangles, extracted);
	compact(srcv, extracted, dstv, dsti);
}

template<class VertexType, class IndexType>
void extract(const Mesh<VertexType, IndexType>& srcm, const std::vector<int>& triangles, Mesh<VertexType, IndexType>& dstm)
{
	IndexStream<IndexType> extracted;
	extract(srcm.indices(), triangles, extracted);
	compact(srcm.vertices(), extracted, dstm.vertices(), dstm.indices());
}

template<class IndexType>
void split(const IndexStream<IndexType>& source, const std::vector<int>& triangles0, IndexStream<IndexType>& res0, IndexStream<IndexType>& res1)
{
	extract(source, triangles0, res0);

	int sz = source.size() / 3;
	std::vector<int> triangles1(sz);
	
	for (std::size_t i = 0; i < sz; ++i)
	{
		triangles1[i] = i;
	}

	for (std::size_t i = 0; i < triangles0.size(); ++i)
	{
		triangles1[triangles0[i]] = -1;
	}

	triangles1.erase(std::remove(triangles1.begin(), triangles1.end(), -1), triangles1.end());

	extract(source, triangles1, res1);
}


template<class VertexType, class IndexType>
void split(const VertexStream<VertexType>& srcv, const IndexStream<IndexType>& srci, const std::vector<int>& triangles, VertexStream<VertexType>& dstv0, IndexStream<IndexType>& dsti0, VertexStream<VertexType>& dstv1, IndexStream<IndexType>& dsti1)
{
	IndexStream<IndexType> is0;
	IndexStream<IndexType> is1;
	split(srci, triangles, is0, is1);

	compact(srcv, is0, dstv0, dsti0);
	compact(srcv, is1, dstv1, dsti1);
}

template<class VertexType, class IndexType>
void split(const Mesh<VertexType, IndexType>& srcm, const std::vector<int>& triangles, Mesh<VertexType, IndexType>& dstm0, Mesh<VertexType, IndexType>& dstm1)
{
	split(srcm.vertices(), srcm.indices(), triangles, dstm0.vertices(), dstm0.indices(), dstm1.vertices(), dstm1.indices());
}





template<class VertexType>
struct defaultUV2Extractor
{
	UV2<0> operator()(const VertexType& vtx) const
	{
		return vtx.template uv2<0>();
	}
};



template<class VertexType, class IndexType, template <class> class UV2Extractor>
struct computeTangentsC
{	
	computeTangentsC(VertexStream<VertexType>& vstr, IndexStream<IndexType> istr)
	{ 
		UV2Extractor<VertexType> extrator;

		std::size_t vertexCount = vstr.size();
		std::size_t count = vertexCount * 2;
		Vector3* tan1 = new Vector3[count];
		Vector3* tan2 = tan1 + vertexCount;
		mem::memset(tan1, 0, count * sizeof(Vector3));

		for(std::size_t a = 0; a < istr.size() / 3; ++a)
		{
			unsigned int i1 = istr[a*3+0];
			unsigned int i2 = istr[a*3+1];
			unsigned int i3 = istr[a*3+2];

			const POS3& v1 = vstr[i1].pos3();
			const POS3& v2 = vstr[i2].pos3();
			const POS3& v3 = vstr[i3].pos3();

			UV2<0> w1 = extrator(vstr[i1]);
			UV2<0> w2 = extrator(vstr[i2]);
			UV2<0> w3 = extrator(vstr[i3]);

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.u - w1.u;
			float s2 = w3.u - w1.u;
			float t1 = w2.v - w1.v;
			float t2 = w3.v - w1.v;

			float r = 1.0f / (s1 * t2 - s2 * t1);
			Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for(std::size_t a = 0; a < vstr.size(); ++a)
		{
			Vector3 n(vstr[a].normal().x, vstr[a].normal().y, vstr[a].normal().z);
			


			
			
			float sign = n.SCross(tan1[a]).SDot(tan2[a]) < 0.0f ? 1.0f : -1.0f;

			vstr[a].tangent() = sign*(tan1[a] - n * n.SDot(tan1[a]) ).GetNormalized();
		}

		delete[] tan1;
	}
};

template<class VertexType, class IndexType> 
void computeTangents(VertexStream<VertexType>& vstr, IndexStream<IndexType> istr) {	computeTangentsC<VertexType, IndexType, defaultUV2Extractor>(vstr, istr); }

template<template <class> class UV2Extractor, class VertexType, class IndexType>	
void computeTangents(VertexStream<VertexType>& vstr, IndexStream<IndexType> istr) {	computeTangentsC<VertexType, IndexType, UV2Extractor>(vstr, istr);}

template<class VertexType, class IndexType>	
void computeTangents(Mesh<VertexType, IndexType>& srcm) 
{
	computeTangents(srcm.vertices(), srcm.indices());
}

template<class IndexType>
void convertCWtoCCW(IndexStream<IndexType>& istr)
{
	int facesCount = istr.size() / 3;
	for(int i = 0; i < facesCount; ++i)
	{
		FaceRef<IndexType>& f = istr.getFaceRef(i);
		std::swap(f.a, f.c);
	}
}



template<class VertexType, class IndexType>
void removeDegenerates(VertexStream<VertexType>& vs, IndexStream<IndexType>& is)
{
	INCOMPLETE;

}



}




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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