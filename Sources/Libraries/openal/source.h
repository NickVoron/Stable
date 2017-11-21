#pragma once
					 
#include "math/library.include.h"

#include "buffer.h"

namespace openal
{
	class source
	{
	public:
		source();
		~source();
		void init();
		void release();

		void bind(const buffer& buf);
		void play(bool looped = false);
		void stop();
		void pause();

		bool playing() const;
		bool stopped() const;
		bool paused() const;

		void position(const Vector3& pos);


	private:
		ALint state() const;
		ALuint id;
		bool inited;
	};
}

