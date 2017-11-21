#pragma once

#include "libopenal/library.include.h"
#include "math/library.include.h"


namespace openal
{
	struct listener
	{
		static float gain();
		static void gain(float value);
		static void position(const Vector3& pos);
		static void velocity(const Vector3& pos);
		static void orientation(const Vector3& dir, const Vector3& up);
	};
}

