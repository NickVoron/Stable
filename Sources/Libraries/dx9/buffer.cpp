#include "buffer.h"

namespace dx9
{

	void Buffer::save(stream::ostream& os)
	{
		os << (int)type << cacheSize;
		os.write(cacheData, cacheSize);	
	}

	void Buffer::load(stream::istream& is)
	{
		int t, sz;
		is >> t >> sz;
		type = (BufferType)t;
		allocCache(sz);

		is.read(cacheData, sz);
		updateFromCache();
	}

}