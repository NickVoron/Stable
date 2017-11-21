#pragma once
#include <ostream>
#include "strings.h"

namespace str
{
	template<typename CharType, int maxLength>
	std::ostream& operator<<(std::ostream& os, const string_t<CharType, maxLength>& value)
	{
		return os << value.c_str();
	}
}