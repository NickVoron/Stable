#include "openal.h"

#include "openal/library.include.h"

namespace AppModules
{
	void OpenAL::init()
	{
		if(!openal::init())
		{
			LOG_ERROR("Failed to initialize OpenAL")
   		}						
	}

	void OpenAL::process(float dt, const State& camera)
	{

		// OpenAL uses a normal right - handed system so your X axis is the same as theirs, 
		// but your Y needs to be their - Z(ie.negated), and your Z needs to be their Y.
		// Trigonometrically, it's a 90 degree rotation around the X axis.

		Vector3 oalPos(camera.position.x, -1 * camera.position.z, camera.position.y);
		
 		openal::listener::position(oalPos);

		Vector3 dir = camera.orientation.GetZAxis();
		Vector3 up = camera.orientation.GetYAxis();
		
		Vector3 oalDir(dir.x, -1 * dir.z, dir.y);
		Vector3 oalUp(up.x, -1 * up.z, -1*up.y);
 		openal::listener::orientation(oalDir, oalUp);
	}

	void OpenAL::release()
	{
		openal::shutdown();	   
	}
}
