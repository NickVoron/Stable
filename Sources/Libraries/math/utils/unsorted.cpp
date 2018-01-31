// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#include "unsorted.h"

#include <stdlib.h>
#include <math.h>

#include <algorithm>

#include "../base/Vector2.h"
#include "../base/Vector3.h"
#include "../base/Vector4.h"
#include "../base/Quaternion.h"

#include "newmath/const.h"


#undef min
#undef max

namespace Base {
namespace MathUtils {

bool
GetRandomTrigger( float probablity )
{
	return ( rand()%8192) < probablity * 8192.0f;
}






float
Egy( float x , float p, float h )
{
	float r = 0.5f - ( fabsf( fmodf( fabsf(x), p) / p - 0.5f) );
	return r*h*2.0f;
}


float
Remnants( float x, float m )
{
	float tmp = x;
	if ( tmp < 0 )
		tmp += m * ceil( -tmp );

	return fmodf( tmp, m );
}


float
Signum( float x )
{
	if ( x > 0 ) return 1.0f;
	else if ( x < 0 ) return -1.0f;
	else return 0.0f;
}


float
SegmentDistanceFlat( const Vector3& a, const Vector3& b, float d, const Vector3& c )
{
	Vector3 clientFlat( c.x, 0, c.z );
	Vector3 startFlat( a.x, 0, a.z );
	Vector3 endFlat( b.x, 0, b.z );

	float res = 9999.9f;
	res = std::min( res, ( clientFlat - startFlat ).MagnitudeSquared() );
	res = std::min( res, ( clientFlat - endFlat ).MagnitudeSquared() );

	Vector3 segDirection =	endFlat - startFlat;
	float projLength = (clientFlat - startFlat).Projection( segDirection ) / d;

	
	if ( (projLength > 0) && (projLength < 1) )
	{
		Vector3 sumVec = startFlat + projLength*segDirection;
		res = std::min( res, (clientFlat - sumVec).MagnitudeSquared() );
		
	}

	return sqrtf(res);
	
}


float
HalfPlane( const Vector2& a, const Vector2& b, const Vector2& c )
{
	
	if ( a == b ) return 0.0f;
	if ( a == c ) return 0.0f;
	if ( b == c ) return 0.0f;

	const Vector2 i = a-b;
	const Vector2 q( i.y, -i.x );

	return (c-b).Projection( q );
}

float
HalfPlane( const Vector3& sectionStart, const Vector3& sectionEnd, const Vector3& point )
{
	Vector2 inpStart( sectionStart.x, sectionStart.z );
	Vector2 inpEnd( sectionStart.x, sectionStart.z );
	Vector2 inpPoint( point.x, point.z );
	return HalfPlane( inpStart, inpEnd, inpPoint );

}


int
LogInt2( int x )
{
	
	int tmp = 1;
	for ( int xx = 0; xx < 31; xx++ )
	{
		if ( tmp == x ) return xx;
		tmp *= 2;
	}
	return -1;
}





float Zero( const float& dummy) { return 0.0f; }

int Zero( const int& dummy) { return 0; }

Vector4 Zero( const Vector4& dummy ) { return Vector4( 0.0f, 0.0f, 0.0f, 0.0f ); }

Vector3 Interpolate(const Vector3& v1, const Vector3& v2 , float coef)
{
	return v1 + (v2 - v1) * coef;
}

Vector2 Interpolate(const Vector2& v1, const Vector2& v2 , float coef)
{
	return v1 + (v2 - v1) * coef;
}

float GetAzimuthNeg( const Vector3& point, const Vector3& targ )
{
	
	if ( ( point.x == targ.x ) && ( point.z == targ.z ) )
		return 0;
	else 
		return atan2f( (targ.x - point.x), -(targ.z - point.z) );
}

float GetAzimuthNeg( const Vector2& point, const Vector2& targ )
{
	
	if ( ( point.x == targ.x ) && ( point.y == targ.y ) )
		return 0;
	else 
		return atan2f( (targ.x - point.x), -(targ.y - point.y) );
}

float GetAzimuthPos( const Vector3& point, const Vector3& targ )
{
	
	if ( ( point.x == targ.x ) && ( point.z == targ.z ) )
		return 0;
	else 
		return atan2f( (targ.x - point.x), (targ.z - point.z) );
}

float GetAzimuthPos( const Vector2& point, const Vector2& targ )
{
	
	if ( ( point.x == targ.x ) && ( point.y == targ.y ) )
		return 0;
	else 
		return atan2f( (targ.x - point.x), (targ.y - point.y) );
}




float GetVertical( const Vector3& point, const Vector3& targ )
{
	
	if ( ( point.y == targ.y ) && ( point.z == targ.z ) )
		return 0;
	else 
	{
		Vector2 hs = Vector2( targ.x - point.x, targ.z - point.z );
		return -atan2f( targ.y - point.y , hs.Magnitude() );
	}
}


Vector3 postRotate(Vector3& v, const Quaternion& q) 
{
	Vector3 c = q.v.SCross(v);
	return ((q.w*q.w)*v + q.v.SDot(v)*q.v + (2.0f*q.w)*c +  q.v.SCross(c)) / q.MagnitudeSquared();
}
Vector3 preRotate(Vector3& v, const Quaternion& q)
{
	Vector3 c = v.SCross(q.v);
	return ((q.w*q.w)*v + q.v.SDot(v)*q.v + (2.0f*q.w)*c + c.SCross(q.v)) / q.MagnitudeSquared();
}

float QuaternionHeading(const Quaternion& q)
{
	float x = q.v.x;
	float y = q.v.y;
	float z = q.v.z;
	float w = q.w;

	float test = x * y + z * w;

	if( !( test > 0.499f ||  test < -0.499f) )
	{
		return atan2f(2.0f*y*w - 2.0f*x*z, 1.0f-2.0f*y*y - 2.0f*z*z);
	}
	else
	{
		float r = 2.0f * atan2f(x, w);
		return ( test > 0.499f ) ? r : -r;
	}
}

float QuaternionPitch(const Quaternion& q)
{
	float x = q.v.x;
	float y = q.v.y;
	float z = q.v.z;
	float w = q.w;

	float test = x * y + z * w;
	if( !( test > 0.499f ||  test < -0.499f) )
	{
		return -atan2f(2.0f*x*w - 2.0f*y*z, 1.0f - 2.0f*x*x - 2.0f*z*z);
	}
	else
	{
		return ( test > 0.499f ) ? nm::PI_2 : -nm::PI_2;
	}
}

float QuaternionBank(const Quaternion& q)
{
	float x = q.v.x;
	float y = q.v.y;
	float z = q.v.z;
	float w = q.w;

	float test = x * y + z * w;
	return !( test > 0.499f ||  test < -0.499f) ? asinf(2.0f * test) : 0.0f;
}

Vector3 QuaterionToHPB(Quaternion qu)
{
	Vector3 result;
	float x = qu.v.x;
	float y = qu.v.y;
	float z = qu.v.z;
	float w = qu.w;
	float& h = result.x;
	float& p = result.y;
	float& b = result.z;

	float test = x * y + z * w;
	if( !( test > 0.499f ||  test < -0.499f) )
	{
		float tx = 2.0f * x; 
		float ty = 2.0f * y;
		float o = 1.0f - 2.0f*z*z;
		h = atan2f(ty * w - tx * z, o - ty*y);
		p = -atan2f(tx * w - ty * z, o - tx*x);
		b = -asinf(2 * test);
	}
	else
	{
		
		b = 0;
		h = 2 * atan2f(x, w);
		p = nm::PI_2;

		if (test < -0.499f) 
		{ 
			h = -h;
			p = -p;
		}
	}

	return result;
}

Quaternion HPBtoQuaternion(Vector3 hpb)
{
	Quaternion x; x.SetXAxis(-hpb.y);
	Quaternion y; y.SetYAxis(hpb.x);
	Quaternion z; z.SetZAxis(hpb.z);
	return y * x * z;
}

void buildPlaneBasis(const Vector3& n, Vector3& p, Vector3& q)
{
	if (fabsf(n.z) > nm::SQRT1_2)
	{
		
		float k = sqrtf(n.y*n.y + n.z*n.z);
		p.x = 0; p.y = -n.z/k; p.z = n.y/k;
		
		q.x = k; q.y = -n.x*p.z; q.z = n.x*p.y;
	}
	else 
	{
		
		float k = sqrtf(n.x*n.x + n.y*n.y);
		p.x = -n.y/k; p.y = n.x/k; p.z = 0;
		
		q.x = -n.z*p.y; q.y = n.z*p.x; q.z = k;
	}
}

Matrix3 changeBasisMatrix(const Vector3& srcNormal, const Vector3& dstNormal)
{
	Vector3 s0, s1, sn = srcNormal;
	Base::MathUtils::buildPlaneBasis(sn, s0, s1);

	Vector3 d0, d1, dn = dstNormal;
	Base::MathUtils::buildPlaneBasis(dn, d0, d1);

	Matrix3 src(sn.x, s0.x, s1.x,
		sn.y, s0.y, s1.y,
		sn.z, s0.z, s1.z);

	Matrix3 dst(dn.x, d0.x, d1.x,
		dn.y, d0.y, d1.y,
		dn.z, d0.z, d1.z);

	src.Invert();

	return src * dst;
}




void SplitFloat( float input, float desiredSize, float& size, int& splices )
{
	float ct = std::max( 1.0f, floorf( (input / desiredSize ) + 0.5f ) );
	size = input / ct;
	splices = static_cast<int>( ct );
	
}



} 
} 




// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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