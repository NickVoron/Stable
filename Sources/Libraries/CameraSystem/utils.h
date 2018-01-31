// Copyright (C) 2012-2015 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

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

	
	
	
	
	
	inline float calculateOptimalDistanceZ(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.x, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	
	inline float calculateOptimalDistanceX(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.y);
		return (extent * invTgHalfFov) + extent;
	}

	
	inline float calculateOptimalDistanceY(float invTgHalfFov, const AABBCenterExtent& bbox)
	{
		float extent = std::max(bbox.extent.z, bbox.extent.x);
		return (extent * invTgHalfFov) + extent;
	}

	
	
	
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