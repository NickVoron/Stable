#include "dateTime.h"

#ifdef WIN32
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <algorithm>
#endif

namespace Base 
{
namespace DateTime 
{

std::string getCurrentDateTimeStr()
{
	std::string dateTime;
#ifdef WIN32
	char tmpbuf[128];

	_tzset();
	_strdate( tmpbuf );	dateTime += tmpbuf;
	dateTime += "_";
	_strtime( tmpbuf ); dateTime += tmpbuf;
	std::replace(dateTime.begin(), dateTime.end(), ':', '.');
	std::replace(dateTime.begin(), dateTime.end(), '/', '.');
#endif
	return dateTime;
}

}
}