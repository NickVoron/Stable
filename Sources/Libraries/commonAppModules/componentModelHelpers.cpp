#include "componentModelHelpers.h"

#include "componentModel/library.include.h"
#include "developmentComponents/library.include.h"
#include "debugDrawComponents/library.include.h"

namespace AppModules
{
	void ComponentModelHelpers::init()
	{
//		ComponentModel::template init<DebugDrawComponents::Module, DevelopmentComponents::Module>();
	}

	void ComponentModelHelpers::process_impl(const intersections::Ray& mouseRay)
	{
//		VisualSwitcher::parameters(mouseRay);
	}
}
