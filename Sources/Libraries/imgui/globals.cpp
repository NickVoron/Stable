// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "globals.h"
#include "imgui.h"

namespace imgui
{
	namespace detail
	{
		Mouse mouse;
		Keyboard keyboard;

		Resources::Font* font;
		Resources::Atlas* guiAtlas;

		bool worldToScreen(const Vector3& pos, Point2& point)
		{
			Viewports::Viewport* vp = imgui::core::currentViewportHandler.viewport();
			if (!vp)
			{
				vp = Viewports::viewports.first();
				if (vp == Viewports::viewports.end())
				{
					vp = 0;
				}
			}

			if (vp)
			{
				nm::index2 res;
				bool onScreen = vp->calculateCamera().screenPos(D3DXVECTOR3(pos.x, pos.y, pos.z), res);
						 
				point.x = res.x;
				point.y = res.y;

				return onScreen;
			}
			

			return false;
		}

		Vector3 billboardPosition(const Vector3& base, const Vector2& delta)
		{
			Viewports::Viewport* vp = imgui::core::currentViewportHandler.viewport();
			if (!vp)
			{
				vp = Viewports::viewports.first();
				if (vp == Viewports::viewports.end())
				{
					vp = 0;
				}
			}

			Vector3 result = base;
			if (vp)
			{
				auto camera = vp->calculateCamera();
				auto right = camera.state.orientation.GetXAxis();
				auto up = camera.state.orientation.GetYAxis();

				result += right * delta.x + up * delta.y;
			}

			return result;
		}
	}
}



// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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