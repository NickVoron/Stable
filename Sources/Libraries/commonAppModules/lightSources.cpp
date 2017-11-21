#include "lightSources.h"
#include "CameraSystem/library.include.h"
#include "debugDrawing/library.include.h"
#include "imgui/library.include.h"

namespace AppModules
{
	struct LightSourcesEditor : public imgui::modules::Module
	{
		LightSourcesEditor(DefaultLightSources& prnt) : parent(prnt) { }

		virtual void process()
		{
			imgui::checkbox("Draw Light Sources", parent.params->drawLightSources);
			imgui::checkbox("Light From Camera", parent.params->lightFromCamera);

			auto& amb = parent.ambient->energy;
			auto& dir = parent.directional->energy;
			imgui::color_rgb("Ambient Color", amb.x, amb.y, amb.z);
			imgui::color_rgb("Directional Color", dir.x, dir.y, dir.z);
		}

		virtual const char* name() const	{	return "LightSourcesEditor";	}

		DefaultLightSources& parent;
	};

	DefaultLightSources::Params::Params()
	{
		directionalDir = nm::Vector3(1, 1, 1);
		directionalColor = nm::Vector3(1, 1, 1);
		ambientColor = nm::Vector3(1, 1, 1)*0.4f;
		drawLightSources = false;
		lightFromCamera = false;
	}

	DefaultLightSources::DefaultLightSources()
	{
	}
	
	CameraSystem::AroundCamera light;

	void DefaultLightSources::init()
	{
		static LightSourcesEditor editor(*this);

		directional = light::Directional::create();
		ambient = light::Ambient::create();
		
		SettingsRegistry::get("defaultDirectionalLight", light.params);
		SettingsRegistry::get("defaultDirectionalColor", directional->energy);
		SettingsRegistry::get("defaultAmbientColor", ambient->energy);
	}

	void DefaultLightSources::process(float dt, const Viewports::CalculatedCamera& camera)
	{	
		light.input.heading(-Input::delta(Input::MOUSE_AXIS_X, Input::MOD_MOUSE_BUTTON_LEFT | Input::MOD_KEY_LALT));
		light.input.pitch(Input::delta(Input::MOUSE_AXIS_Y, Input::MOD_MOUSE_BUTTON_LEFT | Input::MOD_KEY_LALT));
		light.input.update(dt);
		light.calculate(dt);
		
		directional->direction = light.params.getState().orientation.GetZAxis();

		auto& dir = directional->direction;
		auto from = dir * 5.0f;
		auto to = from - dir * 0.3f;

		if(params->drawLightSources)
		{
			//DebugDrawing::color(directional->energy);
			DebugDrawing::arrow(from, to, 0.05f);
		}	

		params->directionalDir = params->lightFromCamera ? -camera.direction : dir;
		params->directionalColor = directional->energy;
		params->ambientColor = ambient->energy;
	}

	void DefaultLightSources::release()
	{
		SettingsRegistry::add("defaultDirectionalLight", light.params);
		SettingsRegistry::add("defaultDirectionalColor", directional->energy);
		SettingsRegistry::add("defaultAmbientColor", ambient->energy);

		light::Directional::destroy(directional);
		light::Ambient::destroy(ambient);
	}

}