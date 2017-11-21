#pragma once

#include "commonComponents/library.include.h"

class PositionLink : public UpdatableComponentAutoLink<PositionLink, Position>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(PositionLink);

	struct Resource : public ResourceBase
	{
		void properties(PositionLink& component);
	};

	void update();

	ComponentLinkList<Position> target;
};
