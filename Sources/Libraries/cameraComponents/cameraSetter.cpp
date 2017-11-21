#include "cameraSetter.h"

#include "cameraSystem/library.include.h"
#include "viewports/library.include.h"

namespace CameraComponents
{

void CameraSetter::linker()
{
	link(paramHolder);
}

void CameraSetter::update(float dt)
{
	for (Viewports::Viewport& vp : Viewports::viewports)
	{
		vp.camera.external(&paramHolder->camera, false);
	}

}


void CameraSetter::onDeactivate()
{
	for (Viewports::Viewport& vp : Viewports::viewports)
	{
		vp.camera.external(0, false);
	}
	
}
}//
