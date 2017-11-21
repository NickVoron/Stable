#pragma once
#include "config.h"
#include "memoryManager.h"
#include "resourceDriver.h"
#include "templates.h"
#include "managers.h"
#include "inheritanceTable.h"
#include "finalizeQuery.h"


class Entity;
class EntitiesList;
template<class... ComponentType> struct ComponentLink;
template<class... ComponentType> struct ComponentLinkList;

class LinkerInterface
{
friend class ComponentsFactory;
public:
	virtual std::size_t classIndex() const = 0;

protected:
	template<class ComponentType> void link(int index, ComponentType*& result)								{ query().add(index, &result); }
	template<class ComponentType> void link(const std::string& alias, ComponentType*& result)				{ query().add(alias, &result); }
	template<class ComponentType> void link(int index, const std::string& alias, ComponentType*& result)	{ query().add(index, alias, &result); }
	template<class ComponentType> void link(ComponentType*& result)											{ link(0, result); } 
	template<class ComponentType> void link(Entity*& parent)												{ query().add<ComponentType>(&parent); }
								  void link(Entity*& parent)												{ query().add(&parent); }
	template<class ComponentType> void link(const Entity*& parent)											{ link<ComponentType>(const_cast<Entity*&>(parent)); }
								  void link(const Entity*& parent)											{ link(const_cast<Entity*&>(parent)); }

	template<template <class... Components> class ComponentEnvelope, class... Components> 
	void link(ComponentEnvelope<Components...>& envelope) { link(envelope.spawner); }

	template<template <class... Components> class ComponentEnvelope, class... Components>
	void link(int index, ComponentEnvelope<Components...>& envelope) { link(index, envelope.spawner); }

	template<template <class... Components> class ComponentEnvelope, class... Components> 
	void link(const std::string& alias, ComponentEnvelope<Components...>& envelope) { link(alias, envelope.spawner); }

	template <size_t n, typename... T> 
	typename std::enable_if<(n >= sizeof...(T))>::type
	link_tuple(std::tuple<T...>&) {}

	template <size_t n, typename... T> 
	typename std::enable_if<(n < sizeof...(T))>::type
	link_tuple(std::tuple<T...>& tup)
	{
		link(std::get<n>(tup));
		link_tuple<n + 1>(tup);
	}

	template <typename... T>
	void link_tuple(std::tuple<T...>& tup)
	{
		LinkerInterface::template link_tuple<0, T...>(tup);
	}

private:
	FinalizeQuery& query();
};


typedef void(*MappingFunction)(ComponentBase& source, ComponentBase& target);

struct SerializerInterface : public LinkerInterface
{
public:
	template<class... T> auto serialize(T&... value)
	{ 
		auto sz = { (serialize_impl(value), 0)... };
        return sz.size();
	}

private:
	template<class... T> 
	void serialize_impl(T&... value);


	template<class... ComponentType>	
	void serialize_impl(ComponentLink<ComponentType...>& lnk);

	template<class... ComponentType>	
	void serialize_impl(ComponentLinkList<ComponentType...>& lnk);

};

class ComponentBase : public SerializerInterface, public ResourceDriver
{
	friend class EntityData;
	friend class Entity;
public:
	virtual ~ComponentBase(){}

private:
	virtual void finalize(){}
	virtual void activate(bool active) {}
};

template<class ComponentChild> 
class ComponentBasePtrT : public ComponentBase 
{
public:
	class ptr
	{
	public:
		ptr(ComponentChild* _p = 0) : p(_p) {}

		ComponentChild& operator*() const { return *get(); }
		ComponentChild* operator->() const { return get(); }
		ComponentChild* get() const { return p; }

	private:
		ComponentChild* p;
	};

protected:
	typedef ComponentChild InheritanceParent;
};

template<class ComponentChild> struct ComponentBaseT : public ComponentBasePtrT<ComponentChild>, public Base::IntrusiveList<ComponentChild, ComponentChild>::BindedNode {};
template<class ComponentChild> struct StrictComponentBaseT : public ComponentBasePtrT<ComponentChild> {};

template<class ComponentChild>
class StrictComponentInterface : public StrictComponentBaseT<ComponentChild>
{
public:
	typedef StrictComponentInterface ComponentInterfaceBase;
	typedef void Parameters;
};

template<class ComponentChild, class ParametersHolder>
class ComponentInterface : public ComponentBaseT<ComponentChild>
{
public:
	typedef ParametersHolder Parameters;
	typedef ComponentInterface ComponentInterfaceBase;
													   
	template<class ParameterType>
	static auto& parameter() { return std::get<ParameterType>(::parameters<ComponentChild>()); }

	template<std::size_t index>
	static auto& parameter() { return std::get<index>(::parameters<ComponentChild>()); }

	template<class... P>
	static auto& parameters(P&... param)
	{ 
		auto& params = ::parameters<ComponentChild>();
		using src_params_tuple = std::tuple<typename std::decay<P>::type...>;
		using dst_params_tuple = typename std::decay<decltype(params)>::type;
		static_assert(std::is_same<src_params_tuple, dst_params_tuple>::value, "parameters list must be equal with 'Component::update()' function");
		params = std::tie(param...);
		return params;
	}


private:
	virtual void activate(bool value) override
	{ 
		if (this->parent_list)
		{
			if (value && !this->isInList())
			{
				ComponentActivator<ComponentChild>::call(static_cast<ComponentChild&>(*this));
				this->include();
			}
			else if (!value && this->isInList())
			{
				ComponentDeactivator<ComponentChild>::call(static_cast<ComponentChild&>(*this));
				this->exclude();																 				
			}
		}		
	}
};


template<class T, bool pointer, class... TT>
struct getref_impl;

template<class T, class... TT>
struct getref_impl<T, false, TT...>
{
	static auto& get(std::tuple<TT...>& t) { return std::get<T>(t); }
	static auto& get(const std::tuple<TT...>& t) { return std::get<T>(t); }
};

template<class T, class... TT>
struct getref_impl<T, true, TT...>
{
	static auto& get(std::tuple<TT...>& t)
	{
		auto& ptr = std::get<T>(t);
		CM_KERNEL_ENFORCE(ptr);
		return *ptr;
	}

	static auto& get(const std::tuple<TT...>& t)
	{
		auto& ptr = std::get<T>(t);
		CM_KERNEL_ENFORCE(ptr);
		return *ptr;
	}
};


template<class T, class... TT>
decltype(auto) getref(std::tuple<TT...>& t)
{
	return getref_impl<T, std::is_pointer<T>::value, TT...>::get(t);
}

template<class T, class... TT>
decltype(auto) getref(const std::tuple<TT...>& t)
{
	return getref_impl<T, std::is_pointer<T>::value, TT...>::get(t);
}

template<class T, class... TT>
decltype(auto) getptr(std::tuple<TT...>& t)
{
	return &getref_impl<T, std::is_pointer<T>::value, TT...>::get(t);
}

template<class T, class... TT>
decltype(auto) getptr(const std::tuple<TT...>& t)
{
	return &getref_impl<T, std::is_pointer<T>::value, TT...>::get(t);
}


template<class LinkableElement>
struct SelectTupleElementType
{
	typedef LinkableElement* type;
};

template<class... ComponentsList> struct ConfigurableSpawner;
template<class... ComponentsList> struct AccumulatingSpawner;

template<class... ComponentType>
struct SelectTupleElementType<ConfigurableSpawner<ComponentType...>>
{
	typedef ConfigurableSpawner<ComponentType...> type;
};

template<class... ComponentType>
struct SelectTupleElementType<AccumulatingSpawner<ComponentType...>>
{
	typedef AccumulatingSpawner<ComponentType...> type;
};

struct empty_type {};

template<class ClientAutoLink, class ComponentType>
struct InterfaceSelector
{
	typedef empty_type type;
};

template<class ClientAutoLink>
struct InterfaceSelector<ClientAutoLink, Entity>
{
	decltype(auto) entity()
	{
		return getref<Entity*>(static_cast<ClientAutoLink*>(this)->components_tuple);
	}

	decltype(auto) entities()
	{
		return entity().getParent();
	}

	/*
	template<class ComponentType>
	void componentLink(ComponentLink<ComponentType>& target)
	{
		target = ComponentLink<ComponentType>(&entity(), static_cast<ComponentType*>(this));
	}

	template<class ComponentType>
	void append(ComponentLinkList<ComponentType>& target)
	{
		ComponentLink<ComponentType> lnk;
		componentLink(lnk);
		target.push_back(lnk);
	}*/
};

template<class ClientAutoLink, class... ComponentType>
struct InterfaceSelector<ClientAutoLink, ConfigurableSpawner<ComponentType...>>
{
	decltype(auto) spawner() {	return static_cast<ClientAutoLink*>(this)->template component<ConfigurableSpawner<ComponentType...>>(); }

	template<class Configurator>
	decltype(auto) spawn(Configurator&& configurator)
	{
		return spawner().spawn(std::forward<Configurator>(configurator));
	}

	decltype(auto) spawn()
	{
		return spawn([](ComponentType&...){});
	}
};

template<class ClientAutoLink, class... ComponentType>
struct InterfaceSelector<ClientAutoLink, AccumulatingSpawner<ComponentType...>>
{
	decltype(auto) spawner() { return static_cast<ClientAutoLink*>(this)->template component<AccumulatingSpawner<ComponentType...>>(); }

	template<class Configurator>
	decltype(auto) spawn(Configurator&& configurator)
	{
		return spawner().spawn(std::forward<Configurator>(configurator));
	}

	decltype(auto) spawn()
	{
		return spawn([](ComponentType&...) {});
	}

	template<class Predicate, class Configurator>
	decltype(auto) spawn(Predicate&& predicate, Configurator&& configurator)
	{
		return spawner().spawn(std::forward<Predicate>(predicate), std::forward<Configurator>(configurator));
	}
};


template<class ClientAutoLink, class ComponentType, class... Components>
struct ComponentInterfaceComposerImpl : public InterfaceSelector<ClientAutoLink, ComponentType>, public ComponentInterfaceComposerImpl<ClientAutoLink, Components...>
{  	
};

template<class ClientAutoLink, class ComponentType>
struct ComponentInterfaceComposerImpl<ClientAutoLink, ComponentType> : public InterfaceSelector<ClientAutoLink, ComponentType>
{	  
};

template<class ClientAutoLink, class... Components>
struct ComponentInterfaceComposer : public ComponentInterfaceComposerImpl<ClientAutoLink, Components...>
{									
};

template<class ComponentChild>								class DataComponent			: public StrictComponentInterface<ComponentChild> { };
template<class ComponentChild>								class SimpleComponent		: public ComponentInterface<ComponentChild, SimpleComponentInputParameters<ComponentChild>> { };
template<class ComponentChild>								class UpdatableComponent	: public ComponentInterface<ComponentChild, UpdatableComponentInputParameters<ComponentChild>>{ };
template<class ComponentChild, class Parameters>			class CustomComponent		: public ComponentInterface<ComponentChild, SimpleComponentInputParameters<ComponentChild, Parameters>>{ };

template<class... Link>
class AutoLinkData
{
public:
	template<class ComponentT>
	ComponentT& component() const
	{
		return getref<typename SelectTupleElementType<ComponentT>::type>(components_tuple);
	}

	template<class ComponentT>
	ComponentT& component()
	{
		return getref<typename SelectTupleElementType<ComponentT>::type>(components_tuple);
	}

	std::tuple<typename SelectTupleElementType<Link>::type...> components_tuple;
};

template<class... Link>
class AutoLink : public AutoLinkData<Link...>, public ComponentInterfaceComposer<AutoLink<Link...>, Link...>
{
};


template<class ComponentChild, class... Link>	
class UpdatableComponentAutoLink : public AutoLink<Link...>, public UpdatableComponent<ComponentChild>
{
public:
	void linker()
	{
		this->link_tuple(this->components_tuple);
	}
};

template<class ComponentChild>
class UpdatableComponentAutoLink<ComponentChild> : public UpdatableComponent<ComponentChild>
{
};

template<class ComponentChild, class... Link>
class SimpleComponentAutoLink : public AutoLink<Link...>, public SimpleComponent<ComponentChild>
{
public:
	void linker()
	{
		this->link_tuple(this->components_tuple);
	}
};

template<class ComponentChild>
class SimpleComponentAutoLink<ComponentChild> : public SimpleComponent<ComponentChild>
{
};


namespace properties_inheritance
{
	template<class ComponentType>
	void inherite(ComponentType& parent, ComponentType& child) 
	{
	}
}


//
//
//
#define CM_DEFINE_BASE_CLSID(BASE_CLASS)	static const char* ClassName() { return STPP_STRINGIZE(BASE_CLASS); }	
											
#define CM_IMPLEMENT_SYSTEM_COMPONENT_INTERFACE(C) \
	friend class ComponentsFactory; \
	static std::atomic<std::size_t>& ClassIndex() { static std::atomic<std::size_t> index(0); return index; } \
	virtual std::size_t classIndex() const override { return ClassIndex().load(); } \
	CM_DEFINE_BASE_CLSID(C)
	
	
