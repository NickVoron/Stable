#include "holder.h"

#include "debugDrawing/library.include.h"

namespace CameraComponents
{

CameraParamHolder::CameraParamHolder()
{
	CameraSystem::cameras().insert(camera);
}

CameraParamHolder::~CameraParamHolder()
{
	camera.remove();
}

void CameraParamHolder::Resource::properties()
{
	property("position", position);
	property("hpb", hpb);
	property("near", nearDist);
	property("far", farDist);
	property("verticalFov", verticalFov);
	property("aspectRatio", aspectRatio);
}

void CameraParamHolder::linker()
{
	link(position);
}

// void CameraParamHolder::load(const Resource* res)
// {
// 	camera.params.getStateRef().SetHPB(res->hpb);
// 	camera.params.getStateRef().position = res->position;
// 	camera.params.setAspect(res->aspectRatio);
// 	camera.params.setFovV(nm::radians(res->verticalFov) );
// 	camera.params.getPlanesRef() = Base::Interval<float>(res->nearDist, res->farDist);
// }
// 
// void CameraParamHolder::save(Resource* res) const
// {
// 	res->hpb				= camera.params.getState().GetHPB();
// 	res->position		= camera.params.getState().position;
// 	res->aspectRatio		= camera.params.getAspect();
// 	res->verticalFov		= nm::degrees(camera.params.getFovV());
// 	res->nearDist		= camera.params.getData().planes.minValue;
// 	res->farDist			= camera.params.getData().planes.maxValue;
// }

void CameraParamHolder::update(float dt)
{
	INCOMPLETE;
	CameraSystem::Affectors::VehicleView& vehicleView = camera.affectors.affectors.get<CameraSystem::Affectors::VehicleView>();

	vehicleView.vehicleState = position->state;
	vehicleView.cameraDelta = Vector3(0, 1, 0.8);

//	camera.focusTo(position->state.position);
//	camera.lookTo(position->state.position + position->state.orientation.GetZAxis()*3);
	camera.calculate(dt);

	
//	DebugDrawing::camera(camera, 0.25f, false);
// 	LOG_REPORT("target: " << target.state.position)
// 
// 	DebugDrawing::color_rgb(1, 1, 0);
// 	DebugDrawing::wireBox(position->state.position);
}


}