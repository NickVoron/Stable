#pragma once

#include "commonComponents/library.include.h"

namespace StearingComponents
{

class StearingTargetsHolder;
class StearingTarget : public CustomComponent<StearingTarget, StearingTargetsHolder>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(StearingTarget);

	void linker();

	Position* position;
};

class StearingTargetsHolder : public SimpleComponentInputParameters<StearingTarget, void>
{
public:
	StearingTarget* findNearest(const Vector3& position);
};

class NearestTargetSearcher : public UpdatableComponent<NearestTargetSearcher>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(NearestTargetSearcher);

	NearestTargetSearcher();
	void update(float dt);
	void linker();

	Position* position;

	ComponentLink<StearingTarget> target;
};

class SeekStearing : public UpdatableComponent<SeekStearing>
{
public:
	CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(SeekStearing);

	SeekStearing();
	void update(float dt);
	void linker();						  

	Position* position;
	LinearMover* mover;
	NearestTargetSearcher* searcher;
};

typedef ComponentModel::Module<SeekStearing, StearingTarget, NearestTargetSearcher> Module;

}