#include "links.h"

std::ostream& operator<<(std::ostream& os, const LinkDesc& ld)
{
	return os;
//	return os << "link: { object index: " << ld.address.objectIndex << ", component index: " << ld.address.componentIndex << " }";
}

std::ostream& operator<<(std::ostream& os, const LinksDescList& ldl)
{
	 os << "links count: " << ldl.size();
	 for (auto& link : ldl)
	 {
		 os << "\t" << link << std::endl;
	 }

	 return os;
}

