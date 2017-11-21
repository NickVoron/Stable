#include "resource.h"
#include "resourceSet.h"
#include "store.h"
#include "math/library.include.h"
#include "streaming/library.include.h"
#include "common/exception_safety.h"

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <boost/filesystem.hpp>

namespace Resources {

	void commit()
	{
		Streaming::Streamer::commit();
	}
	
	//
	//
	//
	void ResourcePath::getRelativeSourceFileName(str::string256& path) const
	{ 
		path += storedPath();
		//path += "/";
		path += localFileName;
	}

	void ResourcePath::getLocalSourceFileName(str::string256& path) const
	{
		path += localFileName;
	}

	void ResourcePath::getFullCompiledFileName(str::string256& path)		const
	{ 
		path += Store::get().compiledPath;
		path += "/";
		path += storedPath();
		path += localFileName;

		Base::FileUtils::create_dir_for_file(path.c_str());

		path += ".";
		path += id.string();
		path += ".compiled";
	}

	void ResourcePath::getFullSourceFileName(str::string256& path) const
	{		

		path.clear(); getCommonSourceFileName(path); bool inCommon = boost::filesystem::exists(path.c_str());
		path.clear(); getDevelopmentSourceFileName(path); bool inDevelopment = boost::filesystem::exists(path.c_str());
		path.clear(); getRelativeSourceFileName(path); bool inProduct = boost::filesystem::exists(path.c_str());

		if ((inCommon && inDevelopment) || (inDevelopment&&inProduct) || (inCommon&&inProduct))
		{
			LOG_WARNING("Duplication of resources in common or in development or in local paths");
 		}

		path.clear(); getCommonSourceFileName(path); if (boost::filesystem::exists(path.c_str())) return;
		path.clear(); getDevelopmentSourceFileName(path); if (boost::filesystem::exists(path.c_str())) return;
		path.clear(); getRelativeSourceFileName(path); if (boost::filesystem::exists(path.c_str())) return;
	}

	void ResourcePath::getDevelopmentSourceFileName(str::string256& path) const
	{
		path += Store::get().developmentPath;
		path += "/";
		getRelativeSourceFileName(path);
	} 
	
	void ResourcePath::getCommonSourceFileName(str::string256& path) const
	{
		path += Store::get().commonPath;
		path += "/";
		getRelativeSourceFileName(path);
	}


	void ResourcePath::getFullSourceFolder(str::string256& path) const
	{
		boost::filesystem::path pth = sourceFileName.c_str();
		pth.remove_filename();
		path = pth.string().c_str();
	}

	void ResourcePath::AssignFileName(const char* newFileName)
	{
		if (localFileName.empty())
		{
			localFileName = boost::filesystem::path(newFileName).lexically_normal().string().c_str();

			getFullSourceFileName(sourceFileName);
			sourceFileName = boost::filesystem::path(sourceFileName.c_str()).lexically_normal().string().c_str();

			getFullCompiledFileName(compiledFileName);
			compiledFileName = boost::filesystem::path(compiledFileName.c_str()).lexically_normal().string().c_str();
		}		
	}


	//
	//
	//
	Resource::~Resource()
	{
		
	}

	void Resource::RegisterAndLoad(const char* path, const char* fileName, std::unique_ptr<UserData> userData)
	{
		AssignFileName(fileName);
		Load(std::move(userData));
		Store::Register(this);
	}

	void Resource::CompileTo(const char* fileName, const UserData* userData, stream::ostream& os)
	{
		AssignFileName(fileName);
		Clear();
		LoadSource(sourceFileName.c_str(), userData);
		Compile(os);
		Clear();
	}

	void Resource::ConstructID(ID& id, const char* name, const UserData* userData)
	{
		id.data(name, userData);
	}

	void Resource::Load(std::unique_ptr<UserData> userData)
	{
		try
		{
 			Clear();

			addDependenciesFromTimeStamp();

			userDataBlock = std::move(userData);

			BaseResourceLoader<Resource>::Params params;
			params.sourceFileName = sourceFileName.c_str();
			params.compiledFileName = compiledFileName.c_str();
			params.resourceToLoad = this;
			params.userData = userDataBlock.get();

			bool loaded = false;
			if( !IsCompiledFileIsActual() )
			{
				Streaming::Streamer::add<SourceResourceLoader<Resource>>(params);
				loaded = true;
				saveActualTimeStamp();	
			}
			
			if (!loaded)
			{
				Streaming::Streamer::add<CompiledResourceLoader<Resource>>(params);
			}			
		}
		catch (std::exception& e)
		{
			LOG_ERROR("can't load or compile resource: " << sourceFileName.c_str() << " last error: " << e);
		}
	}

	void Resource::Reload()
	{
		Load(std::move(userDataBlock));
	}

	void Resource::UpdateCompiled()
	{
		stream::ostream* os = IOStreamsManager::writer(compiledFileName.c_str());
		Compile(*os);
		IOStreamsManager::close(os);
	}

	bool Resource::ReloadIfUpdate()
	{
		bool modified = false;
		try_execute([&modified, this]
		{
			modified = dependencies.checkActualModified();
			if (modified)
			{
				Reload();
			}
		});

		return modified;
	}

	bool Resource::IsCompiledFileIsActual()
	{
		if( !boost::filesystem::exists(sourceFileName.c_str()) )
		{
			return boost::filesystem::exists(compiledFileName.c_str());
		}
		
		if (!boost::filesystem::exists(compiledFileName.c_str()))
		{
			return false;
		}

		return (!Store::watchChanges() || (Store::watchChanges() && dependencies.equalWithStored())) && !Store::alwaysRecompile();
	}


	void Resource::saveActualTimeStamp()
	{
		if(Store::watchChanges())
		{
			dependencies.setActualRoot(sourceFileName.c_str());
			dependencies.refreshStored();
			Store::get().storeDependencies(sourceFileName.c_str(), dependencies.stored);
			Store::get().save();
		}		
	}

	void Resource::addDependenciesFromTimeStamp()
	{
		if(Store::watchChanges())
		{
			Store::get().restoreDependencies(sourceFileName.c_str(), dependencies.stored);
			ENFORCE(dependencies.stored.root.absoluteFileName.empty() || sourceFileName.c_str() == dependencies.stored.root.absoluteFileName);
			dependencies.refreshActual();	
		}
	}

	//
	//
	//
	void ResourcePath::ID::data(const char* name, const UserData* userData)
	{
		crc0 = crc::CRC32(name, strlen(name));
		crc1 = userData ? userData->hash() : 0;
	}

	bool ResourcePath::ID::equal(const ID& id) const
	{
		return (id.crc0 == crc0) && (id.crc1 == crc1);
	}

	str::string32 ResourcePath::ID::string() const
	{
		return str::stringize(crc0, crc1).c_str();
	}
}

