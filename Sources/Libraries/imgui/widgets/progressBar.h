#pragma once

#include "../types.h"

namespace imgui
{
	void progressBar(const Point2& pos, int inputFieldLength, int length, float minVal, float maxVal, float& val);
}