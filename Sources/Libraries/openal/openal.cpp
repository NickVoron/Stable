#include "openal.h"

namespace openal
{
	bool init()
	{
		ALCcontext* context = 0;
		ALCdevice* device = 0;

		device = alcOpenDevice(0);
		if (device)
		{
			context = alcCreateContext(device, 0);
			if (context)
			{
				alcMakeContextCurrent(context);
				return true;
			}
			else
			{
				alcCloseDevice(device);
			}
		}

		return false;
	}

	void shutdown()
	{
		ALCcontext* context;
		ALCdevice* device;

		context = alcGetCurrentContext();
		device = alcGetContextsDevice(context);

		alcMakeContextCurrent(0);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}