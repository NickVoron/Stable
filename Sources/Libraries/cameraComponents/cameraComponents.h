#pragma once

#include "affectors/include.h"
#include "cameraSetter.h"

namespace CameraComponents
{
	typedef ComponentModel::Module<
		CameraParamHolder, 
		FreeRotator, 
		Forward, 
		HorizVert, 
		Around, 
		DistanceConstraint, 
		ToTargetConstraint, 
		BoxConstraint, 
		ThirdPerson, 
		CameraSetter> 
		Module;
}
