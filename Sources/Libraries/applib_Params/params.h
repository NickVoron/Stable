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