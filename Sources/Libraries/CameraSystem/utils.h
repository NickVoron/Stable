#pragma once

#include <algorithm>

#include "params.h"

#undef min
#undef max

namespace CameraSystem
{
	struct AABBCenterExtent
	{
		Vector3 position;
		Vector3 extent;
	};

	// расчитать дистанцию от центра AABB с которой камера четко его охватит, бокс будет "на весь экран"
	// для камеры которая смотрит вдоль оси Z
	// функция использует предрасчитанные данные о камере
	// invTgHalfFov - должен быть равен: 1.0f / tanf( min(horizontalFOV, verticalFOV) * 0.5f )
	// для камеры которая смотрит вдоль оси Z
	inline float calculateOptimalDistanceZ(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.x, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	// для камеры которая смотрит вдоль оси X
	inline float calculateOptimalDistanceX(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	// для камеры которая смотрит вдоль оси Y
	inline float calculateOptimalDistanceY(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.x);
		return (extent * invTgHalfFov) + extent;
	}

	// расчитать дистанцию от центра сферы с которой камера четко её охватит, сфера будет "на весь экран"
	// функция использует предрасчитанные данные о камере
	// invSinHalfFov - должен быть равен: 1.0f / sinf( min(horizontalFOV, verticalFOV) * 0.5f )
	inline float calculateOptimalDistance(float invSinHalfFov, float radius)
	{
		return radius * invSinHalfFov;
	}

	inline void optimalObjectView(CameraSystem::Params& p, const AABBCenterExtent& bbox)
	{
		State& s = p.getStateRef();
		s.SetIdentity();
		s.position.z = -calculateOptimalDistanceZ(p.getComputedData().invTgMinHalfFov, bbox); 
	}

	inline void optimalObjectView(CameraSystem::Params& p, float radius)
	{
		State& s = p.getStateRef();
		s.SetIdentity(); 
		s.position.z = -calculateOptimalDistance(p.getComputedData().invSinMinHalfFov, radius); 
	}
}