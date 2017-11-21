#pragma once

#include "CameraSystem/library.include.h"
#include "cameraControllers/library.include.h"
#include "intersections/library.include.h"
#include "unigui/library.include.h"

namespace Viewports
{
	// получить параметры камеры из менеджера
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
