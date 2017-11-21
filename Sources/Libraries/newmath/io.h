#pragma once

#include <iostream>

#include "index2.h"
#include "index3.h"


namespace nm
{
	inline std::ostream& operator<<(std::ostream& os, const nm::index2& val) { return os << "[" << val.x << ", " << val.y << "]"; }
	inline std::ostream& operator<<(std::ostream& os, const nm::index3& val) { return os << "[" << val.x << ", " << val.y << ", " << val.z << "]"; }
}

