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

// 	float distanceSq(const Vector3& point, const Segment3& segment, Segment3& res);
// 	float distanceSq(const Segment3& s0, const Segment3& s1, Segment3& res);
// 	float distanceSq(const Vector3& point, const Triangle& triangle, Segment3& res);
// 	float distanceSq(const Line& line, const Segment3& triangle, Segment3& res);
// 	float distanceSq(const Line& line, const Triangle& triangle, Segment3& res);
// 	float distanceSq(const Segment3& s0, const Triangle& triangle, Segment3& res);
// 	float distanceSq(const Triangle& t0, const Triangle& t1, Segment3& res);

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
// 
// 	float distance(const Segment3& s0, const Segment3& s1, Segment3& res);
// 	float distance(const Vector3& point, const Triangle& triangle, Segment3& res);
// 	float distance(const Line& line, const Segment3& triangle, Segment3& res);
// 	float distance(const Line& line, const Triangle& triangle, Segment3& res);
// 	float distance(const Segment3& s0, const Triangle& triangle, Segment3& res);
// 	float distance(const Triangle& t0, const Triangle& t1, Segment3& res);
}
