#pragma once
// ќграничение ориентации - смотреть только на цель

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct ToTargetConstraint : public Affector
	{
		virtual void apply(float dt, Affector::Context& ctx);
	};

}
}