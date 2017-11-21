#pragma once

#include "debugDrawing/library.include.h"

#include "screenMouseHelper.h"


namespace AppModules
{

ScreenMouseHelper::Params::Params()
{
	frustumCalc = false;
	frustumDraw = false;
}


void ScreenMouseHelper::process(const Viewports::CalculatedCamera& camera, Params& params)
{
	unigui::Point2 mousePos(Input::mouseX(), Input::mouseY());
	
	auto mpos = nm::index2((int) mousePos.x, (int) mousePos.y);
	params.mouseRay = camera.worldRay(mpos);

	if(params.frustumCalc)
	{
		static unigui::Point2 point;

		if(Input::mouseL(true))
		{
			point = mousePos;
		}

		if(params.frustumDraw && Input::mouseL())
		{
			DebugDrawing::rect( unigui::Rect(point, mousePos - point) );
		}	

		params.mouseFrustum = camera.worldFrustum(nm::index2(point.x, point.y), mpos);
	}	
}


}