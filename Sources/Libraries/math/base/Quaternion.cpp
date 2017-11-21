/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 22.07.2001
	  Time: 19:31:50

	Implemented:	Quaternion class.

*********************************************************************/

#include <math.h>


#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"


#define UNIT_TOLERANCE			0.001
#define QZERO_TOLERANCE			0.00001
#define TRACE_QZERO_TOLERANCE	0.1

// Setup from euler angles
// Angles in radians. Assumes roll is rotation about X, pitch is rotation
// about Y, yaw is rotation about Z.
// p' = roll( pitch( yaw( p ) ) )
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
	// i j h 
	v.x	= cPitch * sc - sPitch * cs;
	v.y = cPitch * ss + sPitch * cc;
	v.z = cPitch * cs - sPitch * sc;

	w	= cPitch * cc + sPitch * ss;
/*	
	cc = cRoll * cPitch;
	sc = sRoll * cPitch;
	cs = cRoll * sPitch;
	ss = sRoll * sPitch;

	v.x = -(sc * cYaw) + (cs * sYaw);
	v.y = -(cc * sYaw) + (ss * cYaw);
	v.z = -(cs * cYaw) + (sc * sYaw);
	w	=  (cc * cYaw) + (ss * sYaw);
	*/
}

// Set from matrix
void
Quaternion::Set( const Matrix3& m )
{
/*
	float qs2, qx2, qy2, qz2;	// squared magnitudes of quat's comp
	float tmp;
	int n;

	//ASSERTF( 0, (TEXT("Buggy!")) );
	// first - compute squared magnitudes of quat components - at least
	// one will be greater than 0 since quaternion is unit magnitude
	qs2	= 0.25f * ( m[0] + m[4] + m[8] + 1.0f );
	qx2 = qs2 - 0.5f * ( m[4] + m[8] );
	qy2 = qs2 - 0.5f * ( m[8] + m[0] );
	qz2	= qs2 - 0.5f * ( m[0] + m[4] );

	// find maximum magnitude component
	n = ( qs2 > qx2 ) ?
		(( qs2 > qy2 ) ? (( qs2 > qz2 ) ? 0 : 3 ) : (( qy2 > qz2 ) ? 2 : 3 ))
		: (( qx2 > qy2 ) ? (( qx2 > qz2 ) ? 1 : 3 ) : (( qy2 > qz2 ) ? 2 : 3 ));

	// compute signed quaternion components using numerically stable method
	switch ( n ) {
	case 0 :
		w	= sqrtf( qs2 );
		tmp	= 0.25f / w;
		v.x	= ( m(2, 1) - m(1, 2) ) * tmp;
		v.y	= ( m(0, 2) - m(2, 0) ) * tmp;
		v.z	= ( m(1, 0) - m(0, 1) ) * tmp;
		break;
	case 1 :
		v.x	= sqrtf( qx2 );
		tmp	= 0.25f / v.x;
		w	= ( m(2, 1) - m(1, 2) ) * tmp;
		v.y	= ( m(0, 1) - m(1, 0) ) * tmp;
		v.z	= ( m(0, 2) - m(2, 0) ) * tmp;
		break;
	case 2 :
		v.y	= sqrtf( qy2 );
		tmp	= 0.25f / v.y;
		w	= ( m(0, 2) - m(2, 0) ) * tmp;
		v.z	= ( m(1, 2) - m(2, 1) ) * tmp;
		v.x	= ( m(1, 0) - m(0, 1) ) * tmp;
		break;
	case 3 :
		v.z	= sqrtf( qz2 );
		tmp	= 0.25f / v.z;
		w	= ( m(1, 0) - m(0, 1) ) * tmp;
		v.x	= ( m(2, 0) - m(0, 2) ) * tmp;
		v.y	= ( m(2, 1) - m(1, 2) ) * tmp;
		break;
	}
	// for consistency, force positive scalar components
	// [ (w; v) = (-w;-v) ]
	if ( w < 0.0f ) {
		w	= -w;
		v.x	= -v.x;
		v.y	= -v.y;
		v.z	= -v.z;
	}
	Normalize();
/////////////////#else
/////////////////#else
/////////////////#else
	float fTrace = m(0,0) + m(1,1) + m(2,2);
	float fRoot;

	if ( fTrace > 0.0f ) {
		// |w| > 1/2
		fRoot = sqrtf( fTrace + 1.0f );
		w = 0.5f * fRoot;
		v.x = ( m(2,1) - m(1,2) ) * fRoot;
		v.y = ( m(0,2) - m(2,0) ) * fRoot;
		v.z = ( m(1,0) - m(0,1) ) * fRoot;
	} else {
		// |w| < 1/2
		static int iNext[3] = { 1, 2, 0 };
		int i  = 0;
		if ( m(1,1) > m(0,0) )
			i = 1;
		if ( m(2,2) > m(i,i) )
			i = 2;
		int j = iNext[i];
		int k = iNext[j];

		fRoot = sqrtf( m(i,i) - m(j,j) - m(k,k) + 1.0f );
		v[i] = 0.5f * fRoot;
		fRoot = 0.5f / fRoot;
		w = ( m(k,j) - m(j,k) ) * fRoot;
		v[j] = ( m(j,i) + m(i,j) ) * fRoot;
		v[k] = ( m(k,i) + m(i,k) ) * fRoot;
	}

///#endif
*/
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

// Set the axis
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


// Gets euler angles
Vector3
Quaternion::GetEulerAngles() const
{
	// NOTE: Optimize !
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
	// unpack matrix
	// axes def : EulOrdXYZs ( static ) Roll Pitch Yaw
	Vector3 a;
	// used matrix entries:
	// 00 10 20 
	// xx 11 21
	// xx 12 22
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

// do Slerp
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
		// SLERP
		angle	= acosf( c );
		s		= sinf( angle );
		sstart	= sinf( (1.0f - t)	* angle ) / s;
		send	= sinf( t			* angle ) / s;
	} else {
		// resort to the lerp, preventing divide by zero
		sstart	= 1.0f - t;
		send	= t;
	}

	res.w	= sstart * start.w		+ send * end2.w;
	res.v.x	= sstart * start.v.x	+ send * end2.v.x;
	res.v.y	= sstart * start.v.y	+ send * end2.v.y;
	res.v.z	= sstart * start.v.z	+ send * end2.v.z;
	
	return res;
}

// do Update
// Thus rotating quaternion in time, using the body rotation rate
// wb, and the time increment, dt.
// wb - angular velocity vector
Quaternion&
Quaternion::Update( const Vector3& wb, float dt )
{
	// calc dq/dt from velocities and current orientation
	operator+= ( GetDQDT( wb ) * dt );  // update current orientation
	Normalize();
	return *this;
	/*
	Quaternion dqdt = GetDQDT( wb );
	Quaternion dq;
	dq.Identity;
	dq.Slerp( dq, dqdt, dt );
	
	*this += dq;
	return *this;*/
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
	//  q.normalize(); Not necessary, but prefered
	Quaternion res = ( GetConjugate() * dqdt ) * 2.0f;
	Vector3 av(res.v.x, res.v.y, res.v.z );
	return av;
	
}

// получить кватернион для разворота объекта в указанном направлении с соблюдением верхней ориентации (вверх)
Quaternion Quaternion::GetUpOrientedDirection(const Vector3& normlizedDirection)
{
	Quaternion  yRot; yRot.Set(atan2f(normlizedDirection.x, normlizedDirection.z), Vector3::yAxis);
	Quaternion  xRot; xRot.Set(atan2f(-normlizedDirection.y, sqrtf(normlizedDirection.x*normlizedDirection.x + normlizedDirection.z*normlizedDirection.z)), Vector3::xAxis);

	//return Quaternion(xRot * yRot);
	return Quaternion(yRot * xRot);
}
