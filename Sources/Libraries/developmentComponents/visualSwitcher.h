#pragma once

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"
#include "intersections/library.include.h"

#ifdef WIN32
#include "debugDrawing/library.include.h"
#endif // WIN32


template<class Component>
struct SwitcherInterface
{
	static void developmentSwitchON(Component& component) {}
	static void developmentSwitchOFF(Component& component) {}
	static bool developmentIsON(const Component& component) { return false; }
};

template<class... Links>
struct SwitcherBase
{
	template<class ComponentType>
	static void developmentTOGGLE(ComponentType& component)
	{
		if (SwitcherInterface<ComponentType>::developmentIsON(component))
		{
			SwitcherInterface<ComponentType>::developmentSwitchOFF(component);
		}
		else
		{
			SwitcherInterface<ComponentType>::developmentSwitchON(component);
		}
	}

	template<class ComponentType>
	static void developmentON(ComponentType& component)
	{
		if (!SwitcherInterface<ComponentType>::developmentIsON(component))
		{
			SwitcherInterface<ComponentType>::developmentSwitchON(component);
		}
	}

	template<class ComponentType>
	static void developmentOFF(ComponentType& component)
	{
		if (SwitcherInterface<ComponentType>::developmentIsON(component))
		{
			SwitcherInterface<ComponentType>::developmentSwitchOFF(component);
		}
	}

	template<class ComponentType>
	static void developmentIsON(bool& result, ComponentType& component)
	{
		result &= SwitcherInterface<ComponentType>::developmentIsON(component);
	}

	template<class ComponentType>
	static bool isActive(const ComponentType* client)
	{
		bool result = (sizeof...(Links) > 0);
		auto seq = { (developmentIsON(result, client->template component<Links>()), 0)... };
		return result;
	}

	template<class ComponentType> static void toggle(ComponentType* client)	{ auto seq = { (developmentTOGGLE(client->template component<Links>()), 0)... }; }
	template<class ComponentType> static void switchON(ComponentType* client)	{ auto seq = { (developmentON(client->template component<Links>()), 0)... }; }
	template<class ComponentType> static void switchOFF(ComponentType* client)	{ auto seq = { (developmentOFF(client->template component<Links>()), 0)... }; }
};


template<class Component, class... Links>
class VisualSwitcher : public UpdatableComponentAutoLink<Component, const Position, Links...>
{
public:
	void update(const intersections::Ray& mouseRay)
	{
#ifdef WIN32
		const Vector3 offclr = Vector3(0.7f, 0.7f, 0.7f);
		const Vector3 onclr = Vector3(0.1f, 1.0f, 0.3f);

		bool active = SwitcherBase<Links...>::isActive(this);

		DebugDrawing::color(active ? onclr : offclr);

		auto& state = this->template component<const Position>().state;
		intersections::Sphere sphere(state.position, 2.0f);
		sphere.radius += 0.25f;
		auto aabb = intersections::aabb(sphere);

		DebugDrawing::text(state.position, active ? "ON" : "OFF");
		DebugDrawing::wireBox(aabb);

		if (Input::mouseL(true) && intersections::test(mouseRay, intersections::aabb(sphere)))
		{
			SwitcherBase<Links...>::toggle(this);
		}
#endif // WIN32
	}
};

template<class Component, class... Links>
class AutoSwitcher : public UpdatableComponentAutoLink<Component, Links...>
{
public:
	void update()
	{
		SwitcherBase<Links...>::switchON(this);
	}
};

template<class Component, class... Links>
class AutoToggler : public UpdatableComponentAutoLink<Component, Links...>
{
public:
	void update()
	{
		SwitcherBase<Links...>::toggle(this);
	}
};