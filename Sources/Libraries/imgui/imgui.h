// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "viewports/library.include.h"

#ifdef USE_WINDOWS
#include "uniguidraw/library.include.h"
#endif

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




// Copyright (C) 2012-2018 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.