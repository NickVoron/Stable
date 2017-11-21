#pragma once
// ��������� ������
// ��������� �� ���������:
// pos (0,0,0)
// dir (0,0,0,1)
// aspect (4/3 = 1.3333...)
// fov_y (67.5, ��� ���� fov_x ��� aspect = 4/3 ����� 90 ��������)

#include "math/library.include.h"

namespace CameraSystem
{
	class Params
	{
	public:
		struct BaseData
		{
			State state;	// ������� � ����������
			float aspect;	// ��������� fov - �������������� / ������������ (������ > 1)
			float fovV;	// ������������ fov � ��������
			Base::Interval<float> planes; // ������� � ������� ��������� ���������

			bool ortho = false;

			void setInterpolated(const BaseData& start, const BaseData& end, float k1);
		};

		struct ComputedData 
		{
			float minHalfFov;
			float maxHalfFov;
			float sinMinHalfFov;
			float sinMaxHalfFov;
			float tgMinHalfFov;
			float tgMaxHalfFov;
			float invSinMinHalfFov;
			float invSinMaxHalfFov;
			float invTgMinHalfFov;
			float invTgMaxHalfFov;
		};

		Params(){ defaults(); }

		void defaults();

		// �������� �������������� fov
		inline float getFovH() const { return data.fovV * data.aspect; }
		inline float getFovV() const { return data.fovV; }
		inline float getAspect() const { return data.aspect; }

		inline void setFovH(float fovH) { data.fovV = fovH / data.aspect; computeValues(); }
		inline void setFovV(float fovV) { data.fovV = fovV; computeValues();}

		// ���������� ��� ���� fov, aspect ����� ������������� ��������
		inline void setFOVs(float fovH, float fovV) { data.fovV = fovV; data.aspect = fovH / fovV; computeValues();}
		
		// ���������� acpect �������(������������ �������������� FOV)
		inline void setAspect(float aspect) { float fovH = getFovH(); data.aspect = aspect;  data.fovV = fovH / aspect; computeValues();}

		// ��������� � ���� ����������������� ��������
		void setInterpolated(const Params& start, const Params& end, float k1);

		const ComputedData& getComputedData() const { return computedData; } 
		const BaseData& getData() const { return data; }

		const State& getState() const  { return data.state; }
		State& getStateRef() { return data.state; }
		Base::Interval<float>& getPlanesRef() { return data.planes; }

		float farPlane() const	{ return data.planes.maxValue; }
		float nearPlane() const { return data.planes.minValue; }

		void lookTo(const Vector3& point);

	private:
		void computeValues();

		BaseData data;
		ComputedData computedData;
	};

}; // end namespace CameraSystem