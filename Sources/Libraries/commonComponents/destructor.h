#pragma once

#include "componentModel/library.include.h"

class Destructor : public SimpleComponentAutoLink<Destructor, Entity>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(Destructor);

	void destruct();
};
