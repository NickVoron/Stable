#pragma once

#include "componentModel/library.include.h"

class Position : public DataComponent<Position>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Position);

	struct Resource : public ResourceBase 
	{
		void properties(Position& component);
	};
	
	State state;
};

namespace properties_inheritance
{
	void inherite(Position& parent, Position& child);
}