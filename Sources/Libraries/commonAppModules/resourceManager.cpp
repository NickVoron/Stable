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

// 			imgui::button_if(store.isServer() || store.isClient(), "StopNetwok", [&store]() { store.stop(); });
// 			imgui::button_if(!store.isClient() && !store.isServer(), "ResourceClient", [&store]() { store.connectToResourcesServer(resourceServerAddr); });
// 			imgui::button_if(store.isClient(), "Sync Resources", [&store]() { store.loadedResourcesSync(); });

			for (auto& s : store)
			{
				if (s.associatedEditor())
				{
					if (imgui::checkbox(s.name(), values[s.name()]))
					{
						for (auto& res : s.resources())
						{
/*
							auto help = res->help();

							if (!help.empty())
							{
								if (imgui::button("help"))
								{
									{
										std::ofstream hos("help.txt");

										for (auto& h : help)
										{
											hos << h << std::endl;
										}
									}

									Resources::edit("help.txt");
								}
							}*/
					
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
		//if (!store.startResourcesServer(resourceServerPort))
		{
			//store.connectToResourcesServer(resourceServerAddr);
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
//			Resources::cleanCompiledLocalResources();
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

	//
	//
	//
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