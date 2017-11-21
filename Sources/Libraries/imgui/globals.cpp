#pragma once

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