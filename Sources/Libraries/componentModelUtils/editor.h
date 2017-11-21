#pragma once

#ifdef WIN32
#include "imguiModules/library.include.h"
#endif
#include "componentModel/library.include.h"
#include "image/library.include.h"

namespace ComponentsEditor
{
	struct editor_int : public PropertyEditor<int>
	{
		virtual void edit(const char* name, int& val);
	};

	struct editor_bool : public PropertyEditor<bool>
	{
		virtual void edit(const char* name, bool& val);
	};

	struct editor_float : public PropertyEditor<float>
	{
		virtual void edit(const char* name, float& val);
	};

	struct editor_color : public PropertyEditor< image::Color<image::RGBA32F> >
	{
		virtual void edit(const char* name, image::Color<image::RGBA32F>& val);
	};

	struct editor_vector3 : public PropertyEditor<Vector3>
	{
		virtual void edit(const char* name, Vector3& val);
	};

	void edit(EntityData* entity);
	void init(); 

	struct EntityEditor 
#ifdef WIN32
		: public imgui::modules::Module
#endif
	{
		EntityEditor();

		virtual void process();
		virtual const char* name() const;

		bool active;
		EntityData* currentEntity;
		EntitiesList* entities;
	};
}



