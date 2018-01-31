// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//



#include <math.h>


#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"


#define UNIT_TOLERANCE			0.001
#define QZERO_TOLERANCE			0.00001
#define TRACE_QZERO_TOLERANCE	0.1





void
Quaternion::Set( float r, float p, float y )
{
	float	cRoll, sRoll, cPitch, sPitch, cYaw, sYaw;
	float	halfRoll, halfPitch, halfYaw;
	float	ss, cc, sc, cs;

	halfRoll	= r * 0.5f;
	halfPitch	= p * 0.5f;
	halfYaw		= y * 0.5f;

	cRoll		= cosf( halfRoll );
	sRoll		= sinf( halfRoll );

	cPitch		= cosf( halfPitch );
	sPitch		= sinf( halfPitch );

	cYaw		= cosf( halfYaw );
	sYaw		= sinf( halfYaw );

	cc = cRoll * cYaw;
	cs = cRoll * sYaw;
	sc = sRoll * cYaw;
	ss = sRoll * sYaw;
	
	v.x	= cPitch * sc - sPitch * cs;
	v.y = cPitch * ss + sPitch * cc;
	v.z = cPitch * cs - sPitch * sc;

	w	= cPitch * cc + sPitch * ss;

}


void
Quaternion::Set( const Matrix3& m )
{

	float trace;
	float s;
	int i,j,k;
	float out[4];

	static int next[3] = { 1, 2, 0 };

	trace = m(0,0) + m(1,1) + m(2,2);

	if ( trace > 0.0f ) {
		s = (float)sqrt( trace + 1.0f );
		w = 0.5f * s;
		s = 0.5f / s;
		v.x = ( m(1,2) - m(2,1) ) * s;
		v.y = ( m(2,0) - m(0,2) ) * s;
		v.z = ( m(0,1) - m(1,0) ) * s;
	} else {
		i = 0;
		if ( m(1,1) > m(0,0) ) 	i = 1;
		if ( m(2,2) > m(i,i) ) 	i = 2;

		j = next[i];
		k = next[j];

		s = m(i,i) - ( m(j,j) + m(k,k) );


		s = (float)sqrt( s + 1.0f );

		out[i]	= s * 0.5f;
		s		= 0.5f / s;
		out[j]	= ( m(i,j) + m(j,i) ) * s;
		out[k]	= ( m(i,k) + m(k,i) ) * s;
		out[3]	= ( m(j,k) - m(k,j) ) * s;

		v.x = out[0];
		v.y = out[1];
		v.z = out[2];
		w = out[3];
	}
}


Quaternion&
Quaternion::Set( float angle, const Vector3& axis, bool normalizeAxis )
{
	float theta, sine;

	theta	= 0.5f * angle;
	sine	= sinf( theta );
	w		= cosf( theta );

	if  ( normalizeAxis ) {
		Vector3 axis0 = axis.GetNormalized();

		v.x	= axis0.x * sine;
		v.y	= axis0.y * sine;
		v.z	= axis0.z * sine;
	} else {
		v.x	= axis.x * sine;
		v.y	= axis.y * sine;
		v.z	= axis.z * sine;
	}
	return *this;
}



Vector3
Quaternion::GetEulerAngles() const
{
	
	Matrix3 m;

	float nq = MagnitudeSquared();
	float s	= ( nq > 0.0f ) ? ( 2.0f / nq ) : 0.0f;	
	float xs = v.x * s,		ys = v.y * s,	zs = v.z * s;
	float wx = w * xs,		wy = w * ys,	wz = w * zs;
	float xx = v.x * xs,	xy = v.x * ys,	xz = v.x * zs;
	float yy = v.y * ys,	yz = v.y * zs,	zz = v.z * zs;
	m(0,0) = 1.0f - ( yy + zz );	m(0,1) = xy - wz;	m(0,2) = xz + wy;
	m(1,0) = xy + wz;	m(1,1) = 1.0f - ( xx + zz );	m(1,2) = yz - wx;
	m(2,0) = xz - wy;	m(2,1) = yz + wx;	m(2,2) = 1.0f - ( xx + yy );
	
	
	Vector3 a;
	
	
	
	
	float cy = sqrtf( m(0,0) * m(0,0) + m(1,0) * m(1,0) );
	if ( cy > 16 * 0.000001f) {
		a.x = atan2f( m(2,1), m(2,2) );
		a.y = atan2f( -m(2,0), cy );
		a.z = atan2f( m(1,0), m(0,0) );
	} else {
		a.x = atan2f( -m(1,2), m(1,1) );
		a.y = atan2f( m(2,0), cy );
		a.z = 0.0f;
	}
	
	return a;
}


Quaternion Quaternion::Slerp( const Quaternion& start, const Quaternion& end, float t, bool shortest )
{
	Quaternion res;
	Quaternion end2;
	float angle;
	float c,s;
	float sstart, send;

	c = start.SDot( end );

	if (((shortest) && ( c < 0.0f )) ||
		((!shortest)&& ( c > 0.0f ))) {

		c	= -c;
		end2.v.x= -end.v.x;
		end2.v.y= -end.v.y;
		end2.v.z= -end.v.z;
		end2.w	= -end.w;
	} else {
		end2	= end;
	}

	if ( (1.0f - fabsf( c )) > 0.000001f) 
	{
		
		angle	= acosf( c );
		s		= sinf( angle );
		sstart	= sinf( (1.0f - t)	* angle ) / s;
		send	= sinf( t			* angle ) / s;
	} else {
		
		sstart	= 1.0f - t;
		send	= t;
	}

	res.w	= sstart * start.w		+ send * end2.w;
	res.v.x	= sstart * start.v.x	+ send * end2.v.x;
	res.v.y	= sstart * start.v.y	+ send * end2.v.y;
	res.v.z	= sstart * start.v.z	+ send * end2.v.z;
	
	return res;
}





Quaternion&
Quaternion::Update( const Vector3& wb, float dt )
{
	
	operator+= ( GetDQDT( wb ) * dt );  
	Normalize();
	return *this;
	
}

Quaternion
Quaternion::GetDQDT( const Vector3& angle ) const
{
	Quaternion q( *this );
	Quaternion aq;
	aq.v = angle;
	aq.w = 0.0f;

	return ( q * aq ) * 0.5f;
}

Vector3
Quaternion::GetVelocity( const Quaternion dqdt ) const
{
	
	Quaternion res = ( GetConjugate() * dqdt ) * 2.0f;
	Vector3 av(res.v.x, res.v.y, res.v.z );
	return av;
	
}


Quaternion Quaternion::GetUpOrientedDirection(const Vector3& normlizedDirection)
{
	Quaternion  yRot; yRot.Set(atan2f(normlizedDirection.x, normlizedDirection.z), Vector3::yAxis);
	Quaternion  xRot; xRot.Set(atan2f(-normlizedDirection.y, sqrtf(normlizedDirection.x*normlizedDirection.x + normlizedDirection.z*normlizedDirection.z)), Vector3::xAxis);

	
	return Quaternion(yRot * xRot);
}




// Copyright (C) 2012-2015 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
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