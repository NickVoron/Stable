// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once







#include "math/library.include.h"

namespace CameraSystem
{
	class Params
	{
	public:
		struct BaseData
		{
			State state;	
			float aspect;	
			float fovV;	
			Base::Interval<float> planes; 

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

		
		inline float getFovH() const { return data.fovV * data.aspect; }
		inline float getFovV() const { return data.fovV; }
		inline float getAspect() const { return data.aspect; }

		inline void setFovH(float fovH) { data.fovV = fovH / data.aspect; computeValues(); }
		inline void setFovV(float fovV) { data.fovV = fovV; computeValues();}

		
		inline void setFOVs(float fovH, float fovV) { data.fovV = fovV; data.aspect = fovH / fovV; computeValues();}
		
		
		inline void setAspect(float aspect) { float fovH = getFovH(); data.aspect = aspect;  data.fovV = fovH / aspect; computeValues();}

		
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



// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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