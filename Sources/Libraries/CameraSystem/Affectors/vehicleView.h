#pragma once
#include "../affector.h"


namespace CameraSystem {
	namespace Affectors {

		//
		//
		//
		struct VehicleView : public Affector
		{	 

			VehicleView();

			State vehicleState;
			Vector3 cameraDelta;


			virtual void apply(float dt, Affector::Context& ctx);
		private:
			
		};


	}
}