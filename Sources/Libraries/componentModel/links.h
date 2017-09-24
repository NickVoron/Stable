// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <vector>

#include "memoryManager.h"
#include "common/apply.h"
#include "entity.h"

class ComponentBase;

struct SaveList : public std::vector<const Entity*>{};
struct LoadList : public std::vector<Entity*>{};

struct LinkSaveData 
{
	std::size_t objectIndex = -1; 
	std::vector<std::size_t> componentIndices; 

	bool valid() const { return !componentIndices.empty(); }
};


template<class... ComponentType>
struct ComponentLink
{
friend struct SerializerInterface;
template<class... CT> friend struct ComponentLinkList;
public:
	ComponentLink() {}
	ComponentLink(const Entity* parent_, ComponentType*... components_) : components(components_...), parent(parent_) {}

	template<class Operation>
	decltype(auto) operator()(Operation&& operation)
	{
		if (parent && parent->isInList())
		{
			bool valid = true;
			stl::for_each(std::tie(std::get<ComponentType*>(components)...), [&valid](auto* ptr) { valid &= (ptr != nullptr); });
			if (valid)
			{
				
				return stl::apply(operation, std::tie(*std::get<ComponentType*>(components)...));
			}
		}

		return decltype(stl::apply(operation, std::tie(*std::get<ComponentType*>(components)...)))();
	}















	template<class Component>
	Component* component()
	{
		return (parent && parent->isInList()) ? std::get<Component*>(components) : nullptr;
	}
	
	friend stream::ostream& operator<<(stream::ostream& os, const ComponentLink<ComponentType...>& l) { l.save(os); return os; }
	friend stream::istream& operator>>(stream::istream& is, ComponentLink<ComponentType...>& l) { l.load(is); return is; }

	EntitiesList& parentEntitiesList() { ENFORCE(parent); return parent->getParent(); }

protected:
	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	std::tuple<ComponentType*...> components;
	const Entity* parent = nullptr;
};

template<class... ComponentType>
struct ComponentLinkList : public std::vector< ComponentLink<ComponentType...> >
{
public:
	template<class Operation>
	std::size_t operator()(Operation&& operation)
	{
		std::size_t result = 0;
		
		this->erase(std::remove_if(this->begin(), this->end(),
		[&result, &operation](auto& link)
		{
			bool alive = link.parent && link.parent->isInList();
			if (alive)
			{
				++result;
				link(operation);					
			}

			return !alive;
		}), this->end());
		
		return result;
	}

	void clean()
	{
		this->erase(std::remove_if(this->begin(), this->end(), [](auto& link)
		{	
			return !(link.parent && link.parent->isInList());
		}), this->end());
	}

	void save(stream::ostream& os) const
	{
		os << (std::size_t)this->size();
		for (std::size_t i = 0; i < this->size(); ++i)
		{
			this->at(i).save(os);
		}
	}

	void load(stream::istream& is)
	{
		std::size_t sz; 
		is >> sz;
		this->resize(sz);
		for (std::size_t i = 0; i < sz; ++i)
		{
			this->at(i).parent = parent;
			this->at(i).load(is);
		}
	}

	std::size_t add(Entity& entity, ComponentType&... component)
	{
		this->emplace_back(&entity, &component...);
		return this->size();
	}

	EntitiesList& parentEntitiesList() { ENFORCE(parent); return parent->getParent(); }

	const Entity* parent = nullptr;

	friend stream::ostream& operator<<(stream::ostream& os, const ComponentLinkList<ComponentType...>& l) { l.save(os); return os; }
	friend stream::istream& operator>>(stream::istream& is, ComponentLinkList<ComponentType...>& l) { l.load(is); return is; }
};

struct LinkDesc 
{	
	LinkSaveData address;

	template<class... ComponentType>
	void flush(ComponentLink<ComponentType...>& link) const
	{
		flush(link, link.parentEntitiesList());
	}

	template<class... ComponentType>
	void flush(ComponentLink<ComponentType...>& link, const EntitiesList& entities) const;

	friend std::ostream& operator<<(std::ostream& os, const LinkDesc& ldl);
};

struct LinksDescList : public std::vector<LinkDesc>
{
	template<class ComponentType>
	void flush(ComponentLinkList<ComponentType>& links) const
	{
		if (!empty())
		{
			links.resize(size());
			for (std::size_t i = 0; i < size(); ++i)
			{
				at(i).flush(links[i], links.parentEntitiesList());
			}
		}			
	}

	friend std::ostream& operator<<(std::ostream& os, const LinksDescList& ldl);
};

template<class... ComponentType>
ComponentLink<ComponentType...>& ToComponentLink(const LinkDesc& link, ComponentLink<ComponentType...>& result)
{
	link.flush(result);
	return result;
}

template<class... ComponentType>
ComponentLinkList<ComponentType...>& ToComponentLinkList(const LinksDescList& link, ComponentLinkList<ComponentType...>& result)
{
	link.flush(result);
	return result;
}

template<class... ComponentType>
LinkDesc& FromComponentLink(const ComponentLink<ComponentType...>& link, LinkDesc& result)
{
	return result;
}

template<class... ComponentType>
LinksDescList& FromComponentLinkList(const ComponentLinkList<ComponentType...>& link, LinksDescList& result)
{
	return result;
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