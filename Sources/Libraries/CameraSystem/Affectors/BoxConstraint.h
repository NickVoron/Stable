#pragma once

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	struct BoxConstraint : public Affector
	{
		struct Data
		{
			Data();
			void range(Vector3& v);

			Range<float> x;
			Range<float> y;
			Range<float> z;
			bool enabledX;
			bool enabledY;
			bool enabledZ;
		};

		virtual void apply(float dt, Affector::Context& ctx);

		Data data;
	};

}
}