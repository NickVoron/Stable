// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "vector.h"
#include "polar3.h"

namespace nm
{	
	float azimuth(const Vector2& point, const Vector2& target);
	float azimuth(const Vector3& point, const Vector3& target);

	float vertical(const Vector3& point, const Vector3& target);

	float degrees(float radians);
	float radians(float degrees);
	Vector3 degrees(const Vector3& radians);
	Vector3 radians(const Vector3& degrees);

	float simplify_angle(float radians);
	float substract_angle(float radians0, float radians1);
	Vector3 simplify_angles(const Vector3& radians);
	Vector3 substract_angles(const Vector3& radians0, const Vector3& radians1);


	Vector3 convertPolar3ToVector3(const Polar3& polar);
	Polar3 convertVector3ToPolar3(const Vector3& vector);
}










// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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