#include "thirdPersonConstraint.h"

namespace CameraComponents
{

	void ThirdPerson::Resource::properties(ThirdPerson& component)
	{
		property("distance", component.camAffector.distance);
		property("heightOffset", component.camAffector.height_offset);
	}
}
