#include "around.h"

namespace CameraComponents
{

	void Around::Resource::properties(Around& component)
	{
		property("distance", component.camAffector.distance);
		property("heightOffset", component.camAffector.height_offset);
	}
}
