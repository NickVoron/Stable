#include "poin2plane.h"
#include "sphere2oobb.h"

namespace intersections
{

void point2planeProjection(const Vector3& N, const Triangle& triangle, Vector3& result)
{
	const Vector3& M0 = triangle.vertices[0];
	const Vector3& M1 = triangle.vertices[1];
	const Vector3& M2 = triangle.vertices[2];


	Vector3 M0N = N-M0;
	Vector3 normal;
	nm::cross(M1-M0, M2-M0, normal);

	float distance = nm::dot(normal, M0N)/ normal.Magnitude();

	normal.Normalize();
	normal = normal* distance;

	result = N-normal;
}

}//