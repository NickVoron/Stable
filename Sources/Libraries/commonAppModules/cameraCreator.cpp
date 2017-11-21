#include "cameraCreator.h"

#include "inputSysWin/library.include.h"

namespace AppModules
{

void CameraCreator::update(const intersections::Ray& mouseRay)
{
	return;

	if( Input::mouseL(true) )
	{
		nm::plane plane(Vector3::zero, Vector3::xAxis, Vector3::zAxis);
				
		Vector3 point;
		if(plane.intersection(mouseRay.origin, mouseRay.direction, &point))
		{
			CameraSystem::cameras().create<CameraSystem::FreeCamera>( State::Simple(point) );
		}
	}
}

}