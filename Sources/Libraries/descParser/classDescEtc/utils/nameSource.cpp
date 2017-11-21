#include "nameSource.h"
#include "stuff/library.include.h"
#include <atomic>

namespace ObjectParser
{

static std::atomic_int i(0);


void ComponentNameSource::reset()
{
	i = 0;
}

int ComponentNameSource::guid() 
{ 
	return ++i; 
}

std::string ComponentNameSource::strGuid(const char* baseName)
{ 
	return str::stringize(baseName, "_", guid());
}

}