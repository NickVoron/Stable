
#include "mitchellNetravali.h"
#include <math.h>

namespace image	
{
	namespace filter
	{

		float mitchellNetravali(float x)
		{
			const float B = 1 / 3.0, C = 1 / 3.0; // Recommended
			// const float B =   1.0, C =   0.0; // Cubic B-spline (smoother results)
			// const float B =   0.0, C = 1/2.0; // Catmull-Rom spline (sharper results)
			x = fabs(x);
			if (x < 1)
				return ((12 - 9 * B - 6 * C) * x * x * x
				+ (-18 + 12 * B + 6 * C) * x * x
				+ (6 - 2 * B)) / 6;
			else if (x < 2)
				return ((-B - 6 * C) * x * x * x
				+ (6 * B + 30 * C) * x * x
				+ (-12 * B - 48 * C) * x
				+ (8 * B + 24 * C)) / 6;
			else
				return 0;
		}
	}

}//