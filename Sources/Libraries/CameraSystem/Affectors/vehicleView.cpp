#include "vehicleView.h"

namespace CameraSystem {
namespace Affectors {


VehicleView::VehicleView():
	vehicleState( State::Identity()),
	cameraDelta(0, 0, 0)
{
	
}

void VehicleView::apply(float dt, Affector::Context& ctx) 
{
	State& s = ctx.state();

	Vector3 delta = vehicleState.orientation.Transform(cameraDelta);

	s.position = vehicleState.position + delta;
	s.orientation = vehicleState.orientation;
}


}//
}//