#pragma once

#include <vector>
#include <optional>

#include "reflection/library.include.h"

#include "common/apply.h"
#include "entity.h"

class ComponentBase;

struct SaveList : public std::vector<const Entity*>{};
struct LoadList : public std::vector<Entity*>{};

struct LinkSaveData 
{
	std::optional<std::size_t> objectIndex;
	std::vector<std::size_t> componentIndices; 

	bool valid() const { return objectIndex && !componentIndices.empty(); }

	friend stream::ostream& operator<<(stream::ostream& os, const LinkSaveData& l) { return os << l.objectIndex << l.componentIndices; }
	friend stream::istream& operator>>(stream::istream& is, LinkSaveData& l) { return is >> l.objectIndex >> l.componentIndices; }
};

struct ValueAddress
{
	mirror::runtime::Type* type;
	std::optional<std::size_t> componentIndex;
	std::size_t componentShift;
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
			stl::for_each(std::tie(std::get<ComponentType*>(components)...), [&valid](auto* ptr)
			{
				valid &= (ptr != nullptr);
			});

			if (valid)
			{
				//LOG_EXPRESSION(parent->getClass().name());
				auto input = std::tie(dynamic_cast<ComponentType&>(*std::get<ComponentType*>(components))...);
				//debug();
				return stl::apply(operation, input);
			}
		}

		return decltype(stl::apply(operation, std::tie(*std::get<ComponentType*>(components)...)))();
	}

 	void debug() const
 	{
 		if (parent)
 		{
			stl::for_each(std::tie(std::get<ComponentType*>(components)...), [](auto* component)
			{
				if(component)
				{
					LOG_MSG(ComponentsFactory::className(*component));
				}
				else
				{
					LOG_MSG("empty");
				}				
			});
 		}
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
		std::vector<std::reference_wrapper<ComponentLink<ComponentType...>>> links;

		std::size_t result = 0;
		this->erase(std::remove_if(this->begin(), this->end(),
		[&result, &links](auto& link)
		{
			bool alive = link.parent && link.parent->isInList();
			if (alive)
			{
				++result;
				links.emplace_back(link);			
			}

			return !alive;
		}), this->end());

		for(auto& link : links)
		{
			link(operation);
		}
		
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

