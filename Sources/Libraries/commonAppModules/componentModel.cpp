#include "componentModel.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"
#include "developmentComponents/library.include.h"
#include "debugDrawComponents/library.include.h"
#include "valueInterfaceComponents/library.include.h"
#include "componentModelUtils/library.include.h"
#include "componentModelConfigurator/library.include.h"
#include "resourceUtils/library.include.h"
#include "expressionFunctions/library.include.h"

namespace AppModules
{
	struct EntityArenaResources : std::list<Resources::Module*>
	{
		Resources::Module* create(const char* fileName)
		{
			emplace_back(nullptr);
			Resources::load(back(), fileName);
			return back();
		}

		void execute()
		{
			for (auto&& res : *this)
			{
				res->execute();
			}
		}
	};

	EntityArenaResources arena;

	void load_module(const char* fileName)
	{														  		
		arena.create(fileName);
	}

	EntityArena imarena;

	void load_module_im(const char* fileName)
	{
		auto& entities = ::ComponentModel::descriptionLoad(Resources::resourceRelativePath(fileName).c_str(), imarena.create());
		entities.activate(true);
	}

	void ComponentModel::init()
	{
		static ComponentsEditor::EntityEditor editor;

		::ComponentModel::template init<
			CommonComponents::Module 
			, TestComponents::Module
			, LogComponents::Module
			, ReferencesComponents::Module
			, DebugDrawComponents::Module
			, DevelopmentComponents::Module
			, ValueInterfaceComponents::Module
		>();

		ComponentsEditor::init();
		ExpressionFunctions::InitFunctionsLibrary();

		scheme::defun("load-module", &load_module);
	}

	void ComponentModel::process(float dt)
	{
		BaseTimer::parameters(dt);
		arena.execute();
	}
}
