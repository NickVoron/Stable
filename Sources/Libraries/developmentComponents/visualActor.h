#pragma once

#include "baseActor.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"
#include "intersections/library.include.h"

#ifdef WIN32
#include "debugDrawing/library.include.h"
#endif // WIN32


template<template <class> class ActorInterface, class Component, class... Links>
class VisualActor : public UpdatableComponentAutoLink<Component, const Position, Links...>
{
public:
	void update(const intersections::Ray& mouseRay)
	{
#ifdef WIN32
		auto& state = this->template component<const Position>().state;
		intersections::Sphere sphere(state.position, 2.0f);
		sphere.radius += 0.25f;
		auto aabb = intersections::aabb(sphere);

		bool underMouse = intersections::test(mouseRay, intersections::aabb(sphere));

		const Vector3 offclr = Vector3(0.7f, 0.7f, 0.7f);
		const Vector3 onclr = Vector3(0.1f, 1.0f, 0.3f);

		DebugDrawing::color(underMouse ? onclr : offclr);

		DebugDrawing::text(state.position, ActorBase<ActorInterface, Links...>::description(this));
		DebugDrawing::wireBox(aabb);

		if (Input::mouseL(true) && underMouse)
		{
			ActorBase<ActorInterface, Links...>::action(this);
		}
#endif // WIN32
	}
};