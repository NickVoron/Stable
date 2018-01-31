// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "listener.h"

namespace openal
{			   
	float listener::gain()
	{
		float value;
		alGetListenerf(AL_GAIN, &value);
		return value;
	}

	void listener::gain(float value)
	{
		alListenerf(AL_GAIN, value);
	}

	void listener::position(const Vector3& pos)
	{
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	}

	void listener::velocity(const Vector3& vel)
	{
		alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
	}

	void listener::orientation(const Vector3& dir, const Vector3& up)
	{
		ALfloat v[6] = {dir.x, dir.y, -dir.z, up.x, up.y, up.z};
		alListenerfv(AL_ORIENTATION, v);
	}

}



// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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