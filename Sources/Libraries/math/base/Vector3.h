// Copyright (C) 2012-2017 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>, Denis Netakhin <denis.netahin@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//


#pragma once

#include <math.h>
#include "Vector2.h"


class Vector3 {
public:
	
	float x, y, z;

	
	static const Vector3 max;
	
	static const Vector3 nan;
	
	static const Vector3 zero;
	
	static const Vector3 one;
	
	static const Vector3 xAxis;
	static const Vector3 yAxis;
	static const Vector3 zAxis;
	static const Vector3 xNegAxis;
	static const Vector3 yNegAxis;
	static const Vector3 zNegAxis;
	static const Vector3 axes[3];



	
	inline	Vector3();
	inline	Vector3( float x, float y, float z );
	inline	Vector3( const float vector[] );

	template<class VectorClass>
	inline Vector3(const VectorClass& vector) { operator=(vector); }

	
	inline void Load( const float vector[] );
	inline void Save( float vector[] ) const;

	
	inline void Zero();
	static inline Vector3 GetZero();

	
	inline float& operator [] (size_t index );
	inline float operator [] (size_t index ) const;

	
	inline const Vector2&	GetVector2() const;

	
	inline Vector3&	operator += ( float scalar );
	inline Vector3& operator -= ( float scalar );
	inline Vector3&	operator *= ( float scalar );
	inline Vector3&	operator /= ( float scalar );

	inline Vector3 operator + ( float scalar ) const;
	inline Vector3 operator - ( float scalar ) const;
	inline Vector3 operator * ( float scalar ) const;
	inline Vector3 operator / ( float scalar ) const;

	friend Vector3 operator + ( float scalar, const Vector3& vector );
	friend Vector3 operator - ( float scalar, const Vector3& vector );
	friend Vector3 operator * ( float scalar, const Vector3& vector );

	
	Vector3& PerComponentMul( const Vector3& b ) { x *= b.x; y*= b.y; z*=b.z;  return *this; }
	Vector3 GetPerComponentMul( const Vector3& b ) const { Vector3 r(*this); r.PerComponentMul(b); return r; }

	

	inline float SDot( const Vector3& vector ) const;

	inline Vector3	SCross( const Vector3& vector ) const;
	inline void		SCross( const Vector3& vector, Vector3& out ) const;

	inline float	Projection( const Vector3& vector ) const;
	inline Vector3	GetProjectionTo( const Vector3& b ) const;

	inline float	Magnitude() const;
	inline float	MagnitudeSquared() const;

	inline float	InvMagnitude() const;
	inline float	InvMagnitudeSquared() const;

	inline Vector3	GetNormalized() const;
	inline Vector3&	Normalize();
	inline Vector3& SetMagnitude( float );

	inline float	ApproxMagnitude() const;
	inline float	ApproxInvMagnitude() const;

	inline Vector3	GetApproxNormalized() const;
	inline Vector3&	ApproxNormalize();

	
	inline float GetSphereAsimuthAngle() const;

	inline bool isNaN() const;

	
	friend Vector3	operator + ( const Vector3& vector );
	friend Vector3	operator - ( const Vector3& vector );

	
	inline Vector3&	operator += ( const Vector3& vector );
	inline Vector3& operator -= ( const Vector3& vector );
	inline Vector3& operator *= ( const Vector3& vector );

	template<class VectorClass>
	inline Vector3&	operator = ( const VectorClass& vector );

	inline Vector3	operator + ( const Vector3& vector ) const;
	inline Vector3	operator - ( const Vector3& vector ) const;

	
	inline bool	Equal( const Vector3& vector, float tolerance = 0.000001f) const;
	inline bool Collinear(const Vector3& vector, float tolerance = 0.000001f) const;
	inline bool NotEqual ( const Vector3& vector, float tolerance = 0.000001f) const;

	
	inline bool operator == ( const Vector3& vector ) const;
	inline bool operator != ( const Vector3& vector ) const;

protected:

	inline float ApproximateMagnitude( float x1, float y1 ) const; 
};





Vector3::Vector3()
{}

Vector3::Vector3( float ax, float ay, float az )
{
	x	= ax;
	y	= ay;
	z	= az;
}

Vector3::Vector3( const float vector[] ) 
{
	x	= vector[0];
	y	= vector[1];
	z	= vector[2];
}

void Vector3::Load( const float vector[] )
{
	x	= vector[0];
	y	= vector[1];
	z	= vector[2];
}

void Vector3::Save( float vector[] ) const
{
	vector[0]	= x;
	vector[1]	= y;
	vector[2]	= z;
}

void Vector3::Zero()
{
	x = y = z = 0.0f;
}

Vector3 Vector3::GetZero()
{
	return Vector3( 0.0f, 0.0f, 0.0f );
}



float& Vector3::operator [] (size_t index )
{
	return *((&x) + index);
}

float Vector3::operator [] (size_t index ) const
{
	return *((&x) + index);
}



const Vector2& Vector3::GetVector2() const
{
	return *(Vector2*)this;
}



Vector3& Vector3::operator += ( float scalar )
{
	x	+= scalar;
	y	+= scalar;
	z	+= scalar;
	return *this;
}

Vector3& Vector3::operator -= ( float scalar )
{
	x	-= scalar;
	y	-= scalar;
	z	-= scalar;
	return *this;
}

Vector3& Vector3::operator *= ( float scalar )
{
	x	*= scalar;
	y	*= scalar;
	z	*= scalar;
	return *this;
}

Vector3& Vector3::operator /= ( float scalar )
{
	x	/= scalar;
	y	/= scalar;
	z	/= scalar;
	return *this;
}

Vector3 Vector3::operator + ( float scalar ) const
{
	return Vector3( *this ) += scalar;
}

Vector3 Vector3::operator - ( float scalar ) const
{
	return Vector3( *this ) -= scalar;
}

Vector3 Vector3::operator * ( float scalar ) const
{
	return Vector3( *this ) *= scalar;
}

Vector3 Vector3::operator / ( float scalar ) const
{
	return Vector3( *this ) /= scalar;
}

inline Vector3 operator + ( float scalar, const Vector3& vector )
{
	return vector + scalar;
}

inline Vector3 operator - ( float scalar, const Vector3& vector )
{
	return vector - scalar;
}

inline Vector3 operator * ( float scalar, const Vector3& vector )
{
	return vector * scalar;
}



float Vector3::SDot( const Vector3& vector ) const
{
	return x * vector.x + y * vector.y + z * vector.z;
}

Vector3 Vector3::SCross( const Vector3& vector ) const
{
	return Vector3( y * vector.z - z * vector.y,
					z * vector.x - x * vector.z,
					x * vector.y - y * vector.x );
}

void Vector3::SCross( const Vector3& vector, Vector3& out ) const
{
	out.x	= y * vector.z - z * vector.y;
	out.y	= z * vector.x - x * vector.z;
	out.z	= x * vector.y - y * vector.x;
}

float Vector3::Projection( const Vector3& vector ) const
{
	return SDot( vector.GetNormalized() );
}

Vector3 Vector3::GetProjectionTo( const Vector3& b ) const
{
	return b * SDot( b.GetNormalized() ) * b.InvMagnitude();
	
}

float Vector3::Magnitude() const
{
	return sqrtf( MagnitudeSquared() );
}

float Vector3::MagnitudeSquared() const
{
	return x * x + y * y + z * z;
}

float Vector3::InvMagnitude() const
{
	float mag = Magnitude();
	return 1.0f / mag;
}

float Vector3::InvMagnitudeSquared() const
{
	float mag = MagnitudeSquared();
	return 1.0f / mag;
}

Vector3 Vector3::GetNormalized() const
{
	float inv_mag = InvMagnitude();

	return Vector3( x * inv_mag, y * inv_mag, z * inv_mag );
}

Vector3& Vector3::Normalize()
{
	float inv_mag = InvMagnitude();

	x *= inv_mag;
	y *= inv_mag;
	z *= inv_mag;

	return *this;
}

Vector3& Vector3::SetMagnitude( float norm )
{
	float new_mag = InvMagnitude() * norm;

	x *= new_mag;
	y *= new_mag;
	z *= new_mag;

	return *this;
}

float Vector3::ApproximateMagnitude( float x1, float y1 ) const
{
	x1 = x1 < 0.0f ? -x1 : x1;
	y1 = y1 < 0.0f ? -y1 : y1;

	if ( x1 < 0.414213f * y1 )
		return y1 + 0.483608f * x1 * x1 / y1;

	if ( y1 < 0.414213f * x1 )
		return x1 + 0.483608f * y1 * y1 / x1;

	
	float sum = x1 + y1;
	if ( sum ) 
		return 1.04907f * sum - 1.36785f * x1 * y1 / sum;
	else 
		return 0.0f;
}

float Vector3::ApproxMagnitude() const
{
	return ApproximateMagnitude( ApproximateMagnitude( x, y ), z );
}

float Vector3::ApproxInvMagnitude() const
{
	float mag = ApproxMagnitude();
	return 1.0f / mag;
}

Vector3 Vector3::GetApproxNormalized() const
{
	float inv_mag = 1.0f / ApproxMagnitude();
	
	return Vector3( x * inv_mag, y * inv_mag, z * inv_mag );
}

Vector3& Vector3::ApproxNormalize()
{
	float inv_mag = 1.0f / ApproxMagnitude();

	x	*= inv_mag;
	y	*= inv_mag;
	z	*= inv_mag;

	return *this;
}


inline float Vector3::GetSphereAsimuthAngle() const
{
	float asimuth = atan2f(x, z);
	if (asimuth < 0) 
	{
		asimuth += 3.14159265358979323846f * 2; 
	}
	return asimuth;
}


inline Vector3 operator - ( const Vector3& vector )
{
	return Vector3( -vector.x, -vector.y, -vector.z );
}

inline Vector3 operator + ( const Vector3& vector )
{
	return vector;
}


template<class VectorClass>
inline Vector3&	Vector3::operator = ( const VectorClass& vector )
{
	x	= vector.x;
	y	= vector.y;
	z	= vector.z;
	return *this;
}

Vector3& Vector3::operator += ( const Vector3& vector )
{
	x	+= vector.x;
	y	+= vector.y;
	z	+= vector.z;
	return *this;
}

Vector3& Vector3::operator -= ( const Vector3& vector )
{
	x	-= vector.x;
	y	-= vector.y;
	z	-= vector.z;
	return *this;
}

Vector3 Vector3::operator + ( const Vector3& vector ) const
{
	return Vector3( *this ) += vector;
}

Vector3 Vector3::operator - ( const Vector3& vector ) const
{
	return Vector3( *this ) -= vector;
}



bool Vector3::Equal( const Vector3& vector, float tolerance ) const
{
	return (fabsf( x - vector.x ) <= tolerance &&
			fabsf( y - vector.y ) <= tolerance &&
			fabsf( z - vector.z ) <= tolerance );
}

bool Vector3::Collinear(const Vector3& vector, float tolerance ) const
{
	return SCross(vector).Magnitude() < tolerance;
}

bool Vector3::NotEqual( const Vector3& vector, float tolerance ) const
{
	return ! Equal( vector, tolerance );
}



bool Vector3::operator == ( const Vector3& vector ) const
{
	return (x == vector.x &&
			y == vector.y &&
			z == vector.z);
}

bool Vector3::operator != ( const Vector3& vector ) const
{
	return ! operator == ( vector );
}


bool Vector3::isNaN() const
{
	return ( isnan(x) || isnan(y) || isnan(z));
}



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