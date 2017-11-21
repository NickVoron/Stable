/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 22.07.2001
	  Time: 2:07:12

	Abstract:	Standatd 4x1 column	Vector class.

*********************************************************************/
#pragma once

#include <math.h>
#include "Vector3.h"

#undef Vector4
#undef SDot
#undef SCross

// Vector4 class
class Vector4 {
public:

	// Public data
	float x, y, z, w;

	static const Vector4 zero;
	static const Vector4 one;
	static const Vector4 xAxis;
	static const Vector4 yAxis;
	static const Vector4 zAxis;
	static const Vector4 wAxis;
	static const Vector4 xNegAxis;
	static const Vector4 yNegAxis;
	static const Vector4 zNegAxis;
	static const Vector4 wNegAxis;


	// Ctor
	inline Vector4();
	inline Vector4( const Vector2& vector );
	inline Vector4( const Vector3& vector );
	inline Vector4( float x, float y );
	inline Vector4( float x, float y, float z );
	inline Vector4( float x, float y, float z, float w );
	inline Vector4( const float vector[] );

	// Load / save
	inline void Load( const float matrix[] );
	inline void Save( float matrix[] ) const;

	// Reset
	inline void Zero();
	inline void Fill(float val);

	// Access
	inline float& operator [] (size_t index );
	inline float operator [] (size_t index ) const;

	// Fast convert
	inline const Vector2& GetVector2() const;
	inline const Vector3& GetVector3() const;

	// Take this 4d vector back to 3d space.
	inline void Homogenize();

	// Scalar
	inline Vector4&	operator += ( float scalar );
	inline Vector4&	operator -= ( float scalar );
	inline Vector4&	operator *= ( float scalar );
	inline Vector4&	operator /= ( float scalar );

	inline Vector4 operator + ( float scalar ) const;
	inline Vector4 operator - ( float scalar ) const;
	inline Vector4 operator * ( float scalar ) const;
	inline Vector4 operator / ( float scalar ) const;

	friend Vector4 operator + ( float scalar, const Vector4& vector );
	friend Vector4 operator - ( float scalar, const Vector4& vector );
	friend Vector4 operator * ( float scalar, const Vector4& vector );

	// Vector

	inline float SDot( const Vector4& vector ) const;

	inline Vector4	SCross( const Vector4& vector) const;
	inline void		SCross( const Vector4& vector, Vector4& out ) const;

	inline float	Projection( const Vector4& vector ) const;

	inline float	Magnitude() const;
	inline float	MagnitudeSquared() const;

	inline float	InvMagnitude() const;
	inline float	InvMagnitudeSquared() const;

	inline Vector4	GetNormalized() const;
	inline Vector4&	Normalize();

	inline float	ApproxMagnitude() const;
	inline float	ApproxInvMagnitude() const;

	inline Vector4	GetApproxNormalized() const;
	inline Vector4&	ApproxNormalize();

	inline Vector4 PerComponentMul(const Vector4& v) const;

	// Unary
	friend Vector4 operator + ( const Vector4& vector );
	friend Vector4 operator - ( const Vector4& vector );
	
	// Infix
	inline Vector4& operator = ( const Vector2& vector );
	inline Vector4& operator = ( const Vector3& vector );
	inline Vector4& operator += ( const Vector4& vector );
	inline Vector4& operator -= ( const Vector4& vector );

	template<class Vector4Type>
	inline Vector4& operator = ( const Vector4Type& vector )
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;

		return *this;
	}

	inline Vector4 operator + ( const Vector4& vector ) const;
	inline Vector4 operator - ( const Vector4& vector ) const;

	// Logical compare
	inline bool Equal( const Vector4& vector, float tolerance = 0.000001f) const;
	inline bool NotEqual( const Vector4& vector, float tolerance = 0.000001f) const;

	// Binary
	inline bool operator == ( const Vector4& vector ) const;
	inline bool operator != ( const Vector4& vector ) const;

protected:

	inline float ApproximateMagnitude( float x1, float y1 ) const;
};


//////////////////////////////////////////////////////////////////////
// Implementation

Vector4::Vector4()
{}

Vector4::Vector4( const Vector2& vector )
{
	x = vector.x;
	y = vector.y;
	z = 0.0f;
	w = 1.0f;
}

Vector4::Vector4( const Vector3& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = 1.0f;
}

Vector4::Vector4( float ax, float ay )
{
	x = ax;
	y = ay;
	z = 0.0f;
	w = 1.0f;
}

Vector4::Vector4( float ax, float ay, float az )
{
	x = ax;
	y = ay;
	z = az;
	w = 1.0f;
}

Vector4::Vector4( float ax, float ay, float az, float aw )
{
	x = ax;
	y = ay;
	z = az;
	w = aw;
}

Vector4::Vector4( const float vector[] )
{
	x = vector[0];
	y = vector[1];
	z = vector[2];
	w = vector[3];
}

// 
void Vector4::Load( const float vector[] ) 
{
	x = vector[0];
	y = vector[1];
	z = vector[2];
	w = vector[3];
}

void Vector4::Save( float vector[] ) const
{
	vector[0]	= x;
	vector[1]	= y;
	vector[2]	= z;
	vector[3]	= w;
}

//
void Vector4::Zero() { x = y = z = w = 0.0f; }
void Vector4::Fill(float val) { x = y = z = w = val;  }

//
float& Vector4::operator [] (size_t index )
{
	return *((&x) + index );
}

float Vector4::operator [] (size_t index ) const
{
	return *((&x) + index );
}

//
const Vector2& Vector4::GetVector2() const
{
	return *(Vector2*)this;
}

const Vector3& Vector4::GetVector3() const
{
	return *(Vector3*)this;
}

//
void Vector4::Homogenize()
{
	float invW = 1.0f / w;
	x *= invW;
	y *= invW;
	z *= invW;
	w = 1.0f;
}

//
Vector4& Vector4::operator += ( float scalar )
{
	x += scalar;
	y += scalar;
	z += scalar;
	w += scalar;
	return *this;
}

Vector4& Vector4::operator -= ( float scalar )
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
	w -= scalar;
	return *this;
}

Vector4& Vector4::operator *= ( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

Vector4& Vector4::operator /= ( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

Vector4 Vector4::operator + ( float scalar ) const
{
	return Vector4( *this ) += scalar;
}

Vector4 Vector4::operator - ( float scalar ) const
{
	return Vector4( *this ) -= scalar;
}

Vector4 Vector4::operator * ( float scalar ) const
{
	return Vector4( *this ) *= scalar;
}

Vector4 Vector4::operator / ( float scalar ) const
{
	return Vector4( *this ) /= scalar;
}

inline Vector4 operator + ( float scalar, const Vector4& vector )
{
	return Vector4(vector) += scalar;
	//return vector + scalar;
}

inline Vector4 operator - ( float scalar, const Vector4& vector )
{
	return Vector4(vector) -= scalar;
	//return vector - scalar;
}

inline Vector4 operator * ( float scalar, const Vector4& vector )
{
	return Vector4(vector) *= scalar;
	//return vector * scalar;
}

//

float Vector4::SDot( const Vector4& vector ) const
{
	return	x * vector.x +
			y * vector.y +
			z * vector.z +
			w * vector.w;
}

Vector4 Vector4::SCross( const Vector4& vector ) const
{
	// convert from homogenius to three space
	float a_oow	= 1.0f / w;
	float ax	= x * a_oow;
	float ay	= y * a_oow;
	float az	= z * a_oow;
	float b_oow	= 1.0f / vector.w;
	float bx	= vector.x * b_oow;
	float by	= vector.y * b_oow;
	float bz	= vector.z * b_oow;

	// calculate and return cross product in three space
	return Vector4( ay * bz - az * by,
					az * bx - ax * bz,
					ax * by - ay * bx );
}

void Vector4::SCross( const Vector4& vector, Vector4& out ) const
{
	// convert from homogenius to three space
	float a_oow	= 1.0f / w;
	float ax	= x * a_oow;
	float ay	= y * a_oow;
	float az	= z * a_oow;
	float b_oow	= 1.0f / vector.w;
	float bx	= vector.x * b_oow;
	float by	= vector.y * b_oow;
	float bz	= vector.z * b_oow;
	

	out.x	= ay * bz - az * by;
	out.y	= az * bx - ax * bz;
	out.z	= ax * by - ay * bx;
}

float Vector4::Projection( const Vector4& vector ) const
{
	return SDot( vector.GetNormalized() );
}

float Vector4::Magnitude() const
{
	return sqrtf( MagnitudeSquared() );
}

float Vector4::MagnitudeSquared() const
{
	return x * x + y * y + z * z + w * w;
}

float Vector4::InvMagnitude() const
{
	float mag = Magnitude();
	return 1.0f / mag;
}

float Vector4::InvMagnitudeSquared() const
{
	float mag = MagnitudeSquared();
	return 1.0f / mag;
}

//

Vector4 Vector4::GetNormalized() const
{
	float mag = Magnitude();
	float inv_mag = 1.0f / mag;
	return Vector4( x * inv_mag, y * inv_mag, z * inv_mag, w * inv_mag );
}

Vector4& Vector4::Normalize()
{
	float inv_mag = InvMagnitude();

	x *= inv_mag;
	y *= inv_mag;
	z *= inv_mag;
	w *= inv_mag;

	return *this;
}

inline Vector4 Vector4::PerComponentMul(const Vector4& v) const
{
	return Vector4(x*v.x, y*v.y, z*v.z, w*v.w);
}

float Vector4::ApproximateMagnitude( float x1, float y1 ) const
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

float Vector4::ApproxMagnitude() const
{
	return ApproximateMagnitude( ApproximateMagnitude( ApproximateMagnitude( x, y ), z ), w );
}

float Vector4::ApproxInvMagnitude() const
{
	float mag = ApproxMagnitude();
	return 1.0f / mag;
}

Vector4 Vector4::GetApproxNormalized() const
{
	float mag = ApproxMagnitude();
	float inv_mag = 1.0f / mag;
	return Vector4( x * inv_mag, y * inv_mag, z * inv_mag, w * inv_mag );
}

Vector4& Vector4::ApproxNormalize()
{
	float mag = ApproxMagnitude();
	float inv_mag = 1.0f / mag;

	x *= inv_mag;
	y *= inv_mag;
	z *= inv_mag;
	w *= inv_mag;

	return *this;
}

// Unary
inline Vector4 operator - ( const Vector4& vector )
{
	return Vector4( -vector.x, -vector.y, -vector.z, -vector.w );
}

inline Vector4 operator + ( const Vector4& vector )
{
	return vector;
}

//

Vector4& Vector4::operator = ( const Vector2& vector )
{
	x = vector.x;
	y = vector.y;
	z = 0.0f;
	w = 1.0f;
	return *this;
}

Vector4& Vector4::operator = ( const Vector3& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = 1.0f;
	return *this;
}

Vector4& Vector4::operator += ( const Vector4& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;
	return *this;
}

Vector4& Vector4::operator -= ( const Vector4& vector )
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;
	return *this;
}

Vector4 Vector4::operator + ( const Vector4& vector ) const
{
	return Vector4( *this ) += vector;
}

Vector4 Vector4::operator - ( const Vector4& vector ) const
{
	return Vector4( *this ) -= vector;
}

//

bool Vector4::Equal( const Vector4& vector, float tolerance ) const
{
	return (fabsf(x - vector.x) <= tolerance &&
			fabsf(y - vector.y) <= tolerance &&
			fabsf(z - vector.z) <= tolerance &&
			fabsf(w - vector.w) <= tolerance );
}

bool Vector4::NotEqual( const Vector4& vector, float tolerance ) const
{
	return !Equal( vector, tolerance );
}

bool Vector4::operator == ( const Vector4& vector ) const
{
	return (x == vector.x &&
			y == vector.y &&
			z == vector.z &&
			w == vector.w );
}

bool Vector4::operator != ( const Vector4& vector ) const
{
	return ! operator == ( vector );
}
