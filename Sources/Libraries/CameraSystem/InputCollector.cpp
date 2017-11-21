#include "inputCollector.h"

namespace CameraSystem
{
	InputCollector::Params::Params()
	{
		defaults();
	}

	void InputCollector::Params::defaults()
	{
		hpb.Zero();
		pos.Zero();
	}

	void InputCollector::update(float dt)
	{
		current = accumulated;
		accumulated.defaults();
	}

	void InputCollector::heading(float delta)		{ accumulated.hpb.x += delta; }
	void InputCollector::pitch(float delta)			{ accumulated.hpb.y += delta; }
	void InputCollector::bank(float delta)			{ accumulated.hpb.z += delta; }

	void InputCollector::horizontal(float delta)	{ accumulated.pos.x += delta; }
	void InputCollector::vertical(float delta)		{ accumulated.pos.y += delta; }
	void InputCollector::forward(float delta)		{ accumulated.pos.z += delta; }

}

