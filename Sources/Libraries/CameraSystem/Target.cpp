#include "target.h"

namespace CameraSystem
{
	State Target::state() const 
	{ 
		return State::Identity();
	}

	float Target::radius() const 
	{ 
		return 0.0f; 
	}

	//
	//
	//
	PointTarget::PointTarget()
	{
		point.Zero();
		r = 0.0f;
	}

	State PointTarget::state() const
	{
		return State::Simple(point);
	}

	float PointTarget::radius() const
	{
		return r;
	}
}