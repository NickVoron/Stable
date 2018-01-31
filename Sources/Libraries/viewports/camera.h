// Copyright (C) 2013-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "CameraSystem/library.include.h"
#include "cameraControllers/library.include.h"
#include "intersections/library.include.h"
#include "unigui/library.include.h"

namespace Viewports
{
	
	void GetMatricesFromCameraParams(const CameraSystem::Params& params, nm::matrix4& projection, nm::matrix4& view, nm::matrix4& invView);

	struct CalculatedCamera
	{
		CalculatedCamera();
		CalculatedCamera(const CalculatedCamera& camera);

		void calculate(const CameraSystem::Params& params);
		void calculate();

		bool screenPos(const nm::Vector3& worldCoord, nm::Vector3& screenCoord) const;
		bool screenPos(const nm::Vector3& worldCoord, float& screenX, float& screenY) const;
		bool screenPos(const nm::Vector3& worldCoord, nm::index2& screenCoord) const;
		void worldPos(const nm::index2& screenCoord, nm::Vector3& pos) const;
		void worldRay(const nm::index2& screenCoord, nm::Vector3& pos, nm::Vector3& dir) const;
		void worldPos(const nm::Vector2& screenPos, nm::Vector3& pos) const;
		void worldRay(const nm::Vector2& screenPos, nm::Vector3& pos, nm::Vector3& dir) const;
		void worldFrustum(const unigui::Rect& screenRect, intersections::Frustum& frustum) const;
		void worldFrustum(intersections::Frustum& frustum) const;

		intersections::Ray worldRay(const nm::index2& screenCoord) const;
		intersections::Ray worldRay(const nm::Vector2& screenPos) const;
		intersections::Frustum worldFrustum(const unigui::Rect& screenRect) const;
		intersections::Frustum worldFrustum(const nm::index2& screenCoord0, const nm::index2& screenCoord1) const;
		intersections::Frustum worldFrustum() const;

		CalculatedCamera& operator=(const CalculatedCamera& camera);

		CameraSystem::Params params;

		State& state;
		nm::Vector3& position;
		nm::Vector3 direction;

		nm::matrix4 world;
		nm::matrix4 view;
		nm::matrix4 projection;
		nm::matrix4 viewProjection;
		nm::matrix4 invView;

		nm::index2 viewportSize;
	};
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