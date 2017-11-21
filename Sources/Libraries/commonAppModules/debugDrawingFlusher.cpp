#include "debugDrawingFlusher.h"

#include "uniguidraw/library.include.h"
#include "debugDrawing/library.include.h"

namespace AppModules
{

	void DebugDrawingFlusher::process()
	{
 		unigui::draw::flush();
 		DebugDrawing::flush();
	}
}