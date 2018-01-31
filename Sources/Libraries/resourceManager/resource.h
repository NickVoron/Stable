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
#include "common/type_traits.h"

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

	struct ResourceTypeInfo
	{
		virtual int typeId() const = 0;
		virtual const char* typeName() const = 0;
	};

	template<class Data>
	struct UserDataBaseT : public UserData
	{
		UserDataBaseT(const Data& data_) :data(data_) {}
		Data data;
	};

	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	template <class T0, class T1>
	inline void hash_combine(std::size_t& seed, const std::pair<T0, T1>& v)
	{
		hash_combine(seed, v.first);
		hash_combine(seed, v.second);
	}

	template <class T>
	inline void hash_combine_container(std::size_t& seed, const T& v)
	{
		for (auto& value : v)
		{
			hash_combine(seed, value);
		}
	}

	enum TYPE_TAG
	{
		COMMON_TYPE	   = 0,
		POD_TYPE	   = 1 << 0,
		CONTAINER_TYPE = 1 << 1,
		STRUCTURE_TYPE = 1 << 2,
	};

	template<class Data, int type = 
		  ((std::is_trivially_destructible<Data>::value && std::is_trivially_copyable<Data>::value) ? POD_TYPE : 0)
		| (stl::IsContainer<Data>::value ? CONTAINER_TYPE : 0)
		| (stl::IsStructure<Data>::value ? STRUCTURE_TYPE : 0)
	>
	struct UserDataT;

	template<class Data>
	struct UserDataT<Data, POD_TYPE> : public UserDataBaseT<Data>
	{
		UserDataT(const Data& data) : UserDataBaseT<Data>(data) {}

		virtual std::size_t hash() const
		{
			return crc::CRC32(&this->data, sizeof(this->data));
		}
	};

	template<class Data>
	struct UserDataT<Data, CONTAINER_TYPE> : public UserDataBaseT<Data>
	{
		UserDataT(const Data& data) : UserDataBaseT<Data>(data) {}

		virtual std::size_t hash() const
		{
			std::size_t seed = 0;
			hash_combine_container(seed, this->data);
			return seed;
		}
	};

	template<class Data>
	struct UserDataT<Data, STRUCTURE_TYPE> : public UserDataBaseT<Data>
	{
		UserDataT(const Data& data) : UserDataBaseT<Data>(data) {}

		virtual std::size_t hash() const
		{
			return std::hash<Data>()(this->data);
		}
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
			void data(const char* name, const UserData* userData);

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
				
		void RegisterAndLoad(const char* path, const char* fileName, std::unique_ptr<UserData> userData);
		void Reload();
		bool ReloadIfUpdate();
		void UpdateCompiled();
		void CompileTo(const char* fileName, const UserData* userData, stream::ostream& os);

		virtual void Clear() = 0;
		virtual void LoadCompiled(stream::istream& is) = 0;

		virtual std::vector<std::string> help() const {	return std::vector<std::string>(); }

	protected:
		virtual void Compile(stream::ostream& os) = 0;
		virtual void LoadSource(const char* sourceFileName, const UserData* userData) = 0;
		virtual void RemoveFromSet() = 0;		
		
		static void ConstructID(ID& id, const char* name, const UserData* userData);

		Dependencies dependencies;

	protected:
		virtual bool IsCompiledFileIsActual();
		virtual void OnLoadCompiledComplete() {}

		std::unique_ptr<UserData> userDataBlock;

	private:
		void Load(std::unique_ptr<UserData> userData);
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

			template<class Operation>
			inline Handle& ifnull(Operation&& operation)
			{
				if (!client)
				{
					operation();
				}

				return *this;
			}

			auto load(const char* resourcePath)
			{
				return Resources::load(client, resourcePath);
			}

			template<class UserData>
			auto load(const char* resourcePath, const UserData& data)
			{
				return Resources::load(client, resourcePath, false, data);
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
		resource->CompileTo(fileName.c_str(), nullptr, os);
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