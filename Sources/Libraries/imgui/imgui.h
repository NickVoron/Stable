#pragma once

#include "viewports/library.include.h"
#include "uniguidraw/library.include.h"

#include "types.h"
#include "globals.h"

#include "layout/layout.h"
#include "layout/area.h"
#include "layout/shift.h"
#include "layout/transform.h"

#include "widgets/button.h"		
#include "widgets/panel.h"
#include "widgets/checkBox.h"
#include "widgets/slider.h"
#include "widgets/label.h"
#include "widgets/separator.h"
#include "widgets/color.h"
#include "widgets/color_ext.h"
#include "widgets/list.h"
#include "widgets/image.h"
#include "widgets/combobox.h"
#include "widgets/progressBar.h"

namespace imgui
{
namespace core
{
	extern Viewports::ViewportHandle currentViewportHandler;

	void init();
	void reload_render_target();
	void begin(Viewports::Viewport* viewport);
	void end();
	void checked_begin();
	void checked_end();

	void process(const Mouse& mouse, const Keyboard& keyboard, float dt);
	void destroy();
	bool mouseProcessed();

	void draw();
	
}	
}