// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "bodies.h"
#include "meshOperations.h"

namespace Geometry {
namespace Generation {
namespace Private {

	void generateQuadIndices(BaseIndexStream& is)
	{
		is.resize(6);

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		is[3] = 2;
		is[4] = 3;
		is[5] = 0;
	}

	void generateQuadXY(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		vs.resize(4);
		
		Vector2 halfSize = size * 0.5f;

		vs[0].pos3(-halfSize.x, -halfSize.y, 0 );	vs[0].uv2<0>( 0.0f, 0.0f );
		vs[1].pos3(+halfSize.x, -halfSize.y, 0 );	vs[1].uv2<0>( 1.0f, 0.0f );
		vs[2].pos3(+halfSize.x, +halfSize.y, 0 );	vs[2].uv2<0>( 1.0f, 1.0f );
		vs[3].pos3(-halfSize.x, +halfSize.y, 0 );	vs[3].uv2<0>( 0.0f, 1.0f );

		generateQuadIndices(is);
		generateNormals(vs, is);
	}

	void generateQuadXZ(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		vs.resize(4);

		Vector2 halfSize = size * 0.5f;

		vs[0].pos3(-halfSize.x, 0 , -halfSize.y);	vs[0].uv2<0>( 0.0f, 0.0f );
		vs[1].pos3(+halfSize.x, 0 , -halfSize.y);	vs[1].uv2<0>( 1.0f, 0.0f );
		vs[2].pos3(+halfSize.x, 0 , +halfSize.y);	vs[2].uv2<0>( 1.0f, 1.0f );
		vs[3].pos3(-halfSize.x, 0 , +halfSize.y);	vs[3].uv2<0>( 0.0f, 1.0f );

		generateQuadIndices(is);
		generateNormals(vs, is);
	}

	void generateQuadYZ(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		vs.resize(4);

		Vector2 halfSize = size * 0.5f;

		vs[0].pos3(0, -halfSize.x, -halfSize.y);		vs[0].uv2<0>(0.0f, 0.0f);
		vs[1].pos3(0, +halfSize.x, -halfSize.y);		vs[1].uv2<0>(1.0f, 0.0f);
		vs[2].pos3(0, +halfSize.x, +halfSize.y);	vs[2].uv2<0>(1.0f, 1.0f);
		vs[3].pos3(0, -halfSize.x, +halfSize.y);		vs[3].uv2<0>(0.0f, 1.0f);

		generateQuadIndices(is);
		generateNormals(vs, is);
	}

	void generateQuad2D(BaseVertex2DStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		vs.resize(4);
		is.resize(6);

		Vector2 halfSize = size * 0.5f;

		vs[0].pos2(-halfSize.x, -halfSize.y);
		vs[1].pos2(+halfSize.x, -halfSize.y);
		vs[2].pos2(+halfSize.x, +halfSize.y);
		vs[3].pos2(-halfSize.x, +halfSize.y);

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		is[3] = 0;
		is[4] = 2;
		is[5] = 3;
	}

	void generateScreenQuad(BaseVertexScreenStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		vs.resize(4);
		is.resize(6);

		Vector2 halfSize = size * 0.5f;

		vs[0].posT(-halfSize.x, -halfSize.y, 0.01f, 1.0f );
		vs[1].posT(+halfSize.x, -halfSize.y, 0.01f, 1.0f );
		vs[2].posT(+halfSize.x, +halfSize.y, 0.01f, 1.0f );
		vs[3].posT(-halfSize.x, +halfSize.y, 0.01f, 1.0f );

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		is[3] = 0;
		is[4] = 2;
		is[5] = 3;
	}

	void generateScreenQuad(BaseVertexScreenStream& vs, BaseIndexStream& is, const Vector2& pos, const Vector2& size)
	{
		vs.resize(4);
		is.resize(6);

		Vector2 halfSize = size * 0.5f;

		vs[0].posT(pos.x,			pos.y,			0.01f, 1.0f );
		vs[1].posT(pos.x + size.x,	pos.y,			0.01f, 1.0f );
		vs[2].posT(pos.x + size.x,	pos.y + size.y, 0.01f, 1.0f );
		vs[3].posT(pos.x,			pos.y + size.y, 0.01f, 1.0f );

		is[0] = 0;
		is[1] = 1;
		is[2] = 2;

		is[3] = 0;
		is[4] = 2;
		is[5] = 3;
	}


	void generateRing(BaseVertexStream& vs, BaseIndexStream& is, float radius1, float radius2, unsigned int slices)
	{
		vs.resize(slices * 2);
		is.resize(slices * 6);

		float segmentAngle = nm::TwoPI / slices;
		Vector2 inV(0, radius1);
		Vector2 exV(0, radius2);
		for (unsigned int i = 0; i < slices; ++i)
		{
			float a = i * segmentAngle;
			float s = sinf( a );
			float c = cosf( a );
						
			vs[i * 2 + 0].pos3(inV.x*s + inV.y*c, 0.0f, inV.x*c - inV.y*s);
			vs[i * 2 + 1].pos3(exV.x*s + exV.y*c, 0.0f, exV.x*c - exV.y*s);
		}

		int k = 0;
		for (unsigned int j = 0; j < slices; ++j)
		{
			int base = j * 2;
			
			is[k++] = base;
			is[k++] = base + 1;
			is[k++] = base + 3;

			is[k++] = base;
			is[k++] = base + 3;
			is[k++] = base + 2;
		}
		
		is[k-4] = 1;
		is[k-2] = 1;
		is[k-1] = 0;
 	}

	void generatePlaneDiscSector(BaseVertexStream& vs, BaseIndexStream& is, float radius, float startAngle, float endAngle, unsigned int slices)
	{
		vs.resize(slices + 2);
		is.resize(slices * 3);

		int v = 0;
		vs[v++].pos3() = POS3(0.0f, 0.0f, 0.0f);

		float diffAngles = nm::substract_angle(endAngle, startAngle);
		float da = diffAngles / slices;
		for(unsigned int i = 0; i <= slices; ++i)
		{
			float a = startAngle + da * i;
			vs[v++].pos3(radius * sinf(a), 0.0f, radius * cosf(a) );
		}	

		int k = 0;
		for(unsigned int i = 0; i < slices; ++i)
		{
			is[k++] = 0;
			is[k++] = i + 1;
			is[k++] = i + 2;
		}
	}

	void generatePlaneDisc(BaseVertexStream& vs, BaseIndexStream& is, float radius, unsigned int slices)
	{
		vs.resize(slices + 1);
		is.resize(slices * 3);

		int v = 0;
		vs[v++].pos3(0.0f, 0.0f, 0.0f);

		float da = nm::TwoPI / slices;
		for(unsigned int i = 0; i < slices; ++i)
		{
			float a = da * i;
			vs[v++].pos3(radius * sinf(a), 0.0f, radius * cosf(a) );
		}	

		int k = 0;
		for(unsigned int i = 0; i < slices - 1; ++i)
		{
			is[k++] = 0;
			is[k++] = i + 1;
			is[k++] = i + 2;
		}
		is[k++] = 0;
		is[k++] = slices;
		is[k++] = 1;

	}

	void generateStar(BaseVertexStream& vs, BaseIndexStream& is, float radius1, float radius2, unsigned int rays)
	{
		vs.resize(rays * 2);
		is.resize(rays * 3);

		float da = nm::TwoPI / rays;
		float da_2 = da * 0.5f;
		int v = 0;
		for(unsigned int i = 0; i < rays; ++i)
		{
			float a = da * i;
			vs[v++].pos3(radius1 * sinf(a - da_2 ), 0.0f, radius1 * cosf(a - da_2) );
			vs[v++].pos3(radius2 * sinf(a),			0.0f, radius2 * cosf(a) );
		}

		int k = 0;
		for(unsigned int i = 0; i < rays; ++i)
		{
			int base = i * 2;
			is[k++] = base;
			is[k++] = base + 1;
			is[k++] = base + 2;
		}

		is[rays * 3 - 1] = 0;
	}

	void generatePlaneSectorsDisc(BaseVertexStream& vs, BaseIndexStream& is, float radius, int sectorsCount, float sectorAngle, unsigned int slicesInSector)
	{
		vs.resize((slicesInSector + 2) * sectorsCount);
		is.resize((slicesInSector * 3) * sectorsCount);

		float sectorAngleStep = nm::TwoPI / sectorsCount;
		float startAngle = 0.0f;

		int v = 0;
		int k = 0;
		vs[v++].pos3(0.0f, 0.0f, 0.0f);
		for(int j = 0; j < sectorsCount; ++j)
		{
			float endAngle = startAngle + sectorAngle;
			float diffAngles = nm::substract_angle(endAngle, startAngle);

			float da = diffAngles / slicesInSector;
			for(unsigned int i = 0; i <= slicesInSector; ++i)
			{
				float a = startAngle + da * i;
				vs[v++].pos3(radius * sinf(a), 0.0f, radius * cosf(a));
			}	

			for(unsigned int i = 0; i < slicesInSector; ++i)
			{
				is[k++] = 0;
				is[k++] = i + 1 + j * (slicesInSector + 1);
				is[k++] = i + 2 + j * (slicesInSector + 1);
			}

			startAngle += sectorAngleStep;
		}
	}

	void generateDome(BaseVertexStream& vs, BaseIndexStream& is, float radius, int rows, int columns)
	{
		vs.resize(columns * rows);
		is.resize((columns - 1) * (rows - 1) * 6);
		
		int domeIndex = 0;
		for(int i = 0; i < columns; ++i)
		{
			const float moveXZ = cosf(1.0f) * i / (columns - 1);
			for(int j = 0; j < rows; ++j)
			{	
				const float moveY = nm::TwoPI * j / (rows - 1) ;

				vs[domeIndex].pos3(sinf(moveXZ) * cosf(moveY) * radius, cosf(moveXZ) * radius, sinf(moveXZ) * sinf(moveY) * radius);
				domeIndex++;
			}
		}

		int k = 0;
		for(int i = 0; i < rows - 1; ++i)
		{
			for(int j = 0; j < columns - 1; ++j)
			{
				is[k++] = i * rows + j;
				is[k++] = (i + 1) * rows + j;
				is[k++] = (i + 1) * rows + j + 1;

				is[k++] = (i + 1) * rows + j + 1;
				is[k++] = i * rows + j + 1;
				is[k++] = i * rows + j;
			}
		}

	}

	void generateLine(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& p0, const Vector3& p1)
	{
		vs.resize(2);
		is.resize(2);

		vs[0].pos3() = p0;
		vs[1].pos3() = p1;

		is[0] = 0;
		is[1] = 1;
	}

	void generateWireBox(BaseVertexStream& vs, BaseIndexStream& is, const Vector3& size)
	{
		BaseVertexStream lvs;			
		BaseIndexStream lis;
		Vector3 extents = size * 0.5f;

		Vector3 at(-extents.x, extents.y, -extents.z );
		Vector3 bt(-extents.x, extents.y, extents.z );
		Vector3 ct(extents.x, extents.y, extents.z );
		Vector3 dt(extents.x, extents.y, -extents.z );
		
		Vector3 ab(-extents.x, -extents.y, -extents.z );
		Vector3 bb(-extents.x, -extents.y, extents.z );
		Vector3 cb(extents.x, -extents.y, extents.z );
		Vector3 db(extents.x, -extents.y, -extents.z );

		generateLine(lvs, lis, at, bt); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, bt, ct); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, ct, dt); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, dt, at); mergeMeshes(vs, is, lvs, lis);

		generateLine(lvs, lis, ab, bb); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, bb, cb); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, cb, db); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, db, ab); mergeMeshes(vs, is, lvs, lis);

		generateLine(lvs, lis, at, ab); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, bt, bb); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, ct, cb); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, dt, db); mergeMeshes(vs, is, lvs, lis);
	}

	void generateWireQuadXY(BaseVertexStream& vs, BaseIndexStream& is, const Vector2& size)
	{
		BaseVertexStream lvs;			
		BaseIndexStream lis;
		Vector2 extents = size * 0.5f;

		Vector3 a(-extents.x, -extents.y, 0);
		Vector3 b(-extents.x, extents.y, 0);
		Vector3 c(extents.x, extents.y, 0);
		Vector3 d(extents.x, -extents.y, 0);

		generateLine(lvs, lis, a, b); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, b, c); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, c, d); mergeMeshes(vs, is, lvs, lis);
		generateLine(lvs, lis, d, a); mergeMeshes(vs, is, lvs, lis);
	}

	void generateWireCircleXY(BaseVertexStream& vs, BaseIndexStream& is, float radius)
	{
		int segmentsCount = 36;
		float angle = nm::TwoPI / segmentsCount;
		std::vector<Vector3> points(segmentsCount);

		for (int i = 0; i < segmentsCount; ++i)
		{
			Vector3& pt = points[i];
			pt.x = sinf(angle * i) * radius;
			pt.y = cosf(angle * i)* radius;
			pt.z = 0.0f;
		}
		
		BaseVertexStream lvs;			
		BaseIndexStream lis;

		for (int i = 0; i < segmentsCount - 1; ++i)
		{
			const Vector3& a = points[i];
			const Vector3& b = points[i+1];
			generateLine(lvs, lis, a, b); 
			mergeMeshes(vs, is, lvs, lis);						
		}

		generateLine(lvs, lis, points.back(), points.front()); 
		mergeMeshes(vs, is, lvs, lis);						
	}

}}}



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