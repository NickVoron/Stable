// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "entity.h"
#include "classesLib.h"
#include "links.h"

struct EntityArena : std::list<std::unique_ptr<EntitiesList>>
{
	EntitiesList& create();
	void execute();
};

class EntitiesList : public Base::IntrusiveList<Entity>
{
	friend class Class;
	friend class SystemSpawner;
public:
	struct EntityLinkLoadInfo
	{
		LinkSaveData data;
		std::vector<ComponentBase**> links;
	};

	struct LoadLinks : public std::vector<EntityLinkLoadInfo>
	{
		template<class... ComponentType>
		auto add(const LinkSaveData& data, ComponentType**... link)
		{
			EntityLinkLoadInfo info;
			info.data = data;
			auto sz = { (info.links.push_back((ComponentBase**) link), (ComponentBase**) link)... };
			push_back(info);
            return sz.size();
		}
	};

	EntitiesList();
	~EntitiesList();
	void activate(bool active);
	Entity* create(const std::string& className);
	Entity* create(std::size_t classIndex);

	Entity* at(std::size_t index) const;

	void clear();
	void save(stream::ostream& os) const;
	void load(stream::istream& is);

	void execute();

	void clearObjects();
	void saveObjects(stream::ostream& os) const;

	template<class Function>
	void loadObjects(stream::istream& is, bool activate, const std::vector<ComponentExternalLink>& externalLinks, Function&& func);

	void loadObjects(stream::istream& is, bool activate, const std::vector<ComponentExternalLink>& externalLinks);

	bool findComponent(ComponentBase* component, std::optional<std::size_t>& objectIndex, std::size_t& componentIndex) const;

	void debugOutput() const;
	std::string debugstr() const;

	ClassesLib classes;
	ExecutionList executionList;
	EntityPool pool;
	LoadLinks loadLinks;

private:
	void solveLoadList(const LoadList& loadList, const std::vector<ComponentExternalLink>& externalLinks);
	
	Entity& add();
};

template<class Function>
void EntitiesList::loadObjects(stream::istream& is, bool activate, const std::vector<ComponentExternalLink>& externalLinks, Function&& func)
{
	std::size_t count;	
	is >> count;
	if (count > 0)
	{
		LoadList loadList;
		for (std::size_t i = 0; i < count; ++i)
		{
			std::size_t classIndex;
			is >> classIndex;
			auto& cls = classes.classes[classIndex];
			Entity& entity = cls->createInstance(add());
			entity.load(is);
			entity.activate(activate);
			loadList.push_back(&entity);
		}

		if (!loadLinks.empty())
		{
			solveLoadList(loadList, externalLinks);
			loadLinks.clear();
		}

		for (auto& entity : loadList)
		{
			entity->finalize();
		}

		for (auto& entity : loadList)
		{
			func(*entity);
		}
	}
}




template<class... ComponentType>
void ComponentLink<ComponentType...>::save(stream::ostream& os) const
{
	ENFORCE(parent);
	LinkSaveData data;
	stl::for_each(components, [&entities = parent->getParent(), &data](auto& p)
	{
		std::size_t componentIndex = 0;
		if (entities.findComponent(p, data.objectIndex, componentIndex))
		{
			data.componentIndices.push_back(componentIndex);
		}
	});
	os << data.objectIndex << data.componentIndices;
	
}

template<class... ComponentType>
void ComponentLink<ComponentType...>::load(stream::istream& is)
{
	ENFORCE(parent);
	LinkSaveData data;
	is >> data.objectIndex >> data.componentIndices;
	stl::apply(
	[&entities = parent->getParent(), &data](auto&... component)
	{
		entities.loadLinks.add(data, &component...);
	}, components);
}

template<class... ComponentType>
void LinkDesc::flush(ComponentLink<ComponentType...>& link, const EntitiesList& entities) const
{
	if (address.valid())
	{
		if(address.componentIndices.size() == sizeof...(ComponentType))
		{
			auto sampler = [entity = entities.at(*address.objectIndex), &indices = address.componentIndices](size_t index)
			{
				return &entity->getComponent(indices[index]);
			};

			stl::init_tuple_from_sampler(sampler, link.components);
		}
		else
		{
			ERROR_EXPRESSION(address.componentIndices.size(), sizeof...(ComponentType));
		}
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