#include "namesGenerator.h"
#include "stringUtils.h"

#include <mutex>

namespace Base
{	
	namespace
	{
		std::mutex namesGeneratorMTX;
	}	

	std::string namesGenerator::uniqueName(const std::string& baseName )
	{
		std::unique_lock<std::mutex> lock(namesGeneratorMTX);

		static std::map<std::string, int> names;
		int& counter = names[baseName];
		++counter;
		return str::stringize(baseName, "_", counter);
	}
}