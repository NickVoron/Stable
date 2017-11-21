#pragma once

#include <iostream>
#include <algorithm>

#include "math/library.include.h"
#include "newmath/library.include.h"

#include "triangle.h"

namespace intersections
{
	

	struct Sphere 
	{
		static Sphere zero();
		static Sphere one(float x, float y, float z);

		Sphere(){};
		Sphere(const nm::Vector3& c, float r):center(c), radius(r){};
		nm::Vector3 center;
		float radius;

		void merge(const Sphere& sphere);
		void transform(const State& state, float scale);

	};

	struct Ray
	{
		Ray(){}
		Ray(const nm::Vector3& org, const nm::Vector3& dir) : origin(org), direction(dir){}
		nm::Vector3 origin;
		nm::Vector3 direction;
	};


	struct Line : public Ray 
	{
	};

	struct Segment3 
	{
		nm::Vector3 p0;
		nm::Vector3 p1;

		nm::Vector3 direction() const
		{
			return p1 - p0;
		}

	};

	struct Capsule3 
	{
		Segment3 segment;
		float radius;
	};

}
