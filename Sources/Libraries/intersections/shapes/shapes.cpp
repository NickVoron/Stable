#include "shapes.h"

#include "debugDrawing/library.include.h"

namespace intersections
{


	//If the spheres enclose each other :
	//| c1 - c2 | +r1 < r2 or vice versa, take the larger sphere.
	//	Otherwise from the diagram it is clear that
	//	R = (r1 + r2 + | c1 - c2 | ) / 2
	//	C = c1 + (c2 - c1) * (R - r1) / | c2 - c1 | (linear interpolation)
	//	Where c1, c2, C are vectors, and | v | is the magnitude of vector v.

	Sphere Sphere::zero()
	{
		Sphere result;
		result.center = Vector3(0, 0, 0);
		result.radius = 0;

		return result;
	}

	Sphere Sphere::one(float x, float y, float z)
	{
		Sphere result;
		result.center = Vector3(x, y, z);
		result.radius = 1;

		return result;
	}


	void Sphere::merge(const Sphere& sphere)
	{
		auto& c1 = center;
		const auto& c2 = sphere.center;

		float& r1 = radius;
		const float& r2 = sphere.radius;

		if (nm::magnitude(c1 - c2) + r1 <= r2)
		{
			c1 = c2;
			r1 = r2;
			return;
		}

		if (nm::magnitude(c2 - c1) + r2 < r1)
		{
			return;
		}

		float r0 = (r1 + r2 + nm::magnitude(c1 - c2)) / 2;
		auto c0 = c1 + (c2 - c1)*(r0 - r1) / nm::magnitude(c2 - c1);

		c1 = c0;
		r1 = r0;
	}

	void Sphere::transform(const State& state, float scale)
	{
		center = state.GetLocalVector(center*scale);
		radius = radius *scale;
	}
	

}
