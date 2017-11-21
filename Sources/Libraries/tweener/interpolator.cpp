#include "interpolator.h"

namespace tween
{
	Quaternion lerp(const Quaternion& v0, const Quaternion& v1, float t)
	{
		return Quaternion::Slerp(v0, v1, t);
	}

	State lerp(const State& v0, const State& v1, float t)
	{
		State result = v0;
		result.position = nm::lerp(v0.position, v1.position, t);
		result.orientation = Quaternion::Slerp(v0.orientation, v1.orientation, t);
		return result;
	}
}