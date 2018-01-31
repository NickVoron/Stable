// Copyright (C) 2012-2016 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"
















































































class Quaternion {
public:

	
	Vector3	v;					
	float	w;					

	
	inline Quaternion();
	
	
	inline Quaternion( float r, float p, float y );
	inline Quaternion( const Vector3& euler );

	
	inline Quaternion( const Matrix3& m );

	
	inline Quaternion( float x, float y, float z, float w );
	inline Quaternion( const Vector3& v, float w );
	inline Quaternion( const Vector4& v );

	

	
	void Set( float roll, float pitch, float yaw  );
	inline void Set( const Vector3& euler );

	
	void Set( const Matrix3& m );

	
	inline void Set( float qx, float qy, float qz, float qw );
	inline void Set( const Vector3& qv, float qw );

	
	Quaternion& Set( float angle, const Vector3& axis, bool normalizeAxis = false );

	inline Quaternion& SetXAxis( float angle );
	inline Quaternion& SetYAxis( float angle );
	inline Quaternion& SetZAxis( float angle );

	
	inline Vector3	Axis() const;

	
	inline float	Angle() const;

	
	inline void Identity();
	inline bool IsIdentity() const;
	static inline Quaternion GetIdentity();

	

	
	Vector3 GetEulerAngles() const;

	
	
	inline void GetTransformation( Matrix3& m ) const;

	
	
	
	inline void GetTransformation( Matrix4& m, bool only3x3 = true ) const;

	
	inline Vector3 GetXAxis() const;
	inline void GetXAxis( Vector3& axis ) const;

	
	inline Vector3 GetYAxis() const;
	inline void GetYAxis( Vector3& axis ) const;

	
	inline Vector3 GetZAxis() const;
	inline void GetZAxis( Vector3& axis ) const;

	
	inline Quaternion& FromAxes( const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis );
	inline void ToAxes( Vector3& xAxis, Vector3& yAxis, Vector3& zAxis ) const;

	
	
	inline Vector3 Transform( const Vector3& o ) const;
	
	inline Vector3 InverseTransform( const Vector3& o ) const;

	

	
	
	
	
	static Quaternion Slerp( const Quaternion& start, const Quaternion& end, float time, bool shortest = true );

	
	Quaternion& Update( const Vector3& wb, float dt );
	Quaternion GetDQDT( const Vector3& angle ) const;
	Vector3 GetVelocity( const Quaternion dqdt ) const;

	
	static Quaternion GetUpOrientedDirection(const Vector3& normlizedDirection);


	
	inline Quaternion& operator *= ( float s );
	inline Quaternion& operator /= ( float s );
	inline Quaternion operator * ( float s ) const;
	inline Quaternion operator / ( float s ) const;

	

	inline Quaternion GetInverse() const;
	inline Quaternion& Invert();

	
	inline float SDot( const Quaternion& q ) const;

	
	inline float Magnitude() const;
	inline float MagnitudeSquared() const;

	
	inline Quaternion GetConjugate() const;
	inline Quaternion& Conjugate();

	
	inline Quaternion GetNormalized() const;
	inline Quaternion& Normalize();

	
	inline Quaternion&	operator += ( const Quaternion& q );
	inline Quaternion&	operator -= ( const Quaternion& q );

	inline Quaternion&	operator *= ( const Quaternion& q );
	inline Quaternion&	operator /= ( const Quaternion& q );

	inline Quaternion operator + ( const Quaternion& q ) const;
	inline Quaternion operator - ( const Quaternion& q ) const;
	inline Quaternion operator * ( const Quaternion& q ) const;
	inline Quaternion operator / ( const Quaternion& q ) const;

	
	inline bool Equal( const Quaternion& q, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Quaternion& q, float tolerance = 0.000001f) const;

	
	inline bool operator == ( const Quaternion& q ) const;
	inline bool operator != ( const Quaternion& q ) const;

};





Quaternion::Quaternion()
{}

Quaternion::Quaternion( float r, float p, float y )
{
	Set( r, p ,y );
}

Quaternion::Quaternion( const Vector3& euler )
{
	Set( euler.x, euler.y, euler.z );
}

Quaternion::Quaternion( const Matrix3& m )
{
	Set( m );
}

Quaternion::Quaternion( float qx, float qy, float qz, float qw )
{
	Set( qx, qy, qz, qw );
}

Quaternion::Quaternion( const Vector3& qv, float qw )
{
	Set( qv, qw );
}

Quaternion::Quaternion( const Vector4& v4 )
{
	Set( v4.x, v4.y, v4.z, v4.w );
}



void Quaternion::Set( const Vector3& euler )
{
	Set( euler.x, euler.y, euler.z );
}

void Quaternion::Set( float qx, float qy, float qz, float qw )
{
	v.x	= qx;
	v.y	= qy;
	v.z	= qz;
	w	= qw;
	Normalize();
}

void Quaternion::Set( const Vector3& qv, float qw )
{
	v	= qv;
	w	= qw;
	Normalize();
}


Quaternion& Quaternion::SetXAxis( float angle )
{
	w	= cosf( 0.5f * angle );
	v.x	= sinf( 0.5f * angle );
	v.y	= 0.0f;
	v.z	= 0.0f;
	return *this;
}

Quaternion& Quaternion::SetYAxis( float angle )
{
	w	= cosf( 0.5f * angle );
	v.x	= 0.0f;
	v.y	= sinf( 0.5f * angle );
	v.z	= 0.0f;
	return *this;
}

Quaternion& Quaternion::SetZAxis( float angle )
{
	w	= cosf( 0.5f * angle );
	v.x	= 0.0f;
	v.y	= 0.0f;
	v.z	= sinf( 0.5f * angle );
	return *this;
}





Vector3 Quaternion::Axis() const
{
	if ( v.MagnitudeSquared() < 0.00001 ) {
		return Vector3::zAxis;
	} else {
		return v.GetNormalized();
	}
}

float Quaternion::Angle() const
{
	return 2.0f * acosf( w );
}


void Quaternion::Identity()
{
	v.Zero();
	w = 1.0f;
}

bool Quaternion::IsIdentity() const
{
	return w == 1.0f;
}

Quaternion Quaternion::GetIdentity()
{
	Quaternion r;
	r.Identity();
	return r;
}



void Quaternion::GetTransformation( Matrix3& m ) const
{
	

	


	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	x2 = v.x + v.x;    y2 = v.y + v.y;    z2 = v.z + v.z;
	xx = v.x * x2;   xy = v.x * y2;   xz = v.x * z2;
	yy = v.y * y2;   yz = v.y * z2;   zz = v.z * z2;
	wx = w * x2;   wy = w * y2;   wz = w * z2;

	m(0, 0) = 1.0f - (yy + zz);
	m(1, 0) = xy - wz;
	m(2, 0) = xz + wy;

	m(0, 1) = xy + wz;
	m(1, 1) = 1.0f - (xx + zz);
	m(2, 1) = yz - wx;

	m(0, 2) = xz - wy;
	m(1, 2) = yz + wx;
	m(2, 2) = 1.0f - (xx + yy);
}

void Quaternion::GetTransformation( Matrix4& m, bool only3x3 ) const
{
	float u = 2.0f / ( v.x * v.x + v.y * v.y + v.z * v.z + w * w );
	float xs = v.x * u;	float ys = v.y * u;	float zs = v.z * u;
	float wx = w * xs;	float wy = w * ys;	float wz = w * zs;
	float xx = v.x * xs; float xy = v.x * ys; float xz = v.x * zs;
	float yy = v.y * ys; float yz = v.y * zs; float zz = v.z * zs;

	m(0, 0) = 1.0f - ( yy + zz );
	m(0, 1) = xy - wz;
	m(0, 2) = xz + wy;

	m(1, 0) = xy + wz;
	m(1, 1) = 1.0f - ( xx + zz );
	m(1, 2) = yz - wx;

	m(2, 0) = xz - wy;
	m(2, 1) = yz + wx;
	m(2, 2) = 1.0f - ( xx + yy );
	
	if ( !only3x3 ) {
		m(0, 3) = 0.0f;
		m(1, 3) = 0.0f;
		m(2, 3) = 0.0f;
		m(3, 0) = 0.0f;
		m(3, 1) = 0.0f;
		m(3, 2) = 0.0f;
		m(3, 3) = 1.0f;

	}
}


Vector3 Quaternion::GetXAxis() const
{
	Vector3 v;
	GetXAxis( v );
	return v;
}

void Quaternion::GetXAxis( Vector3& axis ) const
{
	axis.x = 1.0f -	( v.y * ( v.y + v.y ) + v.z * ( v.z + v.z ));
	axis.y = v.x *	( v.y + v.y ) + w * ( v.z + v.z );
	axis.z = v.x *	( v.z + v.z ) - w * ( v.y + v.y );
}

Vector3 Quaternion::GetYAxis() const
{
	Vector3 v;
	GetYAxis( v );
	return v;
}

void Quaternion::GetYAxis( Vector3& axis ) const
{
	axis.x = v.x *	( v.y + v.y ) - w * ( v.z + v.z );
	axis.y = 1.0f -	( v.x * ( v.x + v.x ) + v.z * ( v.z + v.z ));
	axis.z = v.y *	( v.z + v.z ) + w * ( v.x + v.x );
}

Vector3 Quaternion::GetZAxis() const
{
	Vector3 v;
	GetZAxis( v );
	return v;
}

void Quaternion::GetZAxis( Vector3& axis ) const
{
	axis.x = v.x *	( v.z + v.z ) + w * ( v.y + v.y );
	axis.y = v.y *	( v.z + v.z ) - w * ( v.x + v.x );
	axis.z = 1.0f -	( v.x * ( v.x + v.x ) + v.y * ( v.y + v.y ));
}

Quaternion& Quaternion::FromAxes( const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis )
{
	Matrix3 kRot;

	kRot(0,0) = xAxis.x;
	kRot(1,0) = xAxis.y;
	kRot(2,0) = xAxis.z;

	kRot(0,1) = yAxis.x;
	kRot(1,1) = yAxis.y;
	kRot(2,1) = yAxis.z;

	kRot(0,2) = zAxis.x;
	kRot(1,2) = zAxis.y;
	kRot(2,2) = zAxis.z;

	Set( kRot );
	return *this;
}

void Quaternion::ToAxes( Vector3& xAxis, Vector3& yAxis, Vector3& zAxis ) const
{
	Matrix3 kRot;

	GetTransformation( kRot );

	xAxis.x = kRot(0,0);
	xAxis.y = kRot(1,0);
	xAxis.z = kRot(2,0);

	yAxis.x = kRot(0,1);
	yAxis.y = kRot(1,1);
	yAxis.z = kRot(2,1);

	zAxis.x = kRot(0,2);
	zAxis.y = kRot(1,2);
	zAxis.z = kRot(2,2);
}



Vector3 Quaternion::Transform( const Vector3& o ) const
{
	return Vector3 
		(
		o.x * ( 1.0f - ( v.y * ( v.y + v.y ) + v.z * ( v.z + v.z ))) +
		o.y * ( v.x * ( v.y + v.y ) - w * ( v.z + v.z )) +
		o.z * ( v.x * ( v.z + v.z ) + w * ( v.y + v.y )),

		o.x * ( v.x * ( v.y + v.y ) + w * ( v.z + v.z )) +
		o.y * ( 1.0f - ( v.x * ( v.x + v.x ) + v.z * ( v.z + v.z ))) +
		o.z * ( v.y * ( v.z + v.z ) - w * ( v.x + v.x )),

		o.x * ( v.x * ( v.z + v.z ) - w * ( v.y + v.y )) +
		o.y * ( v.y * ( v.z + v.z ) + w * ( v.x + v.x )) +
		o.z * ( 1.0f - ( v.x * ( v.x + v.x ) + v.y * ( v.y + v.y )))
		);
}


Vector3 Quaternion::InverseTransform( const Vector3& o ) const
{
	return GetInverse().Transform( o );
	
}


Quaternion& Quaternion::operator *= ( float s )
{
	v *= s;
	w *= s;
	return *this;
}

Quaternion& Quaternion::operator /= ( float s )
{
	return operator *=(1.0f / s);
}

Quaternion Quaternion::operator * ( float s ) const
{
	return Quaternion( *this ) *= s;
}

Quaternion Quaternion::operator / ( float s ) const
{
	return Quaternion( *this ) /= s;
}

inline Quaternion operator * ( float s, const Quaternion& q )
{
	return Quaternion( q ) *= s;
}



Quaternion Quaternion::GetInverse() const
{
	return Quaternion( *this ).Invert();
}

Quaternion&  Quaternion::Invert()
{
	float mag = MagnitudeSquared();
	Conjugate();
	return operator /= (mag);
}


float Quaternion::SDot( const Quaternion& q ) const
{
	return v.SDot( q.v ) + w * q.w;
}


float Quaternion::Magnitude() const
{
	return sqrtf( v.SDot( v ) + w * w );
}

float Quaternion::MagnitudeSquared() const
{
	return v.SDot( v ) + w * w;
}


Quaternion Quaternion::GetConjugate() const
{
	return Quaternion( *this ).Conjugate();
}

Quaternion& Quaternion::Conjugate()
{
	v = -v;
	return *this;
}


Quaternion Quaternion::GetNormalized() const
{
	return Quaternion( *this ).Normalize();
}

Quaternion& Quaternion::Normalize() 
{
	return operator /= (Magnitude());
}

Quaternion& Quaternion::operator += ( const Quaternion& q )
{
	v += q.v;
	w += q.w;
	return *this;
}

Quaternion& Quaternion::operator -= ( const Quaternion& q )
{
	v -= q.v;
	w -= q.w;
	return *this;
}

Quaternion& Quaternion::operator *= ( const Quaternion& q )
{
	Quaternion temp = *this;
	v = temp.v.SCross( q.v ) + temp.w * q.v + q.w * temp.v;
	w = temp.w * q.w - temp.v.SDot( q.v );
	return *this;
}

Quaternion& Quaternion::operator /= ( const Quaternion& q )
{
	(*this) *= q.GetInverse();
	return *this;
}

Quaternion Quaternion::operator + ( const Quaternion& q ) const
{
	return Quaternion( *this ) += q;
}

Quaternion Quaternion::operator - ( const Quaternion& q ) const
{
	return Quaternion( *this ) -= q;
}

Quaternion Quaternion::operator * ( const Quaternion& q ) const
{
	Quaternion res;
	res.v = v.SCross( q.v ) + w * q.v + q.w * v;
	res.w = w * q.w - v.SDot( q.v );
	return res;
}

Quaternion Quaternion::operator / ( const Quaternion& q ) const
{
	return (*this) * q.GetInverse();
}


bool Quaternion::Equal( const Quaternion& q, float tolerance ) const
{
	return (fabsf( w - q.w ) <= tolerance && v.Equal( q.v, tolerance ) );
}

bool Quaternion::NotEqual( const Quaternion& q, float tolerance ) const
{
	return ! Equal( q, tolerance );
}


bool Quaternion::operator == ( const Quaternion& q ) const
{
	return ( w == q.w ) && ( v == q.v );
}

bool Quaternion::operator != ( const Quaternion& q ) const
{

	return ( w != q.w ) || ( v != q.v );
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