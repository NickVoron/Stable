#pragma once

#include <vector>
#include <map>

#include <boost/filesystem.hpp>

#include "streaming/library.include.h"
#include "stuff/library.include.h"
#include "containers/library.include.h"
#include "strings/library.include.h"

#include "errors.h"
#include "dependencies.h"
#include "netdriver.h"

namespace Resources
{
	class Resource;
	class ResourceSetBase;

	struct SystemLockGuard : std::lock_guard<std::recursive_mutex> 
	{ 
		SystemLockGuard(); 
	};

	//
	//
	//
	class ChangeWatcher
	{
	public:
		ChangeWatcher();
		void load();
		void save();
		void release();

		void storeDependencies(const char* fullSourcePath, const Dependencies::StoredState& ss);
		void restoreDependencies(const char* fullSourcePath, Dependencies::StoredState& ss);

	protected:
		bool process();
		void saveTimeStampsRegistry();
		void loadTimeStampsRegistry();
		void add(Resource* res);
		void add(const char* path);
		void clear();

		virtual bool reloadIfUpdate() = 0;

	private:
		struct FileHandleWrapper
		{
			FileHandleWrapper(const char* fileName);
			~FileHandleWrapper();

			std::string fileName;
#ifdef WIN32
			HANDLE handle;
#endif
		};
		
		std::list<FileHandleWrapper> modifyHandles;
		std::map<std::string, Dependencies::StoredState> timeStampsRegistry;
	};

	//
	//
	//
	class Store : 
		public ChangeWatcher, 
		public Base::IntrusiveList<ResourceSetBase>, 
#ifdef NET_RES_DRIVER
		public HostApplication,
#endif
		public Base::MeyersSingleton<Store>
	{
	public:
		typedef Base::bimap<str::string32, int> FactoryDict;

		Store() {}
		static bool ReloadIfUpdate();
		static void Clear();
		static void Destroy();
		static void Register(Resource* res);

		static bool isValidResource(Resource* res);
		static void dictionary(FactoryDict& dict);

		static void setPath(const char* commonPath, const char* developmentPath, const char* compiledPath);

		static Resource* createResource(int resourceTypeID);
		
		static bool& alwaysRecompile();
		static bool& copySources();
		static bool& removeLocalSourcesAfterCompile();
		static bool& compileOnly();
		static bool& watchChanges();

#ifdef NET_RES_DRIVER
		static void startHost(uint16_t port);
		static void startServer(uint16_t port);
		static void connectToServer(const postal::NetworkAddress& serverAddress);
#endif
		
		str::string256 commonPath;
		str::string256 developmentPath;
		str::string256 compiledPath;
		
	private:
		virtual bool reloadIfUpdate();
	};
}
