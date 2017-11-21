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