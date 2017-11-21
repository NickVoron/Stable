#pragma once
// параметры камеры
// параметры по умолчанию:
// pos (0,0,0)
// dir (0,0,0,1)
// aspect (4/3 = 1.3333...)
// fov_y (67.5, при этом fov_x для aspect = 4/3 будет 90 градусов)

#include "math/library.include.h"

namespace CameraSystem
{
	class Params
	{
	public:
		struct BaseData
		{
			State state;	// позиция и ориентация
			float aspect;	// отношение fov - горизонтальный / вертикальный (обычно > 1)
			float fovV;	// вертикальный fov в радианах
			Base::Interval<float> planes; // ближняя и дальняя плоскости отсечения

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

		// получить горизонтальный fov
		inline float getFovH() const { return data.fovV * data.aspect; }
		inline float getFovV() const { return data.fovV; }
		inline float getAspect() const { return data.aspect; }

		inline void setFovH(float fovH) { data.fovV = fovH / data.aspect; computeValues(); }
		inline void setFovV(float fovV) { data.fovV = fovV; computeValues();}

		// установить два угла fov, aspect будет автоматически посчитан
		inline void setFOVs(float fovH, float fovV) { data.fovV = fovV; data.aspect = fovH / fovV; computeValues();}
		
		// установить acpect пикселя(пересчитывая горизонтальный FOV)
		inline void setAspect(float aspect) { float fovH = getFovH(); data.aspect = aspect;  data.fovV = fovH / aspect; computeValues();}

		// устновить в себя интерполированное значение
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