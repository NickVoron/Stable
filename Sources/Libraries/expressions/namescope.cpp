#include "namescope.h"

namespace Expressions
{
	uint64_t newid()
	{
		static std::atomic_size_t counter(0);
		return counter++;
	}

}//