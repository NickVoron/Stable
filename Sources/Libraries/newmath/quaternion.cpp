#include "quaternion.h"
#include "const.h"

namespace nm
{
	void hpb(const Quaternion &qu, Vector3 &result)
	{
		float x = qu.v.x;
		float y = qu.v.y;
		float z = qu.v.z;
		float w = qu.w;
		float& h = result.x;
		float& p = result.y;
		float& b = result.z;

		float test = x * y + z * w;
		if( !( test > 0.499f ||  test < -0.499f) )
		{
			float tx = 2.0f * x; 
			float ty = 2.0f * y;
			float o = 1.0f - 2.0f*z*z;
			h = atan2f(ty * w - tx * z, o - ty*y);
			p = -atan2f(tx * w - ty * z, o - tx*x);
			b = -asinf(2 * test);
		}
		else
		{
			// singularity at pole
			b = 0;
			h = 2 * atan2f(x, w);
			p = PI_2;

			if (test < -0.499f) // pole is south 
			{ 
				h = -h;
				p = -p;
			}
		}
	}

	float heading(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;

		if( !( test > 0.499f ||  test < -0.499f) )
		{
			return atan2f(2.0f*y*w - 2.0f*x*z, 1.0f-2.0f*y*y - 2.0f*z*z);
		}
		else
		{
			float r = 2.0f * atan2f(x, w);
			return ( test > 0.499f ) ? r : -r;// singularity at north or south pole
		}
	}

	float pitch(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;
		if( !( test > 0.499f ||  test < -0.499f) )
		{
			return -atan2f(2.0f*x*w - 2.0f*y*z, 1.0f - 2.0f*x*x - 2.0f*z*z);
		}
		else
		{
			return ( test > 0.499f ) ? PI_2 : -PI_2;// singularity at north or south pole
		}
	}

	float bank(const Quaternion& q)
	{
		float x = q.v.x;
		float y = q.v.y;
		float z = q.v.z;
		float w = q.w;

		float test = x * y + z * w;
		return !( test > 0.499f ||  test < -0.499f) ? asinf(2.0f * test) : 0.0f;
	}
}
