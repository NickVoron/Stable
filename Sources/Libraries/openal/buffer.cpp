#include "buffer.h"
#include "wave.h"

namespace openal
{
	buffer::buffer():inited(false)
	{
		
	}

	void buffer::init()
	{
		release();

		if(!inited)
		{
			alGenBuffers(1, &id);
			inited = true;
		}		
	}

	void buffer::release()
	{
		if(inited)
		{
			alDeleteBuffers(1, &id);
			inited = false;
		}		
	}

	bool buffer::valid() const
	{
		return inited && alIsBuffer(id) != 0;
	}

	buffer& buffers::get(const std::string& name)
	{
		buffer*& buf = (*this)[name];

		if(!buf)
		{
			buf = new buffer();
		}

		return *buf;
	}

	buffer& buffers::loadWAVE(const std::string& fileName)
	{
		buffer& buf = get(fileName);
		if(!buf.valid())
		{		  
			buf.init();
			bufferWAVE(fileName.c_str(), buf);			
		}

		return buf;
	}
}