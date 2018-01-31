// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "params.h"
#include "newmath/library.include.h"
#include <algorithm>

#undef min
#undef max

using namespace CameraSystem;
using namespace Base;

void Params::defaults()
{
	data.state.SetIdentity();

	data.aspect = 4.0f / 3.0f;
	setFovV(45.0f * nm::PI_180);
	
	data.planes.minValue = 0.1f;
	data.planes.maxValue = 1024.0f;

	computeValues();
}

void Params::BaseData::setInterpolated(const Params::BaseData& start, const Params::BaseData& end, float k1)
{
	state.position = nm::lerp(start.state.position, end.state.position, k1);
	state.orientation = start.state.orientation.Slerp(start.state.orientation, end.state.orientation, k1);

	aspect	= nm::lerp(start.aspect, end.aspect, k1);
	fovV	= nm::lerp(start.fovV, end.fovV, k1);

	planes.minValue = nm::lerp(start.planes.minValue, end.planes.minValue, k1);
	planes.maxValue = nm::lerp(start.planes.maxValue, end.planes.maxValue, k1);

}


void Params::setInterpolated(const Params& start, const Params& end, float k1)
{
	data.setInterpolated( start.data, end.data, k1 );
	computeValues();
}

void Params::lookTo(const Vector3& point)
{
	data.state.LookToPoint(point);
}

void Params::computeValues()
{
	float halfFovV = getFovV() * 0.5f;
	float halfFovH = getFovH() * 0.5f;
	float minF = std::min( halfFovV, halfFovH );
	float maxF = std::max( halfFovV, halfFovH );
	computedData.minHalfFov = minF;
	computedData.maxHalfFov = maxF;
	
	computedData.sinMinHalfFov = sinf(minF);
	computedData.sinMaxHalfFov = sinf(maxF);
	computedData.tgMinHalfFov = tanf(minF);
	computedData.tgMaxHalfFov = tanf(maxF);
	computedData.invSinMinHalfFov = 1.0f / computedData.sinMinHalfFov;
	computedData.invSinMaxHalfFov = 1.0f / computedData.sinMaxHalfFov;
	computedData.invTgMinHalfFov = 1.0f / computedData.tgMinHalfFov;
	computedData.invTgMaxHalfFov = 1.0f / computedData.tgMaxHalfFov;
};




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
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