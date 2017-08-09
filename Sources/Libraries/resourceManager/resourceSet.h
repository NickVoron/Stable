// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "stuff/library.include.h"
#include "containers/library.include.h"
#include "udtnet/library.include.h"

#include "store.h"
#include "resource.h"

namespace Resources 
{		  
	class Resource;
	class ResourceSetBase : public Base::IntrusiveList<ResourceSetBase>::Node
	{
	public:
		typedef std::vector<str::string256> ResourcesFilesList;

		ResourceSetBase() 
		{ 
			SystemLockGuard lock;
			Store::get().push_back(*this); 
		}

		virtual ~ResourceSetBase() 
		{ 
			SystemLockGuard lock;
			this->remove(); 
		}

		virtual void clear() = 0;
		virtual bool reloadIfUpdate() = 0;
		virtual void reloadAll() = 0;
		virtual const char* name() const = 0;
		virtual ResourcesFilesList resources() const = 0;
		virtual bool associatedEditor() const = 0;
		virtual Resource* createResource() = 0;
		virtual bool isValidResource(Resource* res) = 0;
	};

	template<class T>
	class ResourceSet : public ResourceSetBase, public Base::IntrusiveList<T>
	{
	public:
		ResourceSet() { ENFORCE(T::TypeID() < 0);  T::TypeID() = (int)Store::get().size() - 1; }

		virtual void clear() override;

		template<class UserData>
		T& add(const char* path, const char* name, const UserData& userData)							{ return add(path, name, &userData, sizeof(userData));	}
		T& add(const char* path, const char* name)														{ return add(path, name, 0, 0); }
		T& add(const char* path, const char* name, const char* userData)								{ return add(path, name, (const void*)userData, strlen(userData)); }
		T& add(const char* path, const char* name, const void* userData, std::size_t userDataSize);

		void reloadAll() override;
		bool reloadIfUpdate() override;
		virtual bool isValidResource(Resource* res) override;

		virtual const char* name() const override { return T::TypeName(); }
		virtual ResourcesFilesList resources() const override;
		virtual bool associatedEditor() const override { return has_external_editor<T>::value; }

		virtual Resource* createResource() override { return new T(); }

	private:

		template<class ID>
		T* find(const char* name, const void* userData, std::size_t userDataSize, ID& id);

		template<class ID>
		const T* find(const char* name, const void* userData, std::size_t userDataSize, ID& id) const;
	};

	
	
	
	template < class T >
	void ResourceSet<T>::clear()
	{
		SystemLockGuard lock;
		Base::IntrusiveList<T>::destroy_all();
	}

	template < class T >
	template < class ID >
	const T* ResourceSet<T>::find(const char* name, const void* userData, std::size_t userDataSize, ID& id) const
	{
		return ((ResourceSet<T>*)this)->find(name, userData, userDataSize, id);
	}

	template < class T >
	template < class ID >
	T* ResourceSet<T>::find(const char* name, const void* userData, std::size_t userDataSize, ID& id)
	{
		T::ConstructID(id, name, userData, userDataSize);

		SystemLockGuard lock;

		for (auto r = ResourceSet::begin(); r != ResourceSet::end(); ++r)
		{
			if( r->id.equal(id) )						
				return &*r;
		}

		return 0;
	}

	template <class T>	
	bool ResourceSet<T>::isValidResource(Resource* res)
	{
		for (auto r = ResourceSet::begin(); r != ResourceSet::end(); ++r)
		{
			if (r == res)
				return true;
		}

		return false;
	}

	template <class T>	void ResourceSet<T>::reloadAll()		{ for (auto& r : *this)	{ r.Reload(); } }
	template <class T>	bool ResourceSet<T>::reloadIfUpdate() { bool res = false; for (auto& r : *this) { res |= r.ReloadIfUpdate(); } return res; }

	template<class T>
	T& ResourceSet<T>::add(const char* path, const char* name, const void* userData, std::size_t userDataSize)
	{	
		SystemLockGuard lock;
		
		Resource::ID id;
		T* item = find(name, userData, userDataSize, id);

		if (!item)
		{
			item = (T*) createResource();
			item->id = id;
			this->push_back(*item);

#ifdef NET_RES_DRIVER
			auto& netapp = Store::get();
			if (!netapp.isClient())
			{
#endif
				item->RegisterAndLoad(path, name, userData, userDataSize);
#ifdef NET_RES_DRIVER
			}
			else if (netapp.isClient() && !netapp.isServer())
			{
				LOG_MSG("new resource request: " << name << " id: " << item);
				netapp.query(item, 0, name);
			}
#endif
		}

		return *item;
	}

	template<class T>
	ResourceSetBase::ResourcesFilesList ResourceSet<T>::resources() const
	{
		SystemLockGuard lock;

		ResourcesFilesList result;
		for (auto r = ResourceSet::begin(); r != ResourceSet::end(); ++r)
		{
			if (r->sourceFileName.empty())
			{
				result.push_back(r->sourceFileName);
			}
		}
		return result;
	}
}




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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