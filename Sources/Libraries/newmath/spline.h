#pragma once
#include <vector>
#include "vector.h"


namespace nm
{
	struct spline
	{
		std::vector<nm::Vector3> vertices;
		std::vector<nm::Vector3> in;
		std::vector<nm::Vector3> out;

		static	nm::Vector3	bezier3(const nm::Vector3& p1, const nm::Vector3& p2, const nm::Vector3& p3, float mu);
		static	nm::Vector3	bezier4(const nm::Vector3& p1, const nm::Vector3& p2, const nm::Vector3& p3, const nm::Vector3& p4, float mu);
		static	nm::Vector3	bezier(nm::Vector3 *p, int n, float mu);
	};
}
