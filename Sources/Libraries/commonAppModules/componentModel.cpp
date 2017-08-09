// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "componentModel.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"
#include "developmentComponents/library.include.h"
#include "debugDrawComponents/library.include.h"
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

		ComponentsCore::template init<
			CommonComponents::Module 
			, TestComponents::Module
			, ReferencesComponents::Module
			, DebugDrawComponents::Module

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




// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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