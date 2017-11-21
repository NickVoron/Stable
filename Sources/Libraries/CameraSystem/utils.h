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

	// ��������� ��������� �� ������ AABB � ������� ������ ����� ��� �������, ���� ����� "�� ���� �����"
	// ��� ������ ������� ������� ����� ��� Z
	// ������� ���������� ��������������� ������ � ������
	// invTgHalfFov - ������ ���� �����: 1.0f / tanf( min(horizontalFOV, verticalFOV) * 0.5f )
	// ��� ������ ������� ������� ����� ��� Z
	inline float calculateOptimalDistanceZ(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.x, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	// ��� ������ ������� ������� ����� ��� X
	inline float calculateOptimalDistanceX(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	// ��� ������ ������� ������� ����� ��� Y
	inline float calculateOptimalDistanceY(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.x);
		return (extent * invTgHalfFov) + extent;
	}

	// ��������� ��������� �� ������ ����� � ������� ������ ����� � �������, ����� ����� "�� ���� �����"
	// ������� ���������� ��������������� ������ � ������
	// invSinHalfFov - ������ ���� �����: 1.0f / sinf( min(horizontalFOV, verticalFOV) * 0.5f )
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