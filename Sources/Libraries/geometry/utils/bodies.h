// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "../mesh/mesh.h"
#include "math/library.include.h"
#include "newmath/library.include.h"

namespace Geometry {
namespace Generation {

	namespace Private
	{
		typedef Vertex<POS3, NORMAL, UV2<0>> BaseVertexType;
		typedef Vertex<POS2> BaseVertexType2D;
		typedef Vertex<POST> BaseVertexTypeScreen;
		typedef unsigned int BaseIndexType;

		typedef VertexStream<BaseVertexType> BaseVertexStream;
		typedef VertexStream<BaseVertexType2D> BaseVertex2DStream;
		typedef VertexStream<BaseVertexTypeScreen> BaseVertexScreenStream;

		typedef IndexStream<BaseIndexType> BaseIndexStream;

		typedef Mesh<BaseVertexType2D, BaseIndexType> BaseMesh2D;
		typedef Mesh<BaseVertexTypeScreen, BaseIndexType> BaseMeshScreen;
		typedef Mesh<BaseVertexType, BaseIndexType> BaseMesh;
		
		void generateCylinder(BaseVertexStream& vs, BaseIndexStream& is, float radius, float height, unsigned int slices, unsigned int stacks);
		void generateCone(BaseVertexStream& vs, BaseIndexStream& is, float radiusTop, float radiusBottom, float height, unsigned int slices, unsigned int stacks);
		void generateBox(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& size);
		void generateDome(BaseVertexStream& vs, BaseIndexStream& is, float radius, int rows, int columns);
		void generateSphere(BaseVertexStream& vs, BaseIndexStream& is, float radius, int subdiv);
		void generatePyramid(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& base, float height);
		void generateGrid(BaseVertexStream& vs, BaseIndexStream& is, const nm::index2& vertexCount, const Vector2& gridStep );
		void generateQuadXY(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateQuadXZ(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateQuadYZ(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateGrid2D(BaseVertex2DStream& vs, BaseIndexStream& is, const nm::index2& vertexCount, const Vector2& gridStep );
		void generateQuad2D(BaseVertex2DStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateStar(BaseVertexStream& vs, BaseIndexStream& is, float radius1, float radius2, unsigned int rays);
		void generateScreenQuad(BaseVertexScreenStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateScreenQuad(BaseVertexScreenStream& vs, BaseIndexStream& is, const Vector2& pos, const Vector2& size);
		void generateRing(BaseVertexStream& vs, BaseIndexStream& is, float radius1, float radius2, unsigned int slices);
		void generatePlaneDisc(BaseVertexStream& vs, BaseIndexStream& is, float radius, unsigned int slices);
		void generatePlaneDiscSector(BaseVertexStream& vs, BaseIndexStream& is, float radius, float startAngle, float endAngle, unsigned int slices);
		void generatePlaneSectorsDisc(BaseVertexStream& vs, BaseIndexStream& is, float radius, int sectorsCount, float sectorAngle, unsigned int slicesInCector);
		void generateLine(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& p0, const Vector3& p1);
		
		void generateWireBox(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& size);
		void generateWireQuadXY(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size);
		void generateWireCircleXY(BaseVertexStream& vs, BaseIndexStream& is, float radius);
	}



template<class VertexType, class IndexType>
void generateCylinder(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, float height, unsigned int slices, unsigned int stacks)
{
	Private::BaseVertexStream bvs;
	Private::generateCylinder(bvs, is, radius, height, slices, stacks);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateBox(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector3& size)
{
	Private::BaseVertexStream bvs;
	Private::generateBox(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateDome(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, int rows, int columns)
{
	Private::BaseVertexStream bvs;
	Private::generateDome(bvs, is, radius, rows, columns);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateSphere(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, int subdiv )
{
	Private::BaseVertexStream bvs;
	Private::generateSphere(bvs, is, radius, subdiv);
	vs.maxAssign(bvs);

}

template<class VertexType, class IndexType>
void generateGrid(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const nm::index2& vertexCount, const Vector2& gridStep)
{
	Private::BaseVertexStream bvs;
	Private::generateGrid(bvs, is, vertexCount, gridStep);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateGrid2D(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const nm::index2& vertexCount, const Vector2& gridStep)
{
	Private::BaseVertex2DStream bvs;
	Private::generateGrid2D(bvs, is, vertexCount, gridStep);
	vs.template assign<Loki::MakeTypelist<POS2>>(bvs);
}

template<class VertexType, class IndexType>
void generateQuadXY(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertexStream bvs;
	Private::generateQuadXY(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateQuadXZ(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertexStream bvs;
	Private::generateQuadXZ(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateQuadYZ(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertexStream bvs;
	Private::generateQuadYZ(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateStar(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius1, float radius2, unsigned int rays)
{
	Private::BaseVertexStream bvs;
	Private::generateStar(bvs, is, radius1, radius2, rays);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateRing(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius1, float radius2, unsigned int slices)
{
	Private::BaseVertexStream bvs;
	Private::generateRing(bvs, is, radius1, radius2, slices);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generatePlaneDiscSector(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, float startAngle, float endAngle, unsigned int slices)
{
	Private::BaseVertexStream bvs;
	Private::generatePlaneDiscSector(bvs, is, radius, startAngle, endAngle, slices);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generatePlaneDisc(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, unsigned int slices)
{
	Private::BaseVertexStream bvs;
	Private::generatePlaneDisc(bvs, is, radius, slices);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generatePlaneSectorsDisc(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius, int sectorsCount, float sectorAngle, int slicesInCector)
{
	Private::BaseVertexStream bvs;
	Private::generatePlaneSectorsDisc(bvs, is, radius, sectorsCount, sectorAngle, slicesInCector);
	vs.maxAssign(bvs);
}


template<class VertexType, class IndexType>
void generateQuad2D(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertex2DStream bvs;
	Private::generateQuad2D(bvs, is, size);
	vs.template assign<Loki::MakeTypelist<POS2>>(bvs);
}

template<class VertexType, class IndexType>
void generateScreenQuad(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertexScreenStream bvs;
	Private::generateScreenQuad(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateScreenQuad(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& pos, const Vector2& size)
{
	Private::BaseVertexScreenStream bvs;
	Private::generateScreenQuad(bvs, is, pos, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateWireBox(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector3& size)
{
	Private::BaseVertexStream bvs;
	Private::generateWireBox(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateWireQuadXY(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector2& size)
{
	Private::BaseVertexStream bvs;
	Private::generateWireQuadXY(bvs, is, size);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateWireCircleXY(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radius)
{
	Private::BaseVertexStream bvs;
	Private::generateWireCircleXY(bvs, is, radius);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateCone(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, float radiusTop, float radiusBottom, float height, unsigned int slices, unsigned int stacks)
{
	Private::BaseVertexStream bvs;
	Private::generateCone(bvs, is, radiusTop, radiusBottom, height, slices, stacks);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType>
void generateLine(VertexStream<VertexType>& vs, IndexStream<IndexType>& is, const Vector3& p0, const Vector3& p1)
{
	Private::BaseVertexStream bvs;
	Private::generateLine(bvs, is, p0, p1);
	vs.maxAssign(bvs);
}

template<class VertexType, class IndexType> void generateCylinder(Mesh<VertexType, IndexType>& mesh, float radius, float height, unsigned int slices, unsigned int stacks)						{ generateCylinder(mesh.vertices(), mesh.indices(), radius, height, slices, stacks); }
template<class VertexType, class IndexType> void generateBox(Mesh<VertexType, IndexType>& mesh, const Vector3& size)																			{ generateBox(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateDome(Mesh<VertexType, IndexType>& mesh, float radius, int rows, int columns)															{ generateDome(mesh.vertices(), mesh.indices(), radius, rows, columns); }
template<class VertexType, class IndexType> void generateSphere(Mesh<VertexType, IndexType>& mesh, float radius, int subdiv)																	{ generateSphere(mesh.vertices(), mesh.indices(), radius, subdiv); }
template<class VertexType, class IndexType> void generateGrid(Mesh<VertexType, IndexType>& mesh, const nm::index2& vertexCount, const Vector2& gridStep)										{ generateGrid(mesh.vertices(), mesh.indices(), vertexCount, gridStep); }
template<class VertexType, class IndexType> void generateGrid2D(Mesh<VertexType, IndexType>& mesh, const nm::index2& vertexCount, const Vector2& gridStep)										{ generateGrid2D(mesh.vertices(), mesh.indices(), vertexCount, gridStep); }
template<class VertexType, class IndexType> void generateStar(Mesh<VertexType, IndexType>& mesh, float radius1, float radius2, unsigned int rays)												{ generateStar(mesh.vertices(), mesh.indices(), radius1, radius2, rays); }
template<class VertexType, class IndexType> void generateQuadXY(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																			{ generateQuadXY(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateQuadXZ(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																			{ generateQuadXZ(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateQuadYZ(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																			{ generateQuadYZ(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateQuad2D(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																			{ generateQuad2D(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateScreenQuad(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																		{ generateScreenQuad(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateScreenQuad(Mesh<VertexType, IndexType>& mesh, const Vector2& pos, const Vector2& size)													{ generateScreenQuad(mesh.vertices(), mesh.indices(), pos, size); }
template<class VertexType, class IndexType> void generateRing(Mesh<VertexType, IndexType>& mesh, float radius1, float radius2, unsigned int slices)												{ generateRing(mesh.vertices(), mesh.indices(), radius1, radius2, slices); }
template<class VertexType, class IndexType> void generatePlaneDiscSector(Mesh<VertexType, IndexType>& mesh, float radius, float startAngle, float endAngle, unsigned int slices)				{ generatePlaneDiscSector(mesh.vertices(), mesh.indices(), radius, startAngle, endAngle, slices); }
template<class VertexType, class IndexType> void generatePlaneSectorsDisc(Mesh<VertexType, IndexType>& mesh, float radius, int sectorsCount, float sectorAngle, int slicesInCector)				{ generatePlaneSectorsDisc(mesh.vertices(), mesh.indices(), radius, sectorsCount, sectorAngle, slicesInCector); }
template<class VertexType, class IndexType> void generatePlaneDisc(Mesh<VertexType, IndexType>& mesh, float radius, unsigned int slices)														{ generatePlaneDisc(mesh.vertices(), mesh.indices(), radius, slices); }
template<class VertexType, class IndexType> void generateCone(Mesh<VertexType, IndexType>& mesh, float radiusTop, float radiusBottom, float height, unsigned int slices, unsigned int stacks)	{ generateCone(mesh.vertices(), mesh.indices(), radiusTop, radiusBottom, height, slices, stacks); }
template<class VertexType, class IndexType> void generateWireLine(Mesh<VertexType, IndexType>& mesh, const Vector3& start, const Vector3& end)													{ generateWireLine(mesh.vertices(), mesh.indices(), start, end); }
template<class VertexType, class IndexType> void generateWireBox(Mesh<VertexType, IndexType>& mesh, const Vector3& size)																		{ generateWireBox(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateWireQuadXY(Mesh<VertexType, IndexType>& mesh, const Vector2& size)																		{ generateWireQuadXY(mesh.vertices(), mesh.indices(), size); }
template<class VertexType, class IndexType> void generateWireCircleXY(Mesh<VertexType, IndexType>& mesh, float radius)																			{ generateWireCircleXY(mesh.vertices(), mesh.indices(), radius); }
template<class VertexType, class IndexType> void generateLine(Mesh<VertexType, IndexType>& mesh, const Vector3& p0, const Vector3& p1)															{ generateLine(mesh.vertices(), mesh.indices(), p0, p1); }






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