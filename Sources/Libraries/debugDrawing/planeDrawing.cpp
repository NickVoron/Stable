
#include "planeDrawing.h"

namespace DebugDrawing
{

	void plane(const nm::plane& plane, const Vector3& basis)
	{
		auto basePoint = plane.projection(basis);
		//sphere(basePoint, 1.3f);

		Vector3 planeVector = plane.n;
		Vector3 randomVector;
		if (!Vector3::zAxis.Collinear(planeVector))
		{
			randomVector = Vector3::zAxis;
		}
		else
		{
			randomVector = Vector3::xAxis;
		}

		
		
		Vector3 base1 = planeVector.SCross(randomVector);
		base1.Normalize();

		Vector3 base2 = planeVector.SCross(base1);
		base2.Normalize();

		

		//line(basePoint, basePoint + planeVector * 100);
		//line(basePoint, basePoint + base1 * 100);
		//line(basePoint, basePoint + base2 * 100);


		for (float i = -50; i<= 50; i+=10 )
		{
			Vector3 p1 = basePoint + base1*i - base2 * 50.0f;
			Vector3 p2 = basePoint + base1*i + base2 * 50.0f;
			line(p1, p2);

		}

		for (float i = -50; i <= 50; i+=10)
		{
			Vector3 p1 = basePoint + base2*i - base1 * 50.0f;
			Vector3 p2 = basePoint + base2*i + base1 * 50.0f;
			line(p1, p2);

		}



	}

}//