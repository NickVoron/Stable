#include "resourceSound.h"

namespace Resources
{
	void Sound::Clear()
	{
		deactivate();
		nativeResource.release();
		buf.release();
	}

	void Sound::Compile(stream::ostream& os)
	{
		os << wave;
	}

	void Sound::LoadSource(const char* sourceFile, const UserData* userData)
	{
		bufferWAVE(sourceFile, buf, wave);
	}

	void Sound::LoadCompiled(stream::istream& is)
	{
		is >> wave;
		bufferWAVE(buf, wave);
		nativeResource.bind(buf);

		wave.clear();

		activate();
	}

}

