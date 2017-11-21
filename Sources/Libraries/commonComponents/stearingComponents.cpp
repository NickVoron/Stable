#include "stearingComponents.h"

namespace StearingComponents
{

SeekStearing::SeekStearing()
{
	
}

void SeekStearing::update(float dt)
{
// 	if(searcher->target)
// 	{
// 		mover->velocity = (searcher->target->position->state.position - position->state.position);
// 	}
// 	else
// 	{
// 		mover->velocity = Vector3::zero;
// 	}
}

void SeekStearing::linker()
{
	link(position);
	link(mover);
	link(searcher);
}


//
//
//
void StearingTarget::linker()
{
	link(position);
}

StearingTarget* StearingTargetsHolder::findNearest(const Vector3& position)
{
	INCOMPLETE;
	return 0;

/*
	float resDist = FLT_MAX;
	StearingTarget* res = 0;
	for(StearingTarget* c = first(); c != end(); c->iterate(c))
	{	
		float dist = (c->position->state.position - position).MagnitudeSquared();
		if( dist < resDist )			
		{
			resDist = dist;
			res = c;
		}
	}

	return res;*/
}


//
//
//
NearestTargetSearcher::NearestTargetSearcher()
{

}

void NearestTargetSearcher::linker()
{
	link(position);
}

void NearestTargetSearcher::update(float dt)					         
{
//	INCOMPLETE;

// 	if(!target)
// 	{
// 		target = StearingTarget::manager().findNearest( position->state.position );
// 
// 		if(target)
// 		{
// 			LOG_ERROR( "target: " << target->position->state.position );	
// 		}
// 	}
}

}


