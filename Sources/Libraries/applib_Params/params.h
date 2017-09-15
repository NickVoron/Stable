// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "viewports/library.include.h"
#include "windowsGUI/library.include.h"

struct StdAppParams
{
	template<class Params>
	StdAppParams(Params& params)
		:dt(params.dt),
		timeFactor(params.timeFactor),
		commandLine(params.commandLine),
		currentCamera(params.currentCalculatedCamera),
 		directionalDir(params.directionalDir),
 		directionalColor(params.directionalColor),
 		ambientColor(params.ambientColor),
		mouseRay(params.mouseRay),
		mouseFrustum(params.mouseFrustum),
		drawCoordinatesGrid(params.drawCoordinatesGrid),
		drawLightSources(params.drawLightSources),
		windowsParams(params.windowsParams),
		mouseFrustumCalc(params.frustumCalc),
		mouseFrustumDraw(params.frustumDraw)
	{}

	float dt;
	float timeFactor;
	std::vector<std::string>& commandLine;
	Viewports::CalculatedCamera& currentCamera;
	intersections::Ray& mouseRay;
	intersections::Frustum& mouseFrustum;
	bool& mouseFrustumCalc;
	bool& mouseFrustumDraw;
 	nm::Vector3& directionalDir;
	nm::Vector3& directionalColor;
	nm::Vector3& ambientColor;
	bool& drawCoordinatesGrid;
	bool& drawLightSources;
	WindowsGUI::WindowsRegistry& windowsParams;
};



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