#pragma once
#include <map>
#include <string>

namespace Base
{	
	struct namesGenerator
	{
		static std::string uniqueName(const std::string& baseName );
	};
}