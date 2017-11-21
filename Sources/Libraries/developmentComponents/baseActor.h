#pragma once

template<template <class> class ActorInterface, class... Links>
struct ActorBase
{
	template<class ComponentType, class... Params>
	static void developmentACTION(ComponentType& component, Params&&... params)
	{
		ActorInterface<ComponentType>::developmentACTION(component, params...);
	}

	template<class ComponentType>
	static void developmentDESC(std::string& result, ComponentType& component)
	{
		result += str::stringize(ActorInterface<ComponentType>::developmentDESC(component), "\n").str();
	}

	template<class ComponentType, class... Params> static void action(ComponentType* client, Params&&... params)
	{
		auto seq = { (developmentACTION(client->template component<Links>(), params...), 0)... };
	}

	template<class ComponentType> static std::string description(ComponentType* client)
	{
		std::string result;
		auto seq = { (developmentDESC(result, client->template component<Links>()), 0)... };
		return result;
	}
};