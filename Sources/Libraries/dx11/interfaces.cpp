#include "interfaces.h"

namespace dx11
{
	void execute(const Viewports::Viewport& viewport, DrawableObjectsList& objects)
	{
		objects.execute(viewport.deviceIndex());
	}

	void execute(const Viewports::Viewport& viewport, DrawableObject& object)
	{
		object.execute(viewport.deviceIndex());
	}
}