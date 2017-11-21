#include "pointConstraint.h"


void PointConstraint::Resource::properties(PointConstraint& component)
{
	property("deltaPosition", component.deltaPosition);
	property("parentPosition", component.parentPosition);
}

void PointConstraint::linker()
{
	link(objectPosition);
}

void PointConstraint::finalize()
{
	hpb = Vector3::zero;
}

void PointConstraint::update(float dt)
{
 	parentPosition([&state = objectPosition->state, this](auto pos)
	{
		state.position = pos.state.GetLocalVector(deltaPosition);
		state.orientation = pos.state.orientation;
	});

	State st = State::Identity();
	st.SetHPB(hpb);
	objectPosition->state.orientation *= st.orientation;
}




