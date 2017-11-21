/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 21.07.2001
	  Time: 22:30:33

	Abstract:	Standard 3x1 Column vector class.

*********************************************************************/
#pragma once

#include <math.h>
#include "Vector2.h"

// Vector3 class
class Vector3 {
public:
	// Public data
	float x, y, z;

	//
	static const Vector3 max;
	//Nan
	static const Vector3 nan;
	//нулевой константный статический вектор
	static const Vector3 zero;
	//единичный константный статический вектор
	static const Vector3 one;
	//константный статический вектор 1-по оси Х
	static const Vector3 xAxis;
	static const Vector3 yAxis;
	static const Vector3 zAxis;
	static const Vector3 xNegAxis;
	static const Vector3 yNegAxis;
	static const Vector3 zNegAxis;
	static const Vector3 axes[3];



	// Ctors
	inline	Vector3();
	inline	Vector3( float x, float y, float z );
	inline	Vector3( const float vector[] );

	template<class VectorClass>
	inline Vector3(const VectorClass& vector):Vector3(vector.x, vector.y, vector.z) {}

	// Load / save
	inline void Load( const float vector[] );
	inline void Save( float vector[] ) const;

	// Reset
	inline void Zero();
	static inline Vector3 GetZero();

	// Access
	inline float& operator [] (size_t index );
	inline float operator [] (size_t index ) const;

	// Conv
	inline const Vector2&	GetVector2() const;

	// Scalar
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

	// additional scaling:
	Vector3& PerComponentMul( const Vector3& b ) { x *= b.x; y*= b.y; z*=b.z;  return *this; }
	Vector3 GetPerComponentMul( const Vector3& b ) const { Vector3 r(*this); r.PerComponentMul(b); return r; }

	// Vector ops

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

	// получить сферический угол азимут нормализованного вектора
	inline float GetSphereAsimuthAngle() const;

	inline bool isNaN() const;

	// Unary
	friend Vector3	operator + ( const Vector3& vector );
	friend Vector3	operator - ( const Vector3& vector );

	// Infix
	inline Vector3&	operator += ( const Vector3& vector );
	inline Vector3& operator -= ( const Vector3& vector );
	inline Vector3& operator *= ( const Vector3& vector );

// 	template<class VectorClass>
// 	inline Vector3&	operator = ( const VectorClass& vector );

	inline Vector3	operator + ( const Vector3& vector ) const;
	inline Vector3	operator - ( const Vector3& vector ) const;

	// Logical compare
	inline bool	Equal( const Vector3& vector, float tolerance = 0.000001f) const;
	inline bool Collinear(const Vector3& vector, float tolerance = 0.000001f) const;
	inline bool NotEqual ( const Vector3& vector, float tolerance = 0.000001f) const;

	// Binary compare
	inline bool operator == ( const Vector3& vector ) const;
	inline bool operator != ( const Vector3& vector ) const;

protected:

	inline float ApproximateMagnitude( float x1, float y1 ) const; 
};


/////////////////////////////////////////////
// Implementation

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

// Access

float& Vector3::operator [] (size_t index )
{
	return *((&x) + index);
}

float Vector3::operator [] (size_t index ) const
{
	return *((&x) + index);
}

// Conv

const Vector2& Vector3::GetVector2() const
{
	return *(Vector2*)this;
}

// Scalar

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

// SDot product

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
	//return b * b.Projection( *this ) * b.InvMagnitude();
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

	// function returns infinite float then zero length used
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

// получить сферический угол азимут нормализованного вектора
inline float Vector3::GetSphereAsimuthAngle() const
{
	float asimuth = atan2f(x, z);
	if (asimuth < 0) 
	{
		asimuth += 3.14159265358979323846f * 2; // 360 градусов
	}
	return asimuth;
}

// Unary
inline Vector3 operator - ( const Vector3& vector )
{
	return Vector3( -vector.x, -vector.y, -vector.z );
}

inline Vector3 operator + ( const Vector3& vector )
{
	return vector;
}

// Vector 
// template<class VectorClass>
// inline Vector3&	Vector3::operator = ( const VectorClass& vector )
// {
// 	x	= vector.x;
// 	y	= vector.y;
// 	z	= vector.z;
// 	return *this;
// }

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

// Logic

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

// Binary

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