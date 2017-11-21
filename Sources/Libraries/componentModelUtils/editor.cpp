#include "editor.h"
#include "io.h"

#ifdef WIN32
#include "imgui/library.include.h"
#endif

namespace ComponentsEditor
{
	void editor_int::edit(const char* name, int& val)
	{

	}

	void editor_bool::edit(const char* name, bool& val)
	{
#ifdef WIN32
		imgui::checkbox(name, val);
#endif
	}

	void editor_float::edit(const char* name, float& val)
	{
#ifdef WIN32
		imgui::slider(name, 0.0f, 100.0f, val);
#endif
	}

	void editor_color::edit(const char* name, image::Color<image::RGBA32F>& val)
	{
#ifdef WIN32
		imgui::color_rgba(name, val);
#endif
	}

	void editor_vector3::edit(const char* name, Vector3& val)
	{
#ifdef WIN32
		imgui::slider(name, 0.0f, 100.0f, val.x);
		imgui::slider(name, 0.0f, 100.0f, val.y);
		imgui::slider(name, 0.0f, 100.0f, val.z);		
#endif
	}

	void edit(EntityData* entity)
	{
#ifdef WIN32
		ClassResources resource;
		resource.create(*entity->cls);
		edit(resource);
#endif
//		resource.flush(*entity);
	}

	void init()
	{
		PropertyEditors::addInst<editor_int, editor_bool, editor_float, editor_color, editor_vector3>();
	}

	EntityEditor::EntityEditor()
		:entities(0), currentEntity(0), active(true)
	{
	}

	void EntityEditor::process()
	{
#ifdef WIN32
		if(entities)
		{
			const char* saveFile = "data.dat";
			if(imgui::button("save"))
			{
				ComponentsIO::save(saveFile, *entities);
			}

			if(Base::FileUtils::FileExists(saveFile) && imgui::button("load"))
			{
				ComponentsIO::load(saveFile, *entities);
			}

			if(imgui::button("clear"))
			{
				entities->clearObjects();
			}

			if(imgui::button(active ? "deactivate" : "activate"))
			{
				active = !active;
				entities->activate(active);
			}

			// 			if(currentEntity && imgui::button("destroy"))
			// 			{
			// 				((Entity*)currentEntity)->destroy();
			// 			}

			for(int i = 0; i < entities->classes.classes.size(); ++i)
			{
				Class& cls = *entities->classes.classes[i];

				if(imgui::button(cls.name().c_str()))
				{
					for (int i = 0; i < 1; ++i)
					{
						Entity* e = entities->create(cls.name());
						e->finalize();
						currentEntity = e;
					}				

					LOG_ERROR(entities->size());

					entities->activate(active);
				}
			}			

/*
			imgui::ListResult lr = imgui::list("list", sources, 5);
			if(lr.selectionChange)
			{
				load(sources[lr.selectedIndex]);
			}*/

		}

		if(currentEntity)
		{
			//	edit(currentEntity);
		}
#endif
	}

	const char* EntityEditor::name() const
	{
		return "Entity Editor";
	}

}
