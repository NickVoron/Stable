#include "fileModifyObserver.h"
#include "fileUtils.h" 

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

namespace Base 
{
namespace FileUtils 
{

bool ModifyObserver::enabled = true;

bool ModifyObserver::CheckModified() const
{
	if(!enabled) return false;

	struct ::stat newStats;
	stat(fileName.c_str(), &newStats);
	time_t t = newStats.st_mtime;
	return (time != t) && Base::FileUtils::FileExists(fileName);
}


bool ModifyObserver::CheckAndClearModified()
{
	if(!enabled) return false;

	struct ::stat newStats;
	stat(fileName.c_str(), &newStats);
	time_t t = newStats.st_mtime;
	bool res = (time != t) && Base::FileUtils::FileExists(fileName);
	time = t;
	return res;
}

void ModifyObserver::ClearModified()
{
	if(!enabled) return;

	struct ::stat newStats;
	stat(fileName.c_str(), &newStats);
	time = newStats.st_mtime;
}
 
void ModifyObserver::SetFileName( const std::string& fn ) 
{ 
	if(!enabled) return;

	fileName = fn; 
	ClearModified();
}

// stream::ostream& operator<<(stream::ostream& os, const ModifyObserver& mo)
// {
// 	return os << mo.fileName << mo.time;
// }
// 
// stream::istream& operator>>(stream::istream& is, ModifyObserver& mo)
// {
// 	return is >> mo.fileName >> mo.time;
// }
} // -namespace
}