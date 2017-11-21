#pragma once

#include "math/library.include.h"

namespace CameraSystem
{
	struct Target
	{
		virtual State state() const;
		virtual float radius() const;
	};

	struct PointTarget : public Target
	{
		PointTarget();

		virtual State state() const;
		virtual float radius() const;

		Vector3 point;
		float r;
	};
}