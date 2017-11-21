#pragma once

#include "valueInterface.h"
#include "valueOperation.h"

namespace ValueInterfaceComponents
{
	typedef ComponentModel::Module<
		  ValueInterface
		, ValueInterfaceMonitor
		, ValueOperation
		, OperationsTick
		, OpSource
		, OpTarget
		, OpAdd
	> Module;
}