#pragma once

#include <string>

namespace ObjectParser
{
	struct ComponentNameSource
	{
		static void reset();
		static int guid();
		static std::string strGuid(const char* baseName);
	};
}