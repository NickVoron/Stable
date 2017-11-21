#pragma once
#include "componentModel/library.include.h"
#include "position.h"

class PointConstraint : public UpdatableComponent<PointConstraint>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(PointConstraint);

	struct Resource : public ResourceBase
	{
		void properties(PointConstraint& component);
	};

	void linker();

	virtual void finalize() override;
	virtual void update(float dt);

	Vector3 hpb;

private:
	Position* objectPosition;
	Vector3 deltaPosition;	
	ComponentLink<Position> parentPosition;
};
