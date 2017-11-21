#pragma once

#include "../affector.h"

namespace CameraSystem{
namespace Affectors{

	class HeightMapConstraint : public Affector 
	{
	public:
		struct HeightMapInterface 
		{
			virtual float height(float x, float z) const = 0;
		};

		struct Data
		{
			Data();
			void range(Vector3& v);

			Range<float> h;
			HeightMapInterface* height;
		};

		virtual void apply(float dt, Affector::Context& ctx);

		Data data;
	};

}
}