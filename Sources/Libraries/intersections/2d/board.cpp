#include "board.h"


namespace intersections2d
{
namespace board
{
	nm::index2 element(const nm::index2& segmentCount, const nm::index2& segmentSize, const nm::index2& pick)
	{
		nm::index2 result(0,0);

		result.x = segments::elemet(segmentCount.x, segmentSize.x, pick.x);
		result.y = segments::elemet(segmentCount.y, segmentSize.y, pick.y);

		return result;
	}

}//

namespace segments
{
	int elemet(int segmentCount, int segmentSize, int pick)
	{
		if (pick<0)
		{
			return 0;
		}

		if ( pick >= segmentSize *segmentCount -1)
			return segmentCount-1;


		return pick/ segmentSize;
	}
}//
}//