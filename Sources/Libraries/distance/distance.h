// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include "intersections/library.include.h"

namespace intersections
{
	template<class Cls> struct DistInfo;

	template<> struct DistInfo<Vector3> {};
	template<> struct DistInfo<nm::Vector3> {};
	template<> struct DistInfo<Segment3>	{	float segmentParam; };
	template<> struct DistInfo<Line>		{	float lineParam; };
	template<> struct DistInfo<Triangle>	{	float barycentric[3];	};

	template<class T0, class T1> struct DistParams
	{
		DistInfo<T0> info0;
		DistInfo<T1> info1;
	};

	float distanceSq(const Vector3& point,	const Segment3& segment,	Segment3& res,	DistParams<Vector3, Segment3>&	params);
	float distanceSq(const Segment3& s0,	const Segment3& s1,			Segment3& res,	DistParams<Segment3, Segment3>& params);
	float distanceSq(const Vector3& point,	const Triangle& triangle,	Segment3& res,	DistParams<Vector3, Triangle>&	params);
	float distanceSq(const Line& line,		const Segment3& triangle,	Segment3& res,	DistParams<Line, Segment3>&		params);
	float distanceSq(const Line& line,		const Triangle& triangle,	Segment3& res,	DistParams<Line, Triangle>&		params);
	float distanceSq(const Segment3& s0,	const Triangle& triangle,	Segment3& res,	DistParams<Segment3, Triangle>& params);
	float distanceSq(const Triangle& t0,	const Triangle& t1,			Segment3& res,	DistParams<Triangle, Triangle>& params);









	template<class T0, class T1>
	float distanceSq(const T0& point, const T1& segment, Segment3& res)
	{
		DistParams<T0, T1> params;
		return distanceSq(point, segment, res, params);
	}


	template<class T0, class T1>
	float distance(const T0& point, const T1& segment, Segment3& res)
	{
		return sqrtf( distanceSq(point, segment, res) );
	}







}




// Copyright (C) 2012-2017 Denis Netakhin <denis.netahin@yandex.ru>, Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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