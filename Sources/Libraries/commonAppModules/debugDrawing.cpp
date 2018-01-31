// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "debugDrawing.h"

namespace AppModules
{
	struct DebugDrawingModuleEditor : public imgui::modules::Module
	{
		DebugDrawingModuleEditor(DebugDrawingModuleBase::Params& p):params(p){}

		virtual void process();
		virtual const char* name() const;

		DebugDrawingModuleBase::Params& params;
	};

	void DebugDrawingModuleEditor::process()
	{
		imgui::checkbox("Enabled", DebugDrawing::enabled());
		imgui::checkbox("Draw grid", params.drawCoordinatesGrid);
	}

	const char* DebugDrawingModuleEditor::name() const
	{
		return "Debug drawing";
	}

	DebugDrawingModuleBase::Params::Params()
	{
		drawCoordinatesGrid = true;
		debugDrawShader = "common/bodies.fx";
		debugFont = "Arial.ttf";
	}

	void DebugDrawingModuleBase::init(Params& params, gapi::Id id)		
	{ 
		static DebugDrawingModuleEditor editor(params);

		DebugDrawing::init(params.debugDrawShader, params.debugFont, id); 

		SettingsRegistry::get("DebugDrawing", params.drawCoordinatesGrid);
	};

	void DebugDrawingModuleBase::destroy(Params& params)	
	{ 
		SettingsRegistry::add("DebugDrawing", params.drawCoordinatesGrid);

		DebugDrawing::release();
	};

	void DebugDrawingModuleBase::process(bool drawCoordinatesGrid)			
	{ 
		if(drawCoordinatesGrid)
		{
			DebugDrawing::coordinatesGrid(Base::Interval<int>(-50, 50), 1.0f);
		}
	};
}



// Copyright (C) 2012-2014 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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