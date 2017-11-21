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
		// OpenAL uses a normal right - handed system so your X axis is the same as theirs, 
		// but your Y needs to be their - Z(ie.negated), and your Z needs to be their Y.
		// Trigonometrically, it's a 90 degree rotation around the X axis.

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