// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <boost/filesystem.hpp>
#include <boost/type_traits.hpp>

#include "stuff/library.include.h"
#include "containers/library.include.h"
#include "strings/library.include.h"
#include "common/exception_safety.h"

#include "errors.h"

#include "resourceLoader.h"
#include "dependencies.h"
#include "store.h"

namespace Resources 
{
	template<class ResourceTypeT, class StringType>
	auto load(ResourceTypeT*& ptr, const StringType& fileName, bool wait = false);
	
	template<class ResourceTypeT, class StringType, class T>
	auto load(ResourceTypeT*& ptr, const StringType& fileName, bool wait, const T& data);

	template<class T> class ResourceSet;

	template<class ResourceType>
	class ResourcesPack : public std::vector < ResourceType* >
	{
	public:
		ResourcesPack() : currentIndex(0) {}

		template<class T>
		void load(const std::string& directory, const std::string& ext, const T& userData)
		{
			for (const auto& file : resourceNames(directory, ext))
			{
				ResourceType* res = 0;
				Resources::load(res, directory + "/" + Base::FileUtils::GetFileNameFrom(file.string()), false, userData);
				ResourcesPack::push_back(res);
			}
		}

		void load(const std::string& directory, const std::string& ext)
		{
			for (const auto& file : resourceNames(directory, ext))
			{
				ResourceType* res = 0;
				Resources::load(res, directory + "/" + Base::FileUtils::GetFileNameFrom(file.string()), false);
				ResourcesPack::push_back(res);
			}
		}
		
		static auto resourceNames(const std::string& directory, const std::string& ext)
		{
			return Base::FileUtils::GetFileNamesByMask(std::string(Store::get().developmentPath.c_str()) + "/" + ResourceType::basePath() + directory + "/", ext, false);
		}

		ResourceType* current() { return ResourcesPack::empty() ? 0 : operator[](currentIdx()); }

		void index(int idx)
		{
			currentIndex = idx;
			currentIdx();
		}

	private:
		int currentIdx()
		{
			if (currentIndex < 0) currentIndex = 0;
			if (currentIndex >= (int) ResourcesPack::size()) currentIndex = ResourcesPack::size() - 1;

			return currentIndex;
		}

		int currentIndex;
	};

	struct ResourceTypeInfo
	{
		virtual int typeId() const = 0;
		virtual const char* typeName() const = 0;
	};

	class ResourcePath
	{
	friend class Store;
	friend class ChangeWatcher;
	template<class T> friend class ResourceSet;
	public:
		struct ID
		{
		public:
			void data(const char* name, const void* userData, std::size_t userDataSize);

			bool equal(const ID& id) const;
			str::string32 string() const;

		private:
			unsigned long crc0 = 0;
			unsigned long crc1 = 0;
		};

		str::string64 localFileName;
		str::string256 sourceFileName;
		str::string256 compiledFileName;

	protected:
		virtual const char* storedPath() const = 0;
		void AssignFileName(const char* newName);

	private:		
		ID id;

		void getFullCompiledFileName(str::string256& path) const;
		void getFullSourceFileName(str::string256& path) const;
		void getFullSourceFolder(str::string256& path) const;
		void getCommonSourceFileName(str::string256& path) const;
		void getDevelopmentSourceFileName(str::string256& path) const;
		void getRelativeSourceFileName(str::string256& path) const;
		void getLocalSourceFileName(str::string256& path) const;
	};

	class Resource : public ResourcePath, public ResourceTypeInfo
	{
	friend class Store;
	template<class T> friend class BaseResourceLoader;
	template<class T> friend class ResourceSet;
	public:

		inline Resource() {}
		inline Resource(const Resource&) = delete;
		inline const Resource& operator=(const Resource&) = delete;

		virtual ~Resource();
				
		void RegisterAndLoad(const char* path, const char* fileName, const void* userData, std::size_t userDataSize);
		void Reload();
		bool ReloadIfUpdate();
		void UpdateCompiled();
		void CompileTo(const char* fileName, const void* userData, std::size_t userDataSize, stream::ostream& os);

		virtual void Clear() = 0;
		virtual void LoadCompiled(stream::istream& is) = 0;

		virtual std::vector<std::string> help() const {	return std::vector<std::string>(); }

	protected:
		virtual void Compile(stream::ostream& os) = 0;
		virtual void LoadSource(const char* sourceFileName, const void* userData, std::size_t userDataSize) = 0;
		virtual void RemoveFromSet() = 0;		
		
		static void ConstructID(ID& id, const char* name, const void* userData, std::size_t userDataSize);

		Dependencies dependencies;

	protected:
		virtual bool IsCompiledFileIsActual();
		virtual void OnLoadCompiledComplete() {}

		mem::mem_desc userDataBlock;

	private:
		void Load(const void* userData, uint32_t userDataSize);
		void saveActualTimeStamp();
		void addDependenciesFromTimeStamp();
	};

	template<class ClientClass, class NativeResource>
	class ResourceT : public Resource, public Base::IntrusiveList<ClientClass>::Node
	{
	friend class Store;
	friend class ResourceSet<ClientClass>;
	public:
		struct Handle
		{
		public:
			inline Handle(ClientClass* res = nullptr) :client(res) { }

			template<class Operation>
			inline Handle& operator()(Operation&& operation)
			{
				if (client)
				{
					client->resource(std::forward<Operation>(operation));
				}

				return *this;
			}

			auto load(const char* resourcePath)
			{
				return Resources::load(client, resourcePath);
			}

			inline void destroy()
			{
				delete client;
				client = nullptr;
			}

		private:
			ClientClass* client = nullptr;
		};

	public:
		typedef ResourceT BaseT;
		typedef NativeResource ClientResource;
		typedef ResourcesPack<ClientClass> Pack;

		virtual ~ResourceT()
		{
			SystemLockGuard lock;
			ResourceT::remove();
		}													 

		static bool& alwaysRecompile() 
		{ 
			SystemLockGuard lock;
			static bool f = false; 
			return f; 
		}

		static ResourceSet<ClientClass>& resourceSet() 
		{
			SystemLockGuard lock;
			static ResourceSet<ClientClass> s; 
			return s; 
		}

		template<class StringType>			static inline ClientClass* add(const StringType& fileName)						{ return &ResourceT::resourceSet().add(ClientClass::basePath(), str::c_str(fileName)); }
		template<class StringType, class T>	static inline ClientClass* add(const StringType& fileName, const T& userData)	{ return &ResourceT::resourceSet().add(ClientClass::basePath(), str::c_str(fileName), userData); }

		virtual const char* storedPath() const { return ClientClass::basePath(); }

		virtual int typeId() const { return ClientClass::TypeID(); }
		virtual const char* typeName() const { return ClientClass::TypeName(); }

		template<class Function>
		inline bool resource(Function&& function) const
		{
			SystemLockGuard lock;
			if (resourcePtr)
			{
				function(*resourcePtr);
			}

			return resourcePtr != nullptr;
		}

	protected:
		virtual bool IsCompiledFileIsActual() 
		{
			return Resource::IsCompiledFileIsActual() && !alwaysRecompile(); 
		}

		virtual void RemoveFromSet() { ResourceT::remove(); }

		inline void deactivate()
		{
			SystemLockGuard lock;
			resourcePtr = nullptr;
		}

		inline void activate()
		{
			SystemLockGuard lock;
			resourcePtr = &nativeResource;
		}

		inline bool active() const 
		{ 
			SystemLockGuard lock;
			return resourcePtr != nullptr;
		}

		NativeResource nativeResource;

	private:
		static int& TypeID()
		{
			static int id = -1;
			return id;
		}														 

		NativeResource* resourcePtr = nullptr;
	};

	void commit();

	template<class ResourceTypeT, class StringType>
	auto load(ResourceTypeT*& ptr, const StringType& fileName, bool wait)
	{ 
		ptr = ResourceTypeT::add(fileName);
		if (wait) commit(); 

		return ptr;
	}
	
	template<class ResourceTypeT, class StringType, class T>
	auto load(ResourceTypeT*& ptr, const StringType& fileName, bool wait, const T& data)
	{
		ptr = ResourceTypeT::add(fileName, data); 
		if (wait) commit();

		return ptr;
	}

	template<class StringType>
	void compile(int platformID, int resourceTypeID, const StringType& fileName, stream::ostream& os)
	{
		Resource* resource = Store::createResource(resourceTypeID);
		resource->CompileTo(fileName.c_str(), 0, 0, os);
		delete resource;
	}

	template<class ResourceTypeT> struct has_external_editor { static const bool value = false; };
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