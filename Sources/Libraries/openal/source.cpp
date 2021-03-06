// Copyright (C) 2012-2016 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "source.h"


namespace openal
{					
	source::source():inited(false)
	{

	}

	source::~source()
	{
		release();
	}

	void source::init()
	{
		if(!inited)
		{
			alGenSources(1, &id);
			inited = true;
		}		
	}

	void source::release()
	{
		if(inited)
		{
			stop();
			alDeleteSources(1, &id);
			inited = false;
		}		
	}

	void source::bind(const buffer& buf)
	{
		if(!inited)
			init();
				
  		alSourcei(id, AL_BUFFER, buf.id);		
	}

	void source::play(bool looped)
	{
		alSourcei(id, AL_LOOPING, looped ? AL_TRUE : AL_FALSE);
		alSourcePlay(id);
	}

	void source::stop()
	{
		alSourceStop(id);
	}

	void source::pause()
	{
		alSourcePause(id);
	}

	void source::position(const Vector3& pos)
	{
		
		
		

		alSource3f(id, AL_POSITION, pos.x, -1*pos.z, pos.y);
	}

	ALint source::state() const
	{
		ALint s;
		alGetSourcei(id, AL_SOURCE_STATE, &s);
		return s;
	}

	bool source::playing() const
	{
		return state() == AL_PLAYING;
	}

	bool source::stopped() const
	{	
		return state() == AL_STOPPED;
	}

	bool source::paused() const
	{
		return state() == AL_PAUSED;
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