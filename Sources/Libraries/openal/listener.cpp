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