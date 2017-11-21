#include "separator.h"
#include "../control.h"
#include "../controlTypes.h"

namespace imgui
{
	void vertical_separator(float width)
	{
		Control ctrl(controls::VERTICAL_SEPARATOR, Rect(0.0f, 0.0f, 0.0f, width));
	}
}