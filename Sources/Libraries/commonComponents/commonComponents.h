#pragma once

#include "componentModel/library.include.h"

#include "position.h"
#include "linearMover.h"
#include "spawner.h"
#include "destructor.h"
#include "spawnTimer.h"
#include "destructTimer.h"
#include "swaper.h"
#include "swapTimer.h"

#include "timer.h"
#include "timerConfigurator.h"
#include "coloredComponents.h"
#include "pointConstraint.h"

#include "referencedComponents.h"

#include "logCommonComponents.h"


namespace CommonComponents
{
	typedef ComponentModel::Module<
			Position, 
			LinearMover, 
			AroundRotator,
			Spawner,
			Destructor, 
			TimerConfigurator, 
			BaseTimer, 
			SpawnTimer, 
			DestructTimer, 
			RedTimer, 
			BlueTimer, 
			Swaper, 
			SwapTimer, 
			PointConstraint
		> Module;
}



namespace TestComponents
{
	typedef ComponentModel::Module<Red, Blue, Green> Module;
}