#include "inheritanceTable.h"

#include <string>

//
//
//
void InheritanceTable::dump()
{
	for(const auto& v : *this)
	{
// 		LOG_MSG(v.first);
// 		for(ClassNameFunc s : v.second)
// 		{
// 			LOG_MSG("\t" << s());
// 		}
// 		LOG_MSG_ENDL;
	}
}

void InheritanceTable::insert(ClassNameFunc parent, ClassNameFunc child)
{
	operator[](parent).insert(child);
}

bool InheritanceTable::valid(const std::string& parent, const std::string& candidate)
{
	for(const auto& v : *this)
	{
		if(v.first() == parent)
		{
			for(ClassNameFunc s : v.second)
			{
				if (s() == candidate)
				{
					return true;
				}
			}
		}
	}

	return false;
}