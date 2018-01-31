// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "store.h"
#include "settingsRegistry/library.include.h"
#include "hash/library.include.h"

#include "resource.h"
#include "resourceSet.h"

#include "common/waitable_queue.h"

namespace Resources 
{

	namespace { std::recursive_mutex sys_mtx; }	
	SystemLockGuard::SystemLockGuard() : std::lock_guard<std::recursive_mutex>(sys_mtx)	{}

	ChangeWatcher::FileHandleWrapper::FileHandleWrapper(const char* fileName_) : fileName(fileName_)
	{
#ifdef WIN32
		handle = FindFirstChangeNotificationA(fileName.c_str(), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
#endif
	}

	ChangeWatcher::FileHandleWrapper::~FileHandleWrapper()
	{
#ifdef WIN32
		FindCloseChangeNotification(handle);
#endif
	}

	void ChangeWatcher::add(Resource* res)
	{
		str::string256 folder;
		res->getFullSourceFolder(folder);
		add(folder.c_str());
	}

	void ChangeWatcher::add(const char* path)
	{
		if (std::find_if(modifyHandles.begin(), modifyHandles.end(), [path](auto& handle) { return handle.fileName == path; }) == modifyHandles.end())
		{
			modifyHandles.emplace_back(path);
		}		
	}

	void ChangeWatcher::clear()
	{
		modifyHandles.clear();
	}	
		
	bool ChangeWatcher::process()
	{
#ifdef WIN32
		bool result = false;
		bool updated = false;

		for(const auto& modifyHandle : modifyHandles)
		{
			if(WaitForSingleObject(modifyHandle.handle, 0) == WAIT_OBJECT_0)
			{
				if(!updated)
				{
					updated = true;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					result |= reloadIfUpdate();
				}

				FindNextChangeNotification(modifyHandle.handle);
			}
		}	

		return result || updated;
#else
		return false;
#endif
	}

	void ChangeWatcher::saveTimeStampsRegistry()
	{
		SettingsRegistry::add("resources", timeStampsRegistry);
		SettingsRegistry::save();
	}

	void ChangeWatcher::loadTimeStampsRegistry()
	{
		try
		{
			timeStampsRegistry.clear();
			SettingsRegistry::get("resources", timeStampsRegistry);
		}
		catch (std::exception& e)
		{
			LOG_ERROR("resources time stamps is invalid, resources will be recompiled: " << e.what());
			timeStampsRegistry.clear();
		}
	}

	void ChangeWatcher::storeDependencies(const char* fullSourcePath, const Dependencies::StoredState& ss)
	{
		timeStampsRegistry[fullSourcePath] = ss;
	}

	void ChangeWatcher::restoreDependencies(const char* fullSourcePath, Dependencies::StoredState& ss)
	{
		ss = timeStampsRegistry[fullSourcePath];
	}

	ChangeWatcher::ChangeWatcher()
	{

	}

	void ChangeWatcher::load()
	{
		if(Store::watchChanges())
		{
			loadTimeStampsRegistry();
		}		
	}

	void ChangeWatcher::save()
	{
		if(Store::watchChanges())
		{
			saveTimeStampsRegistry();
		}		
	}

	void ChangeWatcher::release()
	{
		timeStampsRegistry.clear();
		clear();
	}

	
	
	
	void Store::setPath(const char* commonPath, const char* developmentPath, const char* compiledPath)
	{
    	Store& store = get();

		store.commonPath = commonPath;
		store.developmentPath = developmentPath;
		store.compiledPath = compiledPath;

		store.add(developmentPath);
	}

	void Store::Register(Resource* res)
	{
		if(watchChanges())
		{
			get().add(res);
		}		
	}

	bool Store::reloadIfUpdate()
	{
		bool res = false;
		for (auto& s : get())
		{
			res |= s.reloadIfUpdate();
		}

		return res;
	}

	bool Store::isValidResource(Resource* res)
	{
		return true;
		for (auto& s : get())
		{
			if (s.isValidResource(res))
				return true;
		}

		return false;
	}

	bool Store::ReloadIfUpdate()
	{ 
		SystemLockGuard lock;

		if(watchChanges())
		{
			if (get().process())
			{
				get().save();
				return true;
			}
		}	

		return false;
 	}

	bool& Store::copySources() 						{ SystemLockGuard lock; static bool f = false; return f;	}
	bool& Store::alwaysRecompile()					{ SystemLockGuard lock; static bool f = false; return f;	}
	bool& Store::removeLocalSourcesAfterCompile()	{ SystemLockGuard lock; static bool f = false; return f;	}
	bool& Store::compileOnly()						{ SystemLockGuard lock; static bool f = false; return f;	}
	bool& Store::watchChanges()						{ SystemLockGuard lock; static bool f = true; return f;		}

	void Store::Clear() 
	{ 
		for (auto& s : get())
		{
			s.clear();
		}	
	}

	void Store::Destroy()
	{
		Clear();
		get().remove_all();
	}

#ifdef NET_RES_DRIVER
	void Store::startHost(uint16_t port)
	{
		get().net_host(port);
	}

 	void Store::startServer(uint16_t port)
 	{
 		get().net_server(port);
 	}
 
	void Store::connectToServer(const postal::NetworkAddress& serverAddress)
 	{
		get().net_client(serverAddress);
 	}
#endif

	void Store::dictionary(FactoryDict& dict)
	{
		dict.clear();

		int index = 0;
		for (auto& s : get())
		{
			dict[s.name()] = index++;
		}
	}

	Resource* Store::createResource(int resourceTypeID)
	{
		Store& store = get();

		int index = 0;
		for (auto& s : get())
		{
			if (index == resourceTypeID)
			{
				return s.createResource();
			}
			++index;
		}

		return nullptr;
	}

	stream::ostream& operator <<(stream::ostream& s, const Dependencies::FileInfo& fi)	{ return s << fi.absoluteFileName << fi.relativeFileName << fi.time; }
	stream::istream& operator >>(stream::istream& s, Dependencies::FileInfo& fi)		{ return s >> fi.absoluteFileName >> fi.relativeFileName >> fi.time; }

	stream::ostream& operator <<(stream::ostream& s, const Dependencies::StoredState& ss)	{ return s << ss.root << ss.dependencies; }
	stream::istream& operator >>(stream::istream& s, Dependencies::StoredState& ss) 		{ return s >> ss.root >> ss.dependencies; }

	
	
	
	template<class Type, int count>
	struct MTQueues
	{
		Type instances[count];

		mt::waitable_queue<Type*, mt::lifo> secondary;

		MTQueues()
		{
			for (int i = 0; i < count; ++i)
			{
				secondary.send(&instances[count - i - 1]);
			}
		}

		Type* lock(const char* resourceName)
		{
			Type* res;
			secondary.receive(res);
			res->open(resourceName);
			return res;
		}

		template<class StreamType>
		void unlock(StreamType* val)
		{
			((Type*)val)->close();
			secondary.send(((Type*) val));
		}
	};

	typedef stream::ofstream<> WriterStream;
	typedef stream::ifstream<> ReaderStream;

	auto& writers() 
	{ 
		SystemLockGuard lock;
		static MTQueues<WriterStream, 24> queues; 
		return queues;  
	}

	auto& readers() 
	{ 
		SystemLockGuard lock;
		static MTQueues<ReaderStream, 24> queues; 
		return queues; 
	}

	stream::ostream* IOStreamsManager::writer(const char* resourceName)
	{
		return writers().lock(resourceName);
	}

	stream::istream* IOStreamsManager::reader(const char* resourceName)
	{		
		return readers().lock(resourceName);
	}

	void IOStreamsManager::close(stream::ostream* s)
	{
		writers().unlock(s);
	}

	void IOStreamsManager::close(stream::istream* s)
	{
		readers().unlock(s);
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