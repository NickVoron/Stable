#pragma once

#include <map>

#include "libopenal/library.include.h"

namespace openal
{
	class buffer
	{
	public:
		buffer();
		void init();
		void release();
		bool valid() const;
	
		bool inited;
		ALuint id;
	};

	struct buffers : std::map<std::string, buffer*>
	{
	public:
		buffer& get(const std::string& name);
		buffer& loadWAVE(const std::string& fileName);
	};	
}

