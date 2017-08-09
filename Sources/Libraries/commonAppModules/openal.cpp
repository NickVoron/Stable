// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

		
		
		

		Vector3 oalPos(camera.position.x, -1 * camera.position.z, camera.position.y);
		
 		openal::listener::position(oalPos);

		Vector3& dir = camera.orientation.GetZAxis();
		Vector3& up = camera.orientation.GetYAxis();
		
		Vector3 oalDir(dir.x, -1 * dir.z, dir.y);
		Vector3 oalUp(up.x, -1 * up.z, -1*up.y);
 		openal::listener::orientation(oalDir, oalUp);
	}

	void OpenAL::release()
	{
		openal::shutdown();	   
	}
}




// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.