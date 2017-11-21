#pragma once
#include "position.h"

class LinearMover : public UpdatableComponentAutoLink<LinearMover, Position>
{
public:
  	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(LinearMover);

	struct Resource : public ResourceBase 
	{
		void properties(LinearMover& component);
	};

	void update(float dt);

	Vector3 velocity;
	bool global = false;
};

class AroundRotator : public UpdatableComponentAutoLink<AroundRotator, Position>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(AroundRotator);

	struct Resource : public ResourceBase
	{
		void properties(AroundRotator& component);
	};

	void update(float dt);

	ComponentLink<Position> target;
	float velocity;
};
