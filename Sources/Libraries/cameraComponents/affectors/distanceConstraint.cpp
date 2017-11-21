#include "distanceConstraint.h"

namespace CameraComponents
{
	
	void DistanceConstraint::Resource::properties(DistanceConstraint& component)
	{
		property("range", component.camAffector.range);
	}
}
