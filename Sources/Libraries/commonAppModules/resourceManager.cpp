// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "resourceManager.h"
#include "resourceManager/library.include.h"
#include "resourceUtils/library.include.h"
#include "resourceModule/library.include.h"
#include "resourceText/library.include.h"
#include "scheme/library.include.h"

#ifdef WIN32
#include "imgui/library.include.h"
#include "imguiModules/library.include.h"
#include "resourceAtlas/library.include.h"
#include "resourceShaderGLES/library.include.h"
#include "resourceShader/library.include.h"
#endif

namespace Resources
{
#ifdef WIN32
	template<int shaders> struct has_external_editor<Shader<shaders>> { static const bool value = true; };
	template<> struct has_external_editor<ShaderGLSL> { static const bool value = true; };
	template<> struct has_external_editor<Atlas> { static const bool value = true; };
#endif
	template<> struct has_external_editor<Text> { static const bool value = true; };
	template<> struct has_external_editor<Module> { static const bool value = true; };
}


namespace AppModules
{
	bool commitAfterInit = false;
	bool copySources = false;
	bool rebuildResources = false;
	bool exitAfterInit = false;
	bool removeLocalSourcesAfterCompile = false;
	bool cleanCompiledResources = false;

	unsigned short resourceServerPort = 9000;

	void ResourcesManager::before_init_impl(const std::vector<std::string>& commandLine)
	{
		Resources::setupManager();
	}

#ifdef WIN32
	struct ResourcesEditor : public imgui::modules::Module
	{
		virtual void process()
		{
			Resources::Store& store = Resources::Store::get();





			for (auto& s : store)
			{
				if (s.associatedEditor())
				{
					if (imgui::checkbox(s.name(), values[s.name()]))
					{
						for (auto& res : s.resources())
						{

					
							imgui::button(Base::FileUtils::GetFileNameOnly(res.c_str()), [res]() { Resources::edit(res.c_str()); });
						}
					}
				}
			}

			
		}

		std::map<std::string, bool> values;

		virtual const char* name() const { return "Resources"; }

	};
#endif

	void always_recompile(const std::string& type)
	{
		if (type == "module")
		{
			Resources::Module::alwaysRecompile() = true;
			return;
		}
		else
		{
			LOG_ERROR("always-recompile not implemented for: " << type);
		}	
	}

	ResourcesManager::ResourcesManager()
	{
#ifdef WIN32
		static ResourcesEditor resourcesEditor;
#endif
	}

	void ResourcesManager::init_impl()
	{
		Resources::Store& store = Resources::Store::get();
		
		{
			
		}

		scheme::defun("always-recompile", &always_recompile);
	}

	void ResourcesManager::after_init_impl(bool& exit)
	{
		if(commitAfterInit)
		{
			Streaming::Streamer::commit();
		}

		if(cleanCompiledResources)
		{

		}

		if (exitAfterInit)
		{
			exit = exitAfterInit;
		}		
	}

	void ResourcesManager::release_impl()
	{
		Streaming::Streamer::release();
		Resources::Store::Destroy();
	}

	
	
	
	void ResourceWatcher::process()
	{
		if(!Streaming::Streamer::active())
		{
			Resources::Store::ReloadIfUpdate();
		}
	}

	void ResourceWatcher::before_init()
	{
		Resources::Store::get().load();		
	}

	void ResourceWatcher::release()
	{
		Resources::Store::get().save();
		Resources::Store::get().release();
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