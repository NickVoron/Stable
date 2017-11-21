#pragma once

#include "../mesh/vertexStream.h"
#include "intersections/library.include.h"

namespace Geometry
{
	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *matrix);
	void computeBestFitOBB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos, float *quat);
	void computeBestFitABB(unsigned int vcount, const float *points, unsigned int pstride, float *sides, float *pos);

	template<class VertexCL>
	void computeOptimalOBB(const Geometry::VertexStream< Geometry::Vertex<VertexCL> >& vertices, intersections::OBB& obb)
	{
		Quaternion quat;
		float sides[3];

		computeBestFitOBB(vertices.size(), (float*)&vertices[0].pos3(), sizeof(Geometry::Vertex<VertexCL>), sides, (float*)&obb.center, (float*)&quat);

		obb.extent[0] = sides[0] * 0.5;
		obb.extent[1] = sides[1] * 0.5;
		obb.extent[2] = sides[2] * 0.5;

		obb.axis[0] = quat.Transform(Vector3::xAxis);
		obb.axis[1] = quat.Transform(Vector3::yAxis);
		obb.axis[2] = quat.Transform(Vector3::zAxis);
	}

	//template<class VertexCL>
	//void computeOptimalSphere(const Geometry::VertexStream< Geometry::Vertex<VertexCL> >& vertices, intersections::Sphere& sphere)
	//{
	//	typedef double FT;
	//	typedef Seb::Point<FT> Point;
	//	typedef std::vector<Point> PointVector;
	//	typedef Seb::Smallest_enclosing_ball<FT> Miniball;

	//	PointVector pv;

	//	for (int i = 0; i < vertices.size(); ++i)
	//	{
	//		Point pt(3);
	//		pt[0] = vertices[i].pos3().x;
	//		pt[1] = vertices[i].pos3().y;
	//		pt[2] = vertices[i].pos3().z;
	//		pv.push_back(pt);
	//	}

	//	Miniball mb(3, pv);

	//	sphere.radius = mb.radius();
	//	Miniball::Coordinate_iterator center_it = mb.center_begin();

	//	sphere.center.x = *center_it; ++center_it;
	//	sphere.center.y = *center_it; ++center_it;
	//	sphere.center.z = *center_it;
	//}

}