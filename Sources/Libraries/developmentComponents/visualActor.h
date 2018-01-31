// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "baseActor.h"

#include "componentModel/library.include.h"
#include "commonComponents/library.include.h"
#include "intersections/library.include.h"

#ifdef WIN32
#include "debugDrawing/library.include.h"
#endif 


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
#endif 
	}
};



// Copyright (C) 2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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