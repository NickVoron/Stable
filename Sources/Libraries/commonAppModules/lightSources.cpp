// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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