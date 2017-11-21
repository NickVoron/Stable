#pragma once
#include "newmath/library.include.h"

namespace intersections2d
{
	namespace board 
	{
		nm::index2 element(const nm::index2& segmentCount, const nm::index2& elementSize, const nm::index2& pick);
	}//

	namespace segments
	{
		int elemet(int segmentCount, int segmentSize, int pick); // �� 0 �� segmentCount*elementSize-1
	}

}//