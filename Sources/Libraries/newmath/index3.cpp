#include "index3.h"

namespace nm 
{
	const index3 index3::outIndex( 0x80000000, 0x80000000, 0x80000000);


	stream::ostream& operator<<(stream::ostream& os, const index3& index)
	{
		return os << index.x << index.y << index.z;
	}

	stream::istream& operator >> (stream::istream& is, index3& index)
	{
		return is >> index.x >> index.y >> index.z;
	}

}