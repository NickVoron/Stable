// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "debugDrawComponents.h"

#ifdef WIN32
#include "debugDrawing/library.include.h"
#include "imgui/library.include.h"
#endif

namespace DebugDrawComponents
{

void BaseDebugDrawer::linker()
{
	link(position);
}




void PositionDrawer::update()
{
#ifdef WIN32
	DebugDrawing::locator(position->state);

#endif
}







SphereDrawer::SphereDrawer()
{
#ifdef WIN32
	image::rgb(1.0f, 1.0f, 1.0f, color);
#endif
}

void SphereDrawer::Resource::properties(SphereDrawer& component)
{
	property("radius", component.radius);
	property("color", component.color);
	property("text", component.text);
}

void SphereDrawer::update()
{
#ifdef WIN32
	DebugDrawing::color(color);
	DebugDrawing::sphere(position->state.position, radius);
	if (!text.empty())
	{
		DebugDrawing::text(position->state.position, text);
	}	
#endif
}

}

namespace properties_inheritance
{
	void inherite(DebugDrawComponents::SphereDrawer& parent, DebugDrawComponents::SphereDrawer& child)
	{
		
	}
}




// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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