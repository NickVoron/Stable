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
	//void solveLoadList(const LoadList& loadList, const std::vector<ComponentExternalLink>& externalLinks);
	void solveLoadList(const LoadList& loadList);
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
			solveLoadList(loadList);
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

//
//
//
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
	//debug();
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
		const Entity* entity = entities.at(address.objectIndex.value());
		std::size_t idx = 0;
		auto nlink = ComponentLink<ComponentType...>(entity, (ComponentType*) &entity->getComponent(address.componentIndices[idx++])...);
		link = nlink;
	}
}
