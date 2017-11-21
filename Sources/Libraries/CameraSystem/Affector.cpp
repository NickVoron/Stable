#include "affector.h"

namespace CameraSystem
{
	void AffectorsSet::apply(float dt, Affector::Context& ctx)
	{
		for(auto& affector : *this)
		{
			affector.apply(dt, ctx);
		}
	}

	void AffectorsSet::init(Affector::Context& ctx)
	{
		for (auto& affector : *this)
		{
			affector.init(ctx);
		}
	}
}