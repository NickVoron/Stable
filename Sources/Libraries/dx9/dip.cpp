#pragma once

#include "dip.h"
#include "device.h"

namespace dx9
{

void DrawingStats::resetCounters()
{
	callCounter = 0;
	primitivesCounter = 0;
}

int DrawingStats::callCounter = 0;
int DrawingStats::primitivesCounter = 0;

}