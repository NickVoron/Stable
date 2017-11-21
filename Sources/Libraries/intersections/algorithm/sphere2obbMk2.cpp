#include "sphere2obbMk2.h"


namespace intersections
{


	void clamp(Vector3& v, const Vector3& max)
	{
		if(v.MagnitudeSquared() > max.MagnitudeSquared())
		{
			v.Normalize();
			v*= max.Magnitude();
		}
	}

	void nearestPointMk2(const Vector3& N, const OBB& obb, Vector3& point)
	{
		Vector3 result(0,0,0);

		const Vector3 NC = N-obb.center;

		for (int i=0; i<3; i++)
		{
			Vector3 normalizedAxis = obb.axis[i];
			normalizedAxis.Normalize();
			
			Vector3 sizedAxis = normalizedAxis * obb.extent[i];

			float projSize = nm::dot(sizedAxis, NC)/sizedAxis.Magnitude(); 
			
			Vector3 tmp = normalizedAxis*projSize;
			clamp(tmp, sizedAxis);

			result += tmp;
		}



		point = result+obb.center;


	}

}